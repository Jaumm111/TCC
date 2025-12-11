#include "include/map_sunspec.h"
#include "string.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "esp_log.h"

static const char TAG[] __attribute__((unused)) = "MAP";

static char *bufferJSON = NULL; 
static uint32_t bufferJSONCurrentSize = 0;
#define BUFFER_STEP (1024)

const table_sunspec table_1[9] = {
{.type = SUNSPEC_TYPE_UINT32,.label = "SID",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_UINT16,.label = "ID",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "L",.scale = "",.units = "Registers",.size = 1},{.type = SUNSPEC_TYPE_STRING16,.label = "Mn",.scale = "",.units = "",.size = 16},{.type = SUNSPEC_TYPE_STRING16,.label = "Md",.scale = "",.units = "",.size = 16},{.type = SUNSPEC_TYPE_STRING8,.label = "Opt",.scale = "",.units = "",.size = 8},{.type = SUNSPEC_TYPE_STRING8,.label = "Vr",.scale = "",.units = "",.size = 8},{.type = SUNSPEC_TYPE_STRING16,.label = "SN",.scale = "",.units = "",.size = 16},{.type = SUNSPEC_TYPE_UINT16,.label = "DA",.scale = "",.units = "",.size = 1},};

const table_sunspec table_101[45] = {
{.type = SUNSPEC_TYPE_UINT16,.label = "ID",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "L",.scale = "",.units = "Registers",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "A",.scale = "A_SF",.units = "A",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "AphA",.scale = "A_SF",.units = "A",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "AphB",.scale = "A_SF",.units = "A",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "AphC",.scale = "A_SF",.units = "A",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "A_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PPVphAB",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PPVphBC",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PPVphCA",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PhVphA",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PhVphB",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "PhVphC",.scale = "V_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "V_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "W",.scale = "W_SF",.units = "W",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "W_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "Hz",.scale = "Hz_SF",.units = "Hz",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "Hz_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "VA",.scale = "VA_SF",.units = "VA",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "VA_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "VAr",.scale = "VAr_SF",.units = "var",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "VAr_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "PF",.scale = "PF_SF",.units = "Pct",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "PF_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_ACC32,.label = "WH",.scale = "WH_SF",.units = "Wh",.size = 2},{.type = SUNSPEC_TYPE_SUNSSF,.label = "WH_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "DCA",.scale = "DCA_SF",.units = "A",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "DCA_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "DCV",.scale = "DCV_SF",.units = "V",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "DCV_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "DCW",.scale = "DCW_SF",.units = "W",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "DCW_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "TmpCab",.scale = "Tmp_SF",.units = "C",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "TmpSnk",.scale = "Tmp_SF",.units = "C",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "TmpTrns",.scale = "Tmp_SF",.units = "C",.size = 1},{.type = SUNSPEC_TYPE_INT16,.label = "TmpOt",.scale = "Tmp_SF",.units = "C",.size = 1},{.type = SUNSPEC_TYPE_SUNSSF,.label = "Tmp_SF",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_ENUM16,.label = "St",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_ENUM16,.label = "StVnd",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "Evt1",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "Evt2",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd1",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd2",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd3",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd4",.scale = "",.units = "",.size = 2},};

const table_sunspec table_111[33] = {
{.type = SUNSPEC_TYPE_UINT16,.label = "ID",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_UINT16,.label = "L",.scale = "",.units = "Registers",.size = 1},{.type = SUNSPEC_TYPE_FLOAT32,.label = "A",.scale = "",.units = "A",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "AphA",.scale = "",.units = "A",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "AphB",.scale = "",.units = "A",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "AphC",.scale = "",.units = "A",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PPVphAB",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PPVphBC",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PPVphCA",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PhVphA",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PhVphB",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PhVphC",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "W",.scale = "",.units = "W",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "Hz",.scale = "",.units = "Hz",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "VA",.scale = "",.units = "VA",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "VAr",.scale = "",.units = "var",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "PF",.scale = "",.units = "Pct",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "WH",.scale = "",.units = "Wh",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "DCA",.scale = "",.units = "A",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "DCV",.scale = "",.units = "V",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "DCW",.scale = "",.units = "W",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "TmpCab",.scale = "",.units = "C",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "TmpSnk",.scale = "",.units = "C",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "TmpTrns",.scale = "",.units = "C",.size = 2},{.type = SUNSPEC_TYPE_FLOAT32,.label = "TmpOt",.scale = "",.units = "C",.size = 2},{.type = SUNSPEC_TYPE_ENUM16,.label = "St",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_ENUM16,.label = "StVnd",.scale = "",.units = "",.size = 1},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "Evt1",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "Evt2",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd1",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd2",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd3",.scale = "",.units = "",.size = 2},{.type = SUNSPEC_TYPE_BITFIELD32,.label = "EvtVnd4",.scale = "",.units = "",.size = 2},};

#define NUM_MODELS 3
const map_inverter_full models[NUM_MODELS] = {{.table = table_1,.count = 9,.num_reg = 68},
{.table = table_101,.count = 45,.num_reg = 51},
{.table = table_111,.count = 33,.num_reg = 61},
};


static uint16_t decide(char *string,void *data, table_sunspec table,uint16_t tam,uint16_t *num_char){
    
    switch (table.type)
    {
    case SUNSPEC_TYPE_UINT16:
        *num_char = snprintf(string, tam,"\"%s\" : %hu",table.label, *(uint16_t*)data);
        break;
    case SUNSPEC_TYPE_SUNSSF:
    
    case SUNSPEC_TYPE_INT16:
        *num_char = snprintf(string, tam,"\"%s\" : %hi",table.label, *(int16_t*)data);
        //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": %hi, \"scale\": \"%s\", \"units\": \"%s\"}",
        //    table.label,
        //    *(int16_t*)data,
            //table.description,
        //    table.scale,
        //    table.units);
        break;
    case SUNSPEC_TYPE_UINT32:
        *num_char = snprintf(string, tam,"\"%s\" : %lu",table.label, *(uint32_t*)data);
        //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": %lu, \"scale\": \"%s\", \"units\": \"%s\"}",
        //    table.label,
        //    *(uint32_t*)data,
            //table.description,
        //    table.scale,
        //    table.units);
        break;
    case SUNSPEC_TYPE_FLOAT32:
        *num_char = snprintf(string, tam,"\"%s\" : %.4g",table.label, *(float *)data);
        //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": %.4g, \"scale\": \"%s\", \"units\": \"%s\"}",
        //    table.label,
        //    *(float*)data,
            //table.description,
        //    table.scale,
        //    table.units);
        break;
    case SUNSPEC_TYPE_STRING8:
        *num_char = snprintf(string, tam,"\"%s\" : \"%.8s\"",table.label, (char*)data);
        //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": \"%.8s\", \"description\": \"\", \"scale\": \"%s\", \"units\": \"%s\"}",
        //    table.label,
        //    (char*)data,
            //table.description,
        //    table.scale,
        //    table.units);
        break;
    case SUNSPEC_TYPE_STRING16:
        *num_char = snprintf(string, tam,"\"%s\" : \"%.16s\"",table.label, (char*)data);
        //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": \"%.16s\", \"description\": \"\", \"scale\": \"%s\", \"units\": \"%s\"}",
        //    table.label,
        //    (char*)data,
            //table.description,
        //    table.scale,
        //    table.units);
        break;
    
    default:
        if (table.type < SUNSPEC_DIVISOR_1REG)
            *num_char = snprintf(string, tam,"\"%s\" : \"%04hX\"",table.label, *(uint16_t*)data);
            //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": \"%04hX\", \"description\": \"\", \"scale\": \"%s\", \"units\": \"%s\"}",
            //    table.label,
            //    *(uint16_t*)data,
                //table.description,
            //    table.scale,
            //    table.units);
        else if (table.type < SUNSPEC_DIVISOR_2REG)
            *num_char = snprintf(string, tam,"\"%s\" : \"%08lX\"",table.label, *(uint32_t*)data);
            //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": \"%08lX\", \"description\": \"\", \"scale\": \"%s\", \"units\": \"%s\"}",
            //    table.label,
            //    *(uint32_t*)data,
                //table.description,
            //    table.scale,
            //    table.units);
        else if (table.type < SUNSPEC_DIVISOR_4REG)
            *num_char = snprintf(string, tam,"\"%s\" : \"%016llX\"",table.label, *(uint64_t*)data);
            //*num_char = snprintf(string, tam,"{\"label\": %s, \"v\": \"%016llX\", \"description\": \"\", \"scale\": \"%s\", \"units\": \"%s\"}",
            //    table.label,
            //    *(uint64_t*)data,
                //table.description,
            //    table.scale,
            //    table.units);
        break;
    }
    if (table.type < SUNSPEC_DIVISOR_1REG){
        return 2;
    }else if (table.type < SUNSPEC_DIVISOR_2REG){
        return 4;
    }else if (table.type < SUNSPEC_DIVISOR_4REG){
        return 8;
    }else if (table.type < SUNSPEC_DIVISOR_8REG){
        return 16;
    }else{
        return 32;
    }
    
}

static void realloc_JSON(char *var,uint16_t x){
    ESP_LOGI(TAG,"Begging %i",x);
    var = (char*)realloc(var,(x)*sizeof(char)+ 1);
    
    assert(var != NULL);

    ESP_LOGI(TAG,"ENDING ADDR: %08lX",var);
}

static void insert_JSON(char *var){
    uint16_t x = snprintf(NULL, 0,"%s",var);
    if (x + strlen(bufferJSON) + 1 > bufferJSONCurrentSize){
        bufferJSONCurrentSize += BUFFER_STEP;
        realloc_JSON(bufferJSON,bufferJSONCurrentSize);
    }
    strncat(bufferJSON,var,x + strlen(bufferJSON) + 1);
}

static void init_JSON(){
    if (bufferJSON == NULL){
        bufferJSONCurrentSize += BUFFER_STEP;
        bufferJSON = malloc((bufferJSONCurrentSize)*sizeof(char)+ 1);
        ESP_LOGI(TAG,"%i",bufferJSONCurrentSize);
        assert(bufferJSON != NULL);
    }
    *bufferJSON = '\0';
}


char *convert_map_to_string(void *data, tabel_model_t model){
    ESP_LOGI(TAG,"convertendo");
    uint16_t t_str = 128;
    char *str_atual = (char*)malloc(t_str*sizeof(char)+1);
    *str_atual = '\0';
    void *ptr = data;
    uint16_t n_atual = 0;
    uint16_t n_total = 0;
    uint16_t tmp = 0;
    uint16_t ptr_tmp = 0;

    init_JSON();

    insert_JSON("{");
    if (model != SUNSPEC_TABEL_1){
        insert_JSON("\"table_1\":{");
        for ( uint16_t i = 0; i < models[SUNSPEC_TABEL_1].count; i++){
            ptr_tmp = decide(NULL,ptr, models[SUNSPEC_TABEL_1].table[i],0,&n_atual);
            n_total += n_atual;

            if (n_atual>t_str){
                t_str += 128;
                realloc_JSON(str_atual,t_str);
            }
            ptr_tmp = decide(str_atual,ptr, models[SUNSPEC_TABEL_1].table[i],n_atual+1,&tmp);

            insert_JSON(str_atual);

            if (i < models[SUNSPEC_TABEL_1].count - 1){
                insert_JSON(",\n");
            }
            ptr+= ptr_tmp;
        }

        insert_JSON("},");
    }
    switch (model)
    {
    case SUNSPEC_TABEL_1:
        insert_JSON("\"table_1\":{");
        break;
    case SUNSPEC_TABEL_101:
        insert_JSON("\"table_101\":{");
        break;
    case SUNSPEC_TABEL_111:
        insert_JSON("\"table_111\":{");
        break;
    
    default:
        break;
    }
    for ( uint16_t i = 0; i < models[model].count; i++){
        ptr_tmp = decide(NULL,ptr, models[model].table[i],0,&n_atual);
        n_total += n_atual;

        if (n_atual>t_str){
            t_str += 128;
            realloc_JSON(str_atual,t_str);
        }
        ptr_tmp = decide(str_atual,ptr, models[model].table[i],n_atual+1,&tmp);

        insert_JSON(str_atual);

        if (i < models[model].count - 1){
            insert_JSON(",\n");
        }
        ptr+= ptr_tmp;
    }

    insert_JSON("}}");

    free(str_atual);
    return bufferJSON;
}

