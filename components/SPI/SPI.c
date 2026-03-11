/**
 * @file SPI.c
 * @date 04/27/2025
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include "SPI.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/spi_master.h>
#include "driver/gpio.h"
#include "freertos/semphr.h"
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

/* The configuration structure of the device that we will communicate with via SPI */
typedef struct
{
    spi_device_interface_config_t spi_cfg;
    spi_device_handle_t spi_dev;
} prj_spi_device_t;

static prj_spi_device_t m_spi_device;
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_spi_init (void)
{
    esp_err_t err;
    
    // Configuration for SPI bus
    spi_bus_config_t buscfg = {
        .miso_io_num = CONFIG_SPI_MISO_GPIO,
        .mosi_io_num = CONFIG_SPI_MOSI_GPIO,
        .sclk_io_num = CONFIG_SPI_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    // Initialize SPI bus
    err = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);
    if (err != ESP_OK) {
        return PRJ_ERROR_INTERNAL;
    }

    // Configuration for SPI device
    m_spi_device.spi_cfg = (spi_device_interface_config_t) {
        .clock_speed_hz = CONFIG_SPI_CLOCK_SPEED_HZ,
        .mode = CONFIG_SPI_MODE,
        .spics_io_num = CONFIG_SPI_CS_GPIO,
        .queue_size = 7,
    };

    // Add SPI device
    err = spi_bus_add_device(SPI2_HOST, &m_spi_device.spi_cfg, &m_spi_device.spi_dev);
    if (err != ESP_OK) {
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