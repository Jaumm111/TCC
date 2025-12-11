set(PROJECT_NAME "modbus_master")

idf_component_register(SRCS "master.c" "map_sunspec.c" "main.c" "mqtt_app.c"
                    PRIV_REQUIRES mqtt nvs_flash esp_netif
                    INCLUDE_DIRS ".")
