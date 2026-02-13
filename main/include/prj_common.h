/**
 * @file prj_common.h
 * @date 04/27/2025
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

#ifndef PRJ_COMMON_H
#define PRJ_COMMON_H
/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"
#include "esp_err.h"
#include "sdkconfig.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRJ_SUCCESS                  	(0x00000000U) /*!< Successfull operation */
#define PRJ_ERROR_INTERNAL           	(0x00000001U) /*!< Internal Error */
#define PRJ_ERROR_NOT_FOUND          	(0x00000004U) /*!< Not found */
#define PRJ_ERROR_NOT_SUPPORTED      	(0x00000008U) /*!< Not supported */
#define PRJ_ERROR_INVALID_PARAM      	(0x00000010U) /*!< Invalid Parameter */
#define PRJ_ERROR_INVALID_STATE      	(0x00000020U) /*!< Invalid state, operation disallowed in this state */
#define PRJ_ERROR_INVALID_LENGTH     	(0x00000040U) /*!< Invalid Length */
#define PRJ_ERROR_INVALID_DATA       	(0x00000100U) /*!< Invalid Data */
#define PRJ_ERROR_DATA_SIZE          	(0x00000200U) /*!< Invalid Data size */
#define PRJ_ERROR_TIMEOUT            	(0x00000400U) /*!< Operation timed out */
#define PRJ_ERROR_NULL               	(0x00000800U) /*!< Null Pointer */
#define PRJ_ERROR_FORBIDDEN          	(0x00001000U) /*!< Forbidden Operation */
#define PRJ_ERROR_INVALID_ADDR       	(0x00002000U) /*!< Bad Memory Address */
#define PRJ_ERROR_BUSY               	(0x00004000U) /*!< Busy */
#define PRJ_ERROR_RESOURCES          	(0x00008000U) /*!< Not enough resources for operation */
#define PRJ_ERROR_INCONSISTENT_STORAGE  (0x00010000U) /*!< Data (in Flash or EEPROM) is corrupted */
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
typedef uint32_t prj_status_t;

typedef size_t 	 prj_size_t;
typedef uint64_t prj_u64_t;
typedef uint32_t prj_u32_t;
typedef uint16_t prj_u16_t;
typedef uint8_t  prj_u8_t;
typedef int32_t  prj_i32_t;
typedef int16_t  prj_i16_t;
typedef int8_t 	 prj_i8_t;
typedef float 	 prj_float_t;
typedef double 	 prj_double_t;
typedef char 	 prj_char_t;
typedef bool 	 prj_bool_t;
/***************************************************************************************************
 * API
 **************************************************************************************************/
#endif /* PRJ_COMMON_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
