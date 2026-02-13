/**
 * @file wifi_sta.c
 * @date 05/16/2025
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include "wifi_sta.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "time_sync.h"
/***************************************************************************************************
 * Definitions
 **************************************************************************************************/
#define PRJ_WIFI_STA_TOUT          (5000U)
#define PRJ_WIFI_STA_BIT_CONNECTED (BIT0)
#define PRJ_WIFI_STA_BIT_FAIL      (BIT1)

#define PRJ_WIFI_STA_SSID          (CONFIG_WIFI_STA_SSID)
#define PRJ_WIFI_STA_PASSWORD      (CONFIG_WIFI_STA_PASSWORD)
#define PRJ_WIFI_STA_MAXIMUM_RETRY (CONFIG_WIFI_STA_MAXIMUM_RETRY)

#if CONFIG_WIFI_STA_WPA3_SAE_PWE_HUNT_AND_PECK
#define PRJ_WIFI_STA_SAE_MODE       (WPA3_SAE_PWE_HUNT_AND_PECK)
#define PRJ_WIFI_STA_H2E_IDENTIFIER ""
#elif CONFIG_WIFI_STA_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define PRJ_WIFI_STA_SAE_MODE       (WPA3_SAE_PWE_HASH_TO_ELEMENT)
#define PRJ_WIFI_STA_H2E_IDENTIFIER (CONFIG_WIFI_STA_PASSWORD_ID)
#elif CONFIG_WIFI_STA_WPA3_SAE_PWE_BOTH
#define PRJ_WIFI_STA_SAE_MODE       (WPA3_SAE_PWE_BOTH)
#define PRJ_WIFI_STA_H2E_IDENTIFIER (CONFIG_WIFI_STA_PASSWORD_ID)
#endif

#if CONFIG_WIFI_STA_AUTH_OPEN
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_OPEN)
#elif CONFIG_WIFI_STA_AUTH_WEP
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WEP)
#elif CONFIG_WIFI_STA_AUTH_WPA_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WPA_PSK)
#elif CONFIG_WIFI_STA_AUTH_WPA2_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WPA2_PSK)
#elif CONFIG_WIFI_STA_AUTH_WPA_WPA2_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WPA_WPA2_PSK)
#elif CONFIG_WIFI_STA_AUTH_WPA3_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WPA3_PSK)
#elif CONFIG_WIFI_STA_AUTH_WPA2_WPA3_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WPA2_WPA3_PSK)
#elif CONFIG_WIFI_STA_AUTH_WAPI_PSK
#define PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD (WIFI_AUTH_WAPI_PSK)
#endif
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
/***************************************************************************************************
 * Static functions declaration
 **************************************************************************************************/
static void wifi_sta_event_handler (void *const p_arg, const esp_event_base_t event_base, const prj_i32_t event_id, void *const p_event_data);
/***************************************************************************************************
 * Variables
 **************************************************************************************************/
static EventGroupHandle_t m_wifi_sta_event_group = NULL;
static prj_u8_t m_retry_count = 0U;
/***************************************************************************************************
 * API
 **************************************************************************************************/
void prj_wifi_sta_init (void)
{
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_event_handler_instance_t instance_any_id = NULL;
    esp_event_handler_instance_t instance_got_ip = NULL;
    EventBits_t bits = 0U;

    wifi_config_t wifi_config = {
        .sta = {
            .ssid               = PRJ_WIFI_STA_SSID,
            .password           = PRJ_WIFI_STA_PASSWORD,
            .threshold.authmode = PRJ_WIFI_STA_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e        = PRJ_WIFI_STA_SAE_MODE,
            .sae_h2e_identifier = PRJ_WIFI_STA_H2E_IDENTIFIER,
        },
    };

    m_wifi_sta_event_group = xEventGroupCreate ();

    if (m_wifi_sta_event_group == NULL)
    {
        ESP_LOGE (PRJ_WIFI_STA_TAG, "wifi sta init: failed to create event group");
        return;
    }

    ESP_LOGI (PRJ_WIFI_STA_TAG, "wifi sta init: initializing wifi sta...");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_sta_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_sta_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI (PRJ_WIFI_STA_TAG, "wifi sta init: wifi sta init finished");

    bits = xEventGroupWaitBits(m_wifi_sta_event_group,
                                PRJ_WIFI_STA_BIT_CONNECTED | PRJ_WIFI_STA_BIT_FAIL,
                                pdFALSE,
                                pdFALSE,
                                PRJ_WIFI_STA_TOUT);

    if (bits & PRJ_WIFI_STA_BIT_CONNECTED) 
    {
        ESP_LOGI(PRJ_WIFI_STA_TAG, "wifi sta init: connected to ap ssid:%s password:%s", PRJ_WIFI_STA_SSID, PRJ_WIFI_STA_PASSWORD);
        prj_time_sync_once();
    } 
    else if (bits & PRJ_WIFI_STA_BIT_FAIL) 
    {
        ESP_LOGI(PRJ_WIFI_STA_TAG, "wifi sta init: failed to connect to ssid:%s, password:%s", PRJ_WIFI_STA_SSID, PRJ_WIFI_STA_PASSWORD);
    } 
    else 
    {
        ESP_LOGE(PRJ_WIFI_STA_TAG, "wifi sta init: unexpected event");
    }

    return;
}
/***************************************************************************************************
 * STATIC
 **************************************************************************************************/
static void wifi_sta_event_handler (void *const p_arg, const esp_event_base_t event_base, const prj_i32_t event_id, void *const p_event_data)
{
    ip_event_got_ip_t* event = NULL;

    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_START)) 
    {
        esp_wifi_connect();
    } 
    else if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_DISCONNECTED)) 
    {
        if (m_retry_count < PRJ_WIFI_STA_MAXIMUM_RETRY) 
        {
            esp_wifi_connect();
            m_retry_count++;
            ESP_LOGI(PRJ_WIFI_STA_TAG, "wifi sta event handler: retry to connect to the AP");
        } 
        else 
        {
            xEventGroupSetBits(m_wifi_sta_event_group, PRJ_WIFI_STA_BIT_FAIL);
        }

        ESP_LOGI(PRJ_WIFI_STA_TAG, "wifi sta event handler: connect to the AP fail");
    } 
    else if ((event_base == IP_EVENT) && (event_id == IP_EVENT_STA_GOT_IP))
    {
        event = (ip_event_got_ip_t*) p_event_data;
        ESP_LOGI(PRJ_WIFI_STA_TAG, "wifi sta event handler: got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        m_retry_count = 0;
        xEventGroupSetBits(m_wifi_sta_event_group, PRJ_WIFI_STA_BIT_CONNECTED);
    }

    return;
}
/***************************************************************************************************
 * EOF
 **************************************************************************************************/
