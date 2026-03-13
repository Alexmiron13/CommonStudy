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
        ESP_LOGI(TAG, "spi_bus_add_device_failed=%d",err);
        return PRJ_ERROR_INTERNAL;
    }
    ESP_LOGI(TAG, "spi_bus_add_device=%d",err);
    return PRJ_SUCCESS;
}

void W25_Reset (prj_spi_device_t *m_spi_device)
{
  spi_transaction_t SPITransaction;
  uint8_t data[2];
  data[0] = W25_ENABLE_RESET;
  data[1] = W25_RESET;
  memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
  SPITransaction.length = 2 * 8;
  SPITransaction.tx_buffer = data;
  SPITransaction.rx_buffer = data;
  spi_device_transmit(m_spi_device->spi_dev, &SPITransaction );
}


uint32_t W25_Read_ID(prj_spi_device_t *m_spi_device)
{
  spi_transaction_t SPITransaction;
  uint8_t data[4];
  data[0] = W25_GET_JEDEC_ID;
  memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
  SPITransaction.length = 4 * 8;
  SPITransaction.tx_buffer = data;
  SPITransaction.rx_buffer = data;
  spi_device_transmit(m_spi_device->spi_dev, &SPITransaction );
  return (data[1] << 16) | (data[2] << 8) | data[3];
}

/***************************************************************************************************
 * STATIC
 **************************************************************************************************/

/***************************************************************************************************
 * EOF
 **************************************************************************************************/