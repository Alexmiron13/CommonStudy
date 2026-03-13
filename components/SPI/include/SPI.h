/**
 * @file spi.h
 * @date 11/03/2026
 * @copyright © GmbH, 2026.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Home GmbH.
 */

#ifndef SPI_H
#define SPI_H
/***************************************************************************************************
 * Includes
 *************************************************************************************************/
#include "prj_common.h"
#include <driver/spi_master.h>
/***************************************************************************************************
 * Definitions
 **************************************************************************************************/
#define PRJ_SPI_TAG "SPI"

/* Default SPI configuration values */
#ifndef CONFIG_SPI_MISO_GPIO
#define CONFIG_SPI_MISO_GPIO    48
#endif

#ifndef CONFIG_SPI_MOSI_GPIO
#define CONFIG_SPI_MOSI_GPIO    20
#endif

#ifndef CONFIG_SPI_SCLK_GPIO
#define CONFIG_SPI_SCLK_GPIO    21
#endif

#ifndef CONFIG_SPI_CS_GPIO
#define CONFIG_SPI_CS_GPIO      47
#endif

#ifndef CONFIG_SPI_CLOCK_SPEED_HZ
#define CONFIG_SPI_CLOCK_SPEED_HZ   1000000
#endif

#ifndef CONFIG_SPI_MODE
#define CONFIG_SPI_MODE   0
#endif

#define    W25_ENABLE_RESET  0x66
#define    W25_RESET  0x99
#define    W25_READ  0x03
#define    W25_GET_JEDEC_ID  0x9f
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
/* The configuration structure of the device that we will communicate with via SPI */
typedef struct
{
    spi_device_interface_config_t spi_cfg;
    spi_device_handle_t spi_dev;
} prj_spi_device_t;
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_spi_init (spi_host_device_t host);
void prj_send_spi (prj_spi_device_t *m_spi_device, uint16_t *buf, uint8_t sz);
#endif /* SPI_H */
/***************************************************************************************************
 * EOF
 **************************************************************************************************/