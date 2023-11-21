/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "math.h"

#include "cerberusdef.h"
#include "benchmark.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LED_GPIO     GPIO3
#define EXAMPLE_LED_GPIO_PIN 16U

int test_initial = 1000; 
int test_repeat = 1000;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The PIN status */
volatile bool g_pinSet = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    gpio_pin_config_t test_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Board pin, clock, debug console init */
    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Print a note to terminal. */
    PRINTF("\r\n Test Benchmark init: %d , repeat: %d\r\n" , test_initial, test_repeat);

    /* Init output LED GPIO. */
    GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(TEST_GPIO, TEST_GPIO_PIN, &test_config);

    while (1)
    {

        start_calculation(test_initial , test_repeat);
        
//         PRINTF("\r\n TESTING GPIO 5 \r\n");
//         SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
// #if (defined(FSL_FEATURE_IGPIO_HAS_DR_TOGGLE) && (FSL_FEATURE_IGPIO_HAS_DR_TOGGLE == 1))
//         GPIO_PortToggle(EXAMPLE_LED_GPIO, 1u << EXAMPLE_LED_GPIO_PIN);
// #else
//         if (g_pinSet)
//         {
//             GPIO_PinWrite(TEST_GPIO, TEST_GPIO_PIN, 0U);
//             GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0U);
//             g_pinSet = false;
//         }
//         else
//         {
//             GPIO_PinWrite(TEST_GPIO, TEST_GPIO_PIN, 1U);
//             GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1U);
//             g_pinSet = true;
//         }
// #endif /* FSL_FEATURE_IGPIO_HAS_DR_TOGGLE */
    }
}
