#ifndef _MQTT_APP_CONTROL_
#define	_MQTT_APP_CONTROL_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"

#define TOPIC_TELEMETRY "v1/devices/me/telemetry"
#define TOPIC_ATTRIBUTES "v1/devices/me/attributes"

void mqtt5_app_start(esp_mqtt_client_handle_t *client);
esp_mqtt_client_handle_t *get_client();
#endif