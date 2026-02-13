/**
 * @file uart.c
 * @date 04/27/2025
 * @copyright © Promwad GmbH, 2024-2025.
 *
 * @copyright Use of this source code is governed by the respective Software development
 * agreement/Master service agreement concluded with the Promwad GmbH.
 */

/***************************************************************************************************
 * Includes
 **************************************************************************************************/
#include "uart.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
/***************************************************************************************************
 * Definitions
 **************************************************************************************************/
#define PRJ_UART_BUF_SIZE        (1024U)

#define PRJ_UART_PORT_NUM        (CONFIG_UART_PORT_NUM)
#define PRJ_UART_BAUD_RATE       (CONFIG_UART_BAUD_RATE)
#define PRJ_UART_RXD_PIN         (CONFIG_UART_RXD_PIN)
#define PRJ_UART_TXD_PIN         (CONFIG_UART_TXD_PIN)
#define PRJ_UART_TASK_STACK_SIZE (CONFIG_UART_TASK_STACK_SIZE)
#define PRJ_UART_TASK_PRIORITY   (CONFIG_UART_TASK_PRIORITY)
#define PRJ_UART_QUEUE_SIZE      (CONFIG_UART_QUEUE_SIZE)
/***************************************************************************************************
 * Macros
 **************************************************************************************************/
/***************************************************************************************************
 * Types
 **************************************************************************************************/
/***************************************************************************************************
 * Static functions declaration
 **************************************************************************************************/
static void uart_event_task (void *p_arg);
/***************************************************************************************************
 * Variables
 **************************************************************************************************/
static SemaphoreHandle_t m_uart_mutex = NULL;
static QueueHandle_t m_uart_queue = NULL;
static TaskHandle_t m_uart_task = NULL;

static prj_bool_t m_uart_is_init = false;
/***************************************************************************************************
 * API
 **************************************************************************************************/
prj_status_t prj_uart_init (void)
{
	esp_err_t esp_error = ESP_OK;
	BaseType_t task_result = pdPASS;

	uart_config_t uart_config =
	{
		.baud_rate = PRJ_UART_BAUD_RATE,
		.data_bits = UART_DATA_8_BITS,
		.parity    = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};

	if (m_uart_is_init != true)
	{
		esp_error = uart_driver_install (PRJ_UART_PORT_NUM, PRJ_UART_BUF_SIZE * 2, PRJ_UART_BUF_SIZE * 2,
				PRJ_UART_QUEUE_SIZE, &m_uart_queue, 0);

		if (esp_error != ESP_OK)
		{
			ESP_LOGE (PRJ_UART_TAG, "uart init: failed to install UART driver");
			return PRJ_ERROR_INTERNAL;
		}

		esp_error = uart_param_config (PRJ_UART_PORT_NUM, &uart_config);

		if (esp_error != ESP_OK)
		{
			ESP_LOGE (PRJ_UART_TAG, "uart init: failed to configure UART parameters");
			return PRJ_ERROR_INTERNAL;
		}

		esp_error = uart_set_pin (PRJ_UART_PORT_NUM, PRJ_UART_TXD_PIN, PRJ_UART_RXD_PIN, UART_PIN_NO_CHANGE,
				UART_PIN_NO_CHANGE);

		if (esp_error != ESP_OK)
		{
			ESP_LOGE (PRJ_UART_TAG, "uart init: failed to set UART pins");
			return PRJ_ERROR_INTERNAL;
		}

		if (m_uart_mutex == NULL)
		{
			m_uart_mutex = xSemaphoreCreateMutex();

			if (m_uart_mutex == NULL)
			{
				ESP_LOGE (PRJ_UART_TAG, "uart init: failed to create uart mutex");
				return PRJ_ERROR_INTERNAL;
			}
		}

		if (m_uart_task == NULL)
		{
			task_result = xTaskCreate (uart_event_task, "uart_event_task", PRJ_UART_TASK_STACK_SIZE, NULL,
					PRJ_UART_TASK_PRIORITY, &m_uart_task);

			if (task_result != pdPASS)
			{
				ESP_LOGE (PRJ_UART_TAG, "uart init: failed to create uart event task");
				return PRJ_ERROR_INTERNAL;
			}
		}

		m_uart_is_init = true;
	}
	else
	{
		ESP_LOGW (PRJ_UART_TAG, "uart init: uart already initialized");
	}

	return PRJ_SUCCESS;
}
/***************************************************************************************************
 * STATIC
 **************************************************************************************************/
static void uart_event_task (void *p_arg)
{
	uart_event_t event;
	prj_u8_t data[PRJ_UART_BUF_SIZE] = {0};

	while (1)
	{
		if (xQueueReceive (m_uart_queue, (void * ) &event, portMAX_DELAY) )
		{
			switch (event.type)
			{
				case UART_DATA:
					uart_read_bytes (PRJ_UART_PORT_NUM, data, event.size, portMAX_DELAY);
					ESP_LOGI (PRJ_UART_TAG, "uart event task: received data: %.*s", event.size, data);
					uart_write_bytes (PRJ_UART_PORT_NUM, (const char *) data, event.size);
					break;

				default:
					ESP_LOGI (PRJ_UART_TAG, "uart event task: unknown event type: %d", event.type);
					break;
			}
		}
	}

	vTaskDelete (m_uart_task);
	return;
}
/***************************************************************************************************
 * EOF
 **************************************************************************************************/
