/*
 * SPDX-FileCopyrightText: 2016-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "string.h"
#include "esp_log.h"
#include "modbus_params.h"  // for modbus parameters structures
#include "mbcontroller.h"
#include "sdkconfig.h"              // for KConfig options
//#include "include/map_sunspec.h"
#include "include/master.h"


static const char *TAG = "MASTER_TEST";


// Example Data (Object) Dictionary for Modbus parameters:
// The CID field in the table must be unique.
// Modbus Slave Addr field defines slave address of the device with correspond parameter.
// Modbus Reg Type - Type of Modbus register area (Holding register, Input Register and such).
// Reg Start field defines the start Modbus register number and Reg Size defines the number of registers for the characteristic accordingly.
// The Instance Offset defines offset in the appropriate parameter structure that will be used as instance to save parameter value.
// Data Type, Data Size specify type of the characteristic and its data size.
// Parameter Options field specifies the options that can be used to process parameter value (limits or masks).
// Access Mode - can be used to implement custom options for processing of characteristic (Read/Write restrictions, factory mode values and etc).
const uint16_t num_Register = 200;
static uint16_t full_ptr[248*2];
static uint16_t *currPos= full_ptr;

const mb_parameter_descriptor_t device_parameters[] = {
    // { CID, Param Name, Units, Modbus Slave Addr, Modbus Reg Type, Reg Start, Reg Size, Instance Offset, Data Type, Data Size, Parameter Options, Access Mode}
    { CID_ID_TABLE, STR("ID"), STR("0x"), MB_DEVICE_ADDR1, MB_PARAM_HOLDING, MB_BASE_ADDR+69, 1,
            HOLD_OFFSET(holding_data0), PARAM_TYPE_U16, (2*1), OPTS( 0, 100, 1 ), PAR_PERMS_READ_WRITE_TRIGGER },
    { CID_TABLE_101, STR("TABLE_101"), STR("0x"), MB_DEVICE_ADDR1, MB_PARAM_HOLDING, MB_BASE_ADDR+0, 119,
            HOLD_OFFSET(holding_data0), PARAM_TYPE_U16, (2*(119)), OPTS( 0, 100, 1 ), PAR_PERMS_READ_WRITE_TRIGGER },
    { CID_TABLE_111_0, STR("TABLE_111"), STR("0x"), MB_DEVICE_ADDR1, MB_PARAM_HOLDING, MB_BASE_ADDR+0, 125,
            HOLD_OFFSET(holding_data0), PARAM_TYPE_U16, (2*(125)), OPTS( 0, 100, 1 ), PAR_PERMS_READ_WRITE_TRIGGER },
    { CID_TABLE_111_1, STR("TABLE_111_1"), STR("0x"), MB_DEVICE_ADDR1, MB_PARAM_HOLDING, MB_BASE_ADDR+125, 4,
            HOLD_OFFSET(holding_data0), PARAM_TYPE_U16, (2*(4)), OPTS( 0, 100, 1 ), PAR_PERMS_READ_WRITE_TRIGGER }
};

// Calculate number of parameters in the table
const uint16_t num_device_parameters = (sizeof(device_parameters)/sizeof(device_parameters[0]));

// The function to get pointer to parameter storage (instance) according to parameter description table
static void* master_get_param_data(const mb_parameter_descriptor_t* param_descriptor)
{
    assert(param_descriptor != NULL);
    void* instance_ptr = NULL;
    if (param_descriptor->param_offset != 0) {
       switch(param_descriptor->mb_param_type)
       {
           case MB_PARAM_HOLDING:
               instance_ptr = ((void*)&holding_reg_params + param_descriptor->param_offset - 1);
               break;
           case MB_PARAM_INPUT:
               instance_ptr = ((void*)&input_reg_params + param_descriptor->param_offset - 1);
               break;
           case MB_PARAM_COIL:
               instance_ptr = ((void*)&coil_reg_params + param_descriptor->param_offset - 1);
               break;
           case MB_PARAM_DISCRETE:
               instance_ptr = ((void*)&discrete_reg_params + param_descriptor->param_offset - 1);
               break;
           default:
               instance_ptr = NULL;
               break;
       }
    } else {
        ESP_LOGE(TAG, "Wrong parameter offset for CID #%u", (unsigned)param_descriptor->cid);
        assert(instance_ptr != NULL);
    }
    return instance_ptr;
}

// User operation function to read slave values and check alarm
void master_operation_func(void *arg,esp_mqtt_client_handle_t *client)
{
    esp_err_t err = ESP_OK;
    float value = 0;
    bool alarm_state = false;
    int llll = 0 ;
    const mb_parameter_descriptor_t* param_descriptor = NULL;
    esp_mqtt_event_t *event = (esp_mqtt_event_t *)malloc(sizeof(esp_mqtt_event_t)+1);
    char *topic = (char*)malloc((32+32+3+4+4) * sizeof(char) + 1);
    *topic = '\0';
    snprintf(topic,64+4+4+3+1,"v1/devices/me/telemetry");

    ESP_LOGI(TAG, "Start modbus test...");

    for(uint16_t retry = 0; (!alarm_state); retry++) {
        // Read all found characteristics from slave(s)
        uint16_t num=0;
        uint16_t cid=0;
        currPos = full_ptr;
        uint16_t table_id = 0;
        while ((err != ESP_ERR_NOT_FOUND) && cid < MASTER_MAX_CIDS)
        {
            if (cid != CID_TABLE_111_0){
                num = 0;
                currPos = full_ptr;
            }
            // Get data from parameters description table
            // and use this information to fill the characteristics description table
            // and having all required fields in just one table
            err = mbc_master_get_cid_info(cid, &param_descriptor);
            if ((err != ESP_ERR_NOT_FOUND) && (param_descriptor != NULL)) {
                void* temp_data_ptr = master_get_param_data(param_descriptor);
                assert(temp_data_ptr);
                uint8_t type = 0;
                err = mbc_master_get_parameter(cid, (char*)param_descriptor->param_key,
                                                    (uint8_t*)temp_data_ptr, &type);
                if (err == ESP_OK) {
                    
                    if ((param_descriptor->mb_param_type == MB_PARAM_HOLDING) ||
                        (param_descriptor->mb_param_type == MB_PARAM_INPUT)) {
                        llll++;
                        value = *(float*)(temp_data_ptr);
                        
                        ESP_LOGI(TAG, "Characteristic #%u(%s) (%s) value = %f (0x%" PRIx32 ")%i read successful.",
                                        param_descriptor->cid,
                                        param_descriptor->param_key,
                                        param_descriptor->param_units,
                                        value,
                                        *(uint32_t*)temp_data_ptr,
                                        param_descriptor->mb_size*2);
                        memcpy(currPos,temp_data_ptr,param_descriptor->mb_size*2);
                        if (cid == CID_ID_TABLE){
                            id_and_l tmp = *(id_and_l *)temp_data_ptr;
                            ESP_LOGI(TAG,"ID = %u | L = %u", tmp.ID,tmp.L);
                            table_id = tmp.ID;
                        }
                        char *str_mqtt = NULL;
                        if (cid == CID_TABLE_101 && table_id == 101){
                            str_mqtt = convert_map_to_string(full_ptr,SUNSPEC_TABEL_101);
                        }   
                        if (cid == CID_TABLE_111_1 && table_id == 111){
                            str_mqtt = convert_map_to_string(full_ptr,SUNSPEC_TABEL_111);
                        }
                        if(str_mqtt){
                            event->event_id = MQTT_USER_EVENT;
                            event->data = str_mqtt;
                            event->client = *client;
                            event->topic = topic;
                            esp_mqtt_dispatch_custom_event(*client, event);
                            //ESP_LOGI(TAG,"%s %s",topic,str_mqtt);
                        }
                        cid++;
                        if (cid < MASTER_MAX_CIDS)
                        {
                            currPos += param_descriptor->mb_size;
                        }
                        num++;
                        
                        //char *str_mqtt = convert_map_to_string(temp_data_ptr,map_ID_inverter_types,sizeof(map_ID_inverter_types)/sizeof(map_ID_inverter_types[0]),map_ID_inverter_labels);
                        //esp_mqtt_event_t *event = (esp_mqtt_event_t *)malloc(sizeof(esp_mqtt_event_t)+1);
                        //event->event_id = MQTT_USER_EVENT;
                        //event->data = str_mqtt;
                        //event->client = *client;
                        //event->topic = topic;
                        //esp_mqtt_dispatch_custom_event(*client, event);
                        //ESP_LOGI(TAG,"%s %s",topic,str_mqtt);
                        //if (((value > param_descriptor->param_opts.max) ||
                        //    (value < param_descriptor->param_opts.min))) {
                        //        alarm_state = true;
                        //        break;
                        //}
                    } else {
                        uint8_t state = *(uint8_t*)temp_data_ptr;
                        const char* rw_str = (state & param_descriptor->param_opts.opt1) ? "ON" : "OFF";
                        if ((state & param_descriptor->param_opts.opt2) == param_descriptor->param_opts.opt2) {
                            ESP_LOGI(TAG, "Characteristic #%u %s (%s) value = %s (0x%" PRIx8 ") read successful.",
                                            param_descriptor->cid,
                                            param_descriptor->param_key,
                                            param_descriptor->param_units,
                                            (const char*)rw_str,
                                            *(uint8_t*)temp_data_ptr);
                        } else {
                            ESP_LOGE(TAG, "Characteristic #%u %s (%s) value = %s (0x%" PRIx8 "), unexpected value.",
                                            param_descriptor->cid,
                                            param_descriptor->param_key,
                                            param_descriptor->param_units,
                                            (const char*)rw_str,
                                            *(uint8_t*)temp_data_ptr);
                            alarm_state = true;
                            break;
                        }
                        if (state & param_descriptor->param_opts.opt1) {
                            alarm_state = true;
                            break;
                        }
                    }
                } else {
                    ESP_LOGE(TAG, "Characteristic #%u (%s) read fail, err = 0x%x (%s).",
                                    param_descriptor->cid,
                                    param_descriptor->param_key,
                                    (int)err,
                                    (char*)esp_err_to_name(err));
                }
                vTaskDelay(POLL_TIMEOUT_TICS); // timeout between polls
            }
            
        }
        
        vTaskDelay(UPDATE_CIDS_TIMEOUT_TICS);
    }

    if (alarm_state) {
        ESP_LOGI(TAG, "Alarm triggered by cid #%u.", param_descriptor->cid);
    } else {
        ESP_LOGE(TAG, "Alarm is not triggered after %u retries.", MASTER_MAX_RETRY);
    }
    ESP_LOGI(TAG, "Destroy master...");
    ESP_ERROR_CHECK(mbc_master_destroy());
}

// Modbus master initialization
esp_err_t master_init(void)
{
    // Initialize and start Modbus controller
    mb_communication_info_t comm = {
            .port = MB_PORT_NUM,
#if CONFIG_MB_COMM_MODE_ASCII
            .mode = MB_MODE_ASCII,
#elif CONFIG_MB_COMM_MODE_RTU
            .mode = MB_MODE_RTU,
#endif
            .baudrate = MB_DEV_SPEED,
            .parity = MB_PARITY_NONE
    };
    void* master_handler = NULL;

    esp_err_t err = mbc_master_init(MB_PORT_SERIAL_MASTER, &master_handler);
    MB_RETURN_ON_FALSE((master_handler != NULL), ESP_ERR_INVALID_STATE, TAG,
                                "mb controller initialization fail.");
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                            "mb controller initialization fail, returns(0x%x).", (int)err);
    err = mbc_master_setup((void*)&comm);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                            "mb controller setup fail, returns(0x%x).", (int)err);

    // Set UART pin numbers
    err = uart_set_pin(MB_PORT_NUM, CONFIG_MB_UART_TXD, CONFIG_MB_UART_RXD,
                              CONFIG_MB_UART_RTS, UART_PIN_NO_CHANGE);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
        "mb serial set pin failure, uart_set_pin() returned (0x%x).", (int)err);

    err = mbc_master_start();
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                            "mb controller start fail, returned (0x%x).", (int)err);

    // Set driver mode to Half Duplex
    err = uart_set_mode(MB_PORT_NUM, UART_MODE_RS485_HALF_DUPLEX);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
            "mb serial set mode failure, uart_set_mode() returned (0x%x).", (int)err);

    vTaskDelay(5);
    err = mbc_master_set_descriptor(&device_parameters[0], num_device_parameters);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                                "mb controller set descriptor fail, returns(0x%x).", (int)err);
    ESP_LOGI(TAG, "Modbus master stack initialized...");
    return err;
}

