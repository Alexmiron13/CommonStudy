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

static const char *TAG = "main";
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_spi_init (spi_host_device_t host)
{
    esp_err_t err; 
    // Configuration for SPI bus
    spi_bus_config_t buscfg = 
    {
        .miso_io_num = CONFIG_SPI_MISO_GPIO,
        .mosi_io_num = CONFIG_SPI_MOSI_GPIO,
        .sclk_io_num = CONFIG_SPI_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
        .flags = 0
    };

    // Initialize SPI bus
    err = spi_bus_initialize(host, &buscfg, SPI_DMA_CH_AUTO);
    ESP_LOGI(TAG, "spi bus initialize: %d", err);
    if (err != ESP_OK) 
    {
        return PRJ_ERROR_INTERNAL;
    }
    return PRJ_SUCCESS;
}

void prj_send_spi (prj_spi_device_t *m_spi_device, uint16_t *buf, uint8_t sz)
{
  spi_transaction_t one_transaction;
  memset(&one_transaction, 0, sizeof(one_transaction));
  one_transaction.length = 16;
  for(uint8_t i=0; i<sz; i++)
  {
    one_transaction.tx_buffer = buf + i;
    spi_device_transmit(m_spi_device->spi_dev, &one_transaction);
  }
}
/***************************************************************************************************
 * STATIC
 **************************************************************************************************/

/***************************************************************************************************
 * EOF
 **************************************************************************************************/