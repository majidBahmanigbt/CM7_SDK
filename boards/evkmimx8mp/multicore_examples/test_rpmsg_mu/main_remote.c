/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_platform.h"
#include "rpmsg_env.h"
#include "fsl_device_registers.h"
#include "fsl_mu.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_uart.h"
#include "rsc_table.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RPMSG_LITE_SHMEM_BASE         (VDEV0_VRING_BASE)
#define RPMSG_LITE_LINK_ID            (RL_PLATFORM_IMX8MP_M7_USER_LINK_ID)
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-virtual-tty-channel-1"
#define APP_TASK_STACK_SIZE (256)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif

#define APP_MU_IRQ_PRIORITY (3U)

#define TEST_OUTPUT_GPIO GPIO5
#define TEST_OUTPUT_PIN  9U  

int state1 = 0;

// static void *platform_lock;

// /* Globals */
// static char app_buf[512]; /* Each RPMSG buffer can carry less than 512 payload */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
// static TaskHandle_t app_task_handle = NULL;

int32_t MU1_M7_IRQHandler(void){

    // uint32_t channel;
    
    PRINTF("Received!\n");
    if(state1 == 0){
        state1 = 1;
        GPIO5->DR |= 1u << 9;
    }else{
        state1 = 0;
        GPIO5->DR &= 0xFFFFFDFF;
    }
    // GPIO5->DR = 1u << 9;
    // GPIO5->DR = 0u;
    // if ((((1UL << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(MUB)) != 0UL)
    // {
    // 	PRINTF("Received2!\n");
    //     channel = MU_ReceiveMsgNonBlocking(MUB, RPMSG_MU_CHANNEL); // Read message from RX register.
    //     env_isr(channel >> 16);
    // }

    MU_ClearStatusFlags(MUB, kMU_GenInt0Flag);

    /* ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
     * exception return operation might vector to incorrect interrupt.
     * For Cortex-M7, if core speed much faster than peripheral register write speed,
     * the peripheral interrupt flags may be still set after exiting ISR, this results to
     * the same error similar with errata 83869 */
// #if (defined __CORTEX_M) && ((__CORTEX_M == 4U) || (__CORTEX_M == 7U))
    __DSB();
// #endif

    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{

    gpio_pin_config_t output_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    /* Initialize standard SDK demo application pins */
    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    copyResourceTable();

#ifdef MCMGR_USED
    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();
#endif /* MCMGR_USED */

    GPIO_PinInit(TEST_OUTPUT_GPIO , TEST_OUTPUT_PIN , &output_config);
    GPIO5->DR = GPIO5->DR & 0xFFFFFDFF;
    PRINTF("Start MU New 1\n");

    MU_Init(MUB);
    MU_EnableInterrupts(MUB, kMU_GenInt0InterruptEnable);
    NVIC_SetPriority(MU1_M7_IRQn, APP_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(MU1_M7_IRQn);
    platform_interrupt_enable(MU1_M7_IRQn);

//     /* Create lock used in multi-instanced RPMsg */
// #if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
//     if (0 != env_create_mutex(&platform_lock, 1, &platform_lock_static_ctxt))
// #else
//     if (0 != env_create_mutex(&platform_lock, 1))
// #endif
//     {
//         return -1;
//     }

    PRINTF("Start MU New 2\n");

    for (;;)
        ;
}
