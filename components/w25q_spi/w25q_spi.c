/**
 * @file w25q_spi.c
 * @date 11/03/2026
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/spi_master.h>
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "w25q_spi.h"

/***************************************************************************************************
 * Definitions
 **************************************************************************************************/

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
static const char *TAG = "w25q_spi";
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_w25q_spi_init (prj_spi_device_t *m_spi_device, spi_host_device_t host, gpio_num_t cs_pin)
{
    esp_err_t err;

    gpio_reset_pin( CONFIG_PIN_NUM_CS );
    gpio_set_direction( CONFIG_PIN_NUM_CS, GPIO_MODE_OUTPUT );
    gpio_set_level( CONFIG_PIN_NUM_CS, 0 );

    // Configuration for SPI device
    m_spi_device->spi_cfg = (spi_device_interface_config_t) 
    {
        .clock_speed_hz = CONFIG_SPI_CLOCK_SPEED_HZ,
        .mode = CONFIG_SPI_MODE,
        .spics_io_num = cs_pin,
        .queue_size = 7,
    };

    // Add SPI device
    err = spi_bus_add_device(host, &m_spi_device->spi_cfg, &m_spi_device->spi_dev);
    if (err != ESP_OK) 
    {
        return PRJ_ERROR_INTERNAL;
    }

    return PRJ_SUCCESS;
}
/***************************************************************************************************
 * STATIC
 **************************************************************************************************/

/***************************************************************************************************
 * EOF
 **************************************************************************************************/