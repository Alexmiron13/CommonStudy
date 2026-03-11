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
/***************************************************************************************************
 * Definitions
 **************************************************************************************************/
#define PRJ_SPI_TAG "SPI"

/* Default SPI configuration values */
#ifndef CONFIG_SPI_MISO_GPIO
#define CONFIG_SPI_MISO_GPIO    12
#endif

#ifndef CONFIG_SPI_MOSI_GPIO
#define CONFIG_SPI_MOSI_GPIO    13
#endif

#ifndef CONFIG_SPI_SCLK_GPIO
#define CONFIG_SPI_SCLK_GPIO    14
#endif

#ifndef CONFIG_SPI_CS_GPIO
#define CONFIG_SPI_CS_GPIO      15
#endif

#ifndef CONFIG_SPI_CLOCK_SPEED_HZ
#define CONFIG_SPI_CLOCK_SPEED_HZ   1000000
#endif

#ifndef CONFIG_SPI_MODE
#define CONFIG_SPI_MODE   0
#endif
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_spi_init (void);
#endif /* SPI_H */
/***************************************************************************************************
 * EOF
 **************************************************************************************************/