//
// Created by xl on 2024/6/3.
//
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
#include "my_mqtt_client.h"

#define CONFIG_BROKER_URL "mqtt://192.168.1.114:1883"

static const char *TAG = "MQTT5_EXAMPLE";
esp_mqtt_client_handle_t client_esp32;
uint32_t rgb24bit;
extern void switch_Image();

// 函数来获取指定长度的子字符串，从第一个字符开始
char* get_substring(const char* str, int length) {
    // 检查输入的有效性
    if (str == NULL || length <= 0) {
        return NULL;
    }

    int actual_length = strlen(str);
    if (length > actual_length) {
        length = actual_length;
    }

    // 分配内存给子字符串
    char* substring = (char*)malloc(sizeof(char) * (length + 1));
    if (substring == NULL) {
        return NULL;
    }

    // 复制指定长度的子字符串
    strncpy(substring, str, length);
    substring[length] = '\0'; // 确保子字符串以 null 结尾

    return substring;
}

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static esp_mqtt5_user_property_item_t user_property_arr[] = {
        {"board", "esp32"},
        {"u", "user"},
        {"p", "password"}
};

#define USE_PROPERTY_ARR_SIZE (sizeof(user_property_arr) / sizeof(esp_mqtt5_user_property_item_t))

static esp_mqtt5_publish_property_config_t publish_property = {
        .payload_format_indicator = 1,
        .message_expiry_interval = 1000,
        .response_topic = "/topic/test/response",
        .correlation_data = "123456",
        .correlation_data_len = 6,
};

static esp_mqtt5_subscribe_property_config_t subscribe_property = {
        .subscribe_id = 25555,
        .is_share_subscribe = true,
        .share_name = "group1",
};

static esp_mqtt5_disconnect_property_config_t disconnect_property = {
        .session_expiry_interval = 60,
};

static void print_user_property(mqtt5_user_property_handle_t user_property)
{
    if (user_property) {
        uint8_t count = esp_mqtt5_client_get_user_property_count(user_property);
        if (count) {
            esp_mqtt5_user_property_item_t *item = malloc(count * sizeof(esp_mqtt5_user_property_item_t));
            if (esp_mqtt5_client_get_user_property(user_property, item, &count) == ESP_OK) {
                for (int i = 0; i < count; i++) {
                    ESP_LOGI(TAG, "key is %s, value is %s", item[i].key, item[i].value);
                }
            }
            free(item);
        }
    }
}

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    ESP_LOGD(TAG, "free heap size is %" PRIu32 ", minimum %" PRIu32, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            print_user_property(event->property->user_property);

            esp_mqtt5_client_set_user_property(&publish_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
            esp_mqtt5_client_set_publish_property(client, &publish_property);
            msg_id = esp_mqtt_client_publish(client, "/time/style", "image1", 0, 1, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            esp_mqtt5_client_set_user_property(&subscribe_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
            esp_mqtt5_client_set_subscribe_property(client, &subscribe_property);
            msg_id = esp_mqtt_client_subscribe(client, "/LED/rgb", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/time/style", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            print_user_property(event->property->user_property);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//            print_user_property(event->property->user_property);
//            esp_mqtt5_client_set_publish_property(client, &publish_property);
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
//            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            print_user_property(event->property->user_property);
            esp_mqtt5_client_set_user_property(&disconnect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
            esp_mqtt5_client_set_disconnect_property(client, &disconnect_property);
            esp_mqtt_client_disconnect(client);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            print_user_property(event->property->user_property);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            print_user_property(event->property->user_property);
            ESP_LOGI(TAG, "payload_format_indicator is %d", event->property->payload_format_indicator);
            ESP_LOGI(TAG, "response_topic is %.*s", event->property->response_topic_len, event->property->response_topic);
            ESP_LOGI(TAG, "correlation_data is %.*s", event->property->correlation_data_len, event->property->correlation_data);
            ESP_LOGI(TAG, "content_type is %.*s", event->property->content_type_len, event->property->content_type);
            ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);
            char *data = get_substring(event->data,event->data_len);
            printf("eventdata:%s\n",data);
            if(strcmp(data,"switch") == 0){
                printf("mqttlog:ok\n");
                switch_Image();
            }
            if(strcmp(event->topic,"/LED/rgb") == 0){
                rgb24bit = strtol(data, NULL, 10);
            }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            print_user_property(event->property->user_property);
            ESP_LOGI(TAG, "MQTT5 return code is %d", event->error_handle->connect_return_code);
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

void mqtt5_app_start(void)
{
    esp_mqtt5_connection_property_config_t connect_property = {
            .session_expiry_interval = 10,
            .maximum_packet_size = INT32_MAX,
            .receive_maximum = 65535,
            .topic_alias_maximum = 2,
            .request_resp_info = true,
            .request_problem_info = true,
            .will_delay_interval = 10,
            .payload_format_indicator = true,
            .message_expiry_interval = 10,
            .response_topic = "/test/response",
            .correlation_data = "123456",
            .correlation_data_len = 6,
    };

    esp_mqtt_client_config_t mqtt5_cfg = {
            .broker.address.uri = CONFIG_BROKER_URL,
            .session.protocol_ver = MQTT_PROTOCOL_V_5,
            .credentials.username = "123",
            .credentials.authentication.password = "456",
            .session.last_will.topic = "/topic/will",
            .session.last_will.msg = "i will leave",
            .session.last_will.msg_len = 12,
            .session.last_will.qos = 1,
            .session.last_will.retain = true,
    };

    client_esp32 = esp_mqtt_client_init(&mqtt5_cfg);

    esp_mqtt5_client_set_user_property(&connect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_user_property(&connect_property.will_user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_connect_property(client_esp32, &connect_property);
    esp_mqtt5_client_delete_user_property(connect_property.user_property);
    esp_mqtt5_client_delete_user_property(connect_property.will_user_property);

    esp_mqtt_client_register_event(client_esp32, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client_esp32);
}