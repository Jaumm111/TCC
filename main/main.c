#include "string.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "include/map_sunspec.h"
#include "include/master.h"
#include "include/mqtt_app.h"



static const char *TAG = "MAIN";

void app_main(void)
{

    // Initialization of device peripheral and objects
    ESP_ERROR_CHECK(master_init());
    vTaskDelay(10);
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = true,
        .credentials.username = "kIOG6fLiCCH46HruOaJP",
        .credentials.authentication.password = "llll",
        .session.last_will.topic = TOPIC_ATTRIBUTES,
        .session.last_will.msg = "{\"Conexao\":0}",
        .session.last_will.msg_len = 12,
        .session.last_will.qos = 1,
        .session.last_will.retain = true,
    };
    esp_mqtt_client_handle_t *client = get_client();
    if (client == NULL){
        client = (esp_mqtt_client_handle_t *)malloc(sizeof(esp_mqtt_client_handle_t)+1);
        *client = esp_mqtt_client_init(&mqtt5_cfg);
        mqtt5_app_start(client);
    }

    master_operation_func(NULL,client);
}