/**
 * @file time_sync.c
 * @date 05/13/2025
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include "lwip/ip_addr.h"
#include "esp_sntp.h"
#include "time_sync.h"

#include <sys/time.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/***************************************************************************************************
 * Definitions
 **************************************************************************************************/
#define PRJ_TIME_SYNC_RETRY_CONNECT_MAX (5U)
#define PRJ_TIME_SYNC_YEAR_THRESHOLD    (2025)
#define PRJ_TIME_SYNC_YEAR_OFFSET       (1900)
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
/***************************************************************************************************
 * Static functions declaration
 **************************************************************************************************/
/***************************************************************************************************
 * Variables
 **************************************************************************************************/
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_time_sync_once(void)
{
    prj_status_t status = PRJ_SUCCESS;
    time_t time_now = 0;
    struct tm time_info = {0};
    prj_u8_t retry_num = 0U;

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, CONFIG_SNTP_TIME_SERVER);
    esp_sntp_init();

    while((time_info.tm_year < (PRJ_TIME_SYNC_YEAR_THRESHOLD - PRJ_TIME_SYNC_YEAR_OFFSET)) && (++retry_num < PRJ_TIME_SYNC_RETRY_CONNECT_MAX)) 
    {
        ESP_LOGI(PRJ_TIME_SYNC_TAG, "time obtain: waiting for system time to be set... (%d/%d)", retry_num, PRJ_TIME_SYNC_RETRY_CONNECT_MAX);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&time_now);
        localtime_r(&time_now, &time_info);
    }

    if (time_info.tm_year >= (PRJ_TIME_SYNC_YEAR_THRESHOLD - PRJ_TIME_SYNC_YEAR_OFFSET)) 
    {
        ESP_LOGI(PRJ_TIME_SYNC_TAG, "time obtain: time synchronized: %s", asctime(&time_info));
        status =  PRJ_SUCCESS;
    }
    else
    {
        ESP_LOGW(PRJ_TIME_SYNC_TAG, "time obtain: time not synchronized");
        status =  PRJ_ERROR_TIMEOUT;
    }

    esp_sntp_stop();

    return status;
}
/***************************************************************************************************
 * STATIC
 **************************************************************************************************/
/***************************************************************************************************
 * EOF
 **************************************************************************************************/
