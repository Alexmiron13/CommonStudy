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
typedef struct
{
  uint16_t  PageSize;
  uint32_t  PageCount;
  uint32_t  SectorSize;
  uint32_t  SectorCount;
  uint32_t  BlockSize;
  uint32_t  BlockCount;
  uint32_t  NumKB;
  uint8_t   SR1;
  uint8_t   SR2;
  uint8_t   SR3;
}w25_info_t;
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_w25q_spi_init (prj_spi_device_t *m_spi_device, spi_host_device_t host, gpio_num_t cs_pin)
{
    esp_err_t err;

    gpio_reset_pin(CONFIG_PIN_NUM_CS);
    gpio_set_direction(CONFIG_PIN_NUM_CS, GPIO_MODE_OUTPUT);
    gpio_set_level(CONFIG_PIN_NUM_CS, 0);

    // Configuration for SPI device
    m_spi_device->spi_cfg = (spi_device_interface_config_t) 
    {
        .clock_speed_hz = CONFIG_SPI_CLOCK_SPEED_HZ,
        .mode = CONFIG_SPI_MODE,
        .spics_io_num = cs_pin,
        .queue_size = 7,
     // .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
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


//Она нужна для того, чтобы при вызове функции выполнения транзакции с модулем SPI мы могли передавать значение уровня ножки DC.
// void lcd_spi_pre_transfer_callback(spi_transaction_t *SPITransaction)
// {
//     int dc=(int)SPITransaction->user;
//     gpio_set_level(CONFIG_PIN_NUM_DC, dc);
// }

void W25_Reset (prj_spi_device_t *m_spi_device)
{
  spi_transaction_t SPITransaction;
  uint8_t data[2];
  data[0] = W25_ENABLE_RESET;
  data[1] = W25_RESET;
  memset( &SPITransaction, 0, sizeof(spi_transaction_t));
  SPITransaction.length = 2 * 8;
  SPITransaction.tx_buffer = data;
  SPITransaction.rx_buffer = data;
// SPITransaction.user = (void*)0;
  spi_device_transmit(m_spi_device->spi_dev, &SPITransaction);
}


uint32_t W25_Read_ID(prj_spi_device_t *m_spi_device)
{
  spi_transaction_t SPITransaction;
  w25_info_t w25_info;
  uint8_t data[4];
  data[0] = W25_GET_JEDEC_ID;
  memset( &SPITransaction, 0, sizeof(spi_transaction_t));
  SPITransaction.length = 4 * 8;
  SPITransaction.tx_buffer = data;
  SPITransaction.rx_buffer = data;
  spi_device_transmit(m_spi_device->spi_dev, &SPITransaction);
  uint32_t id = (data[1] << 16) | (data[2] << 8) | data[3];
  ESP_LOGI(TAG, "ID:0x%X",id);
 
  id &= 0x0000ffff;
  switch(id)
  {
    case 0x401A:
      w25_info.BlockCount=1024;
      printf("w25qxx Chip: w25q512\r\n");
      break;
    case 0x4019:
      w25_info.BlockCount=512;
      printf("w25qxx Chip: w25q256\r\n");
      break;
    case 0x4018:
      w25_info.BlockCount=256;
      printf("w25qxx Chip: w25q128\r\n");
      break;
    case 0x4017:
      w25_info.BlockCount=128;
      printf("w25qxx Chip: w25q64\r\n");
      break;
    case 0x4016:
      w25_info.BlockCount=64;
      printf("w25qxx Chip: w25q32\r\n");
      break;
    case 0x4015:
      w25_info.BlockCount=32;
      printf("w25qxx Chip: w25q16\r\n");
      break;
    case 0x4014:
      w25_info.BlockCount=16;
      printf("w25qxx Chip: w25q80\r\n");
      break;
    case 0x4013:
      w25_info.BlockCount=8;
      printf("w25qxx Chip: w25q40\r\n");
      break;
    case 0x4012:
      w25_info.BlockCount=4;
      printf("w25qxx Chip: w25q20\r\n");
      break;
    case 0x4011:
      w25_info.BlockCount=2;
      printf("w25qxx Chip: w25q10\r\n");
      break;
    default:
      printf("w25qxx Unknown ID\r\n");
      return id;
  }
  w25_info.PageSize=256;
  w25_info.SectorSize=0x1000;
  w25_info.SectorCount=w25_info.BlockCount*16;
  w25_info.PageCount=(w25_info.SectorCount*w25_info.SectorSize)/w25_info.PageSize;
  w25_info.BlockSize=w25_info.SectorSize*16;
  w25_info.NumKB=(w25_info.SectorCount*w25_info.SectorSize)/1024;
  printf("Page Size: %d Bytes\r\n",(unsigned int)w25_info.PageSize);
  printf("Page Count: %u\r\n",(unsigned int)w25_info.PageCount);
  printf("Sector Size: %u Bytes\r\n",(unsigned int)w25_info.SectorSize);
  printf("Sector Count: %u\r\n",(unsigned int)w25_info.SectorCount);
  printf("Block Size: %u Bytes\r\n",(unsigned int)w25_info.BlockSize);
  printf("Block Count: %u\r\n",(unsigned int)w25_info.BlockCount);
  printf("Capacity: %u KB\r\n",(unsigned int)w25_info.NumKB);
  return id;
}

/***************************************************************************************************
 * STATIC
 **************************************************************************************************/

/***************************************************************************************************
 * EOF
 **************************************************************************************************/