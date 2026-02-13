/***************************************************************************************************
* Includes
**************************************************************************************************/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <inttypes.h>
/***************************************************************************************************
* Definitions
**************************************************************************************************/
#define GPIO_OUTPUT_PIN_SEL (1ULL << CONFIG_BLINK_GPIO)
#define GPIO_INPUT_PIN_SEL ((1ULL << CONFIG_BUTTON_GPIO_1) | (1ULL << CONFIG_BUTTON_GPIO_2))
#define ESP_INTR_FLAG_DEFAULT 0

/***************************************************************************************************
* Variables
**************************************************************************************************/
static QueueHandle_t m_gpio_evt_queue = NULL;

/***************************************************************************************************
* Static functions declaration
**************************************************************************************************/
static void exti_task1(void *p_arg);

/***************************************************************************************************
* API
**************************************************************************************************/
/**
* @brief Main application entry point
*
* @note This function initializes GPIO configuration and starts interrupt handling
*/
void app_main(void)
{
    int cnt = 0;
    gpio_config_t io_conf = {};
    
    /* Configure output pin */
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

    /* Configure input pins */
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    gpio_set_intr_type(CONFIG_BUTTON_GPIO_1, GPIO_INTR_ANYEDGE);
    m_gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    /* Start the interrupt handling task */
    xTaskCreate(exti_task1, "exti_task1", 2048, NULL, 10, NULL);

    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
/***************************************************************************************************
* STATIC
**************************************************************************************************/
/**
* @brief Task to handle GPIO interrupts
*
* @param p_arg Task argument (unused)
*/
static void exti_task1(void *p_arg)
{
    uint32_t io_num;
    
    for (;;)
    {
        if (xQueueReceive(m_gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            printf("GPIO[%"PRIu32"] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

/***************************************************************************************************
* EOF
**************************************************************************************************/