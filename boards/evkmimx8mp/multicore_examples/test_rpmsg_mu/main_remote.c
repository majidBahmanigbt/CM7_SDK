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
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"


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
static char app_buf[512]; 
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

static struct rpmsg_lite_instance *volatile my_rpmsg = NULL;
static struct rpmsg_lite_endpoint *volatile my_ept = NULL;
static volatile rpmsg_queue_handle my_queue        = NULL;

int32_t MU1_M7_IRQHandler(void){

    // uint32_t channel;
    
    // if(state1 == 0){
    //     state1 = 1;
    //     GPIO5->DR |= 1u << 9;
    // }else{
    //     state1 = 0;
    //     GPIO5->DR &= 0xFFFFFDFF;
    // }
    GPIO5->DR |= 1u << 9;
    GPIO5->DR &= 0xFFFFFDFF;
    PRINTF("Received!\n");
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

    volatile uint32_t remote_addr;
    void *rx_buf;
    uint32_t len;
    int32_t result;
    void *tx_buf;
    uint32_t size;

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
#else
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
#endif /* MCMGR_USED */

    GPIO_PinInit(TEST_OUTPUT_GPIO , TEST_OUTPUT_PIN , &output_config);
    GPIO5->DR = GPIO5->DR & 0xFFFFFDFF;
    PRINTF("Start MU New 1\n");

    MU_Init(MUB);
    MU_EnableInterrupts(MUB, kMU_GenInt0InterruptEnable);
    NVIC_SetPriority(MU1_M7_IRQn, APP_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(MU1_M7_IRQn);
    platform_interrupt_enable(MU1_M7_IRQn);

    rpmsg_lite_wait_for_link_up(my_rpmsg, RL_BLOCK);

    my_queue = rpmsg_queue_create(my_rpmsg);
    my_ept = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);
    (void)rpmsg_ns_announce(my_rpmsg, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, RL_NS_CREATE);

    for (;;)
    {
        /* Get RPMsg rx buffer with message */
        result =
            rpmsg_queue_recv_nocopy(my_rpmsg, my_queue, (uint32_t *)&remote_addr, (char **)&rx_buf, &len, RL_BLOCK);
        if (result != 0)
        {
            assert(false);
        }

        /* Copy string from RPMsg rx buffer */
        assert(len < sizeof(app_buf));
        memcpy(app_buf, rx_buf, len);
        app_buf[len] = 0; /* End string by '\0' */

        if ((len == 2) && (app_buf[0] == 0xd) && (app_buf[1] == 0xa))
            PRINTF("Get New Line From Master Side\r\n");
        else
            PRINTF("Get Message From Master Side : \"%s\" [len : %d]\r\n", app_buf, len);

        /* Get tx buffer from RPMsg */
        tx_buf = rpmsg_lite_alloc_tx_buffer(my_rpmsg, &size, RL_BLOCK);
        assert(tx_buf);
        /* Copy string to RPMsg tx buffer */
        memcpy(tx_buf, app_buf, len);
        /* Echo back received message with nocopy send */
        result = rpmsg_lite_send_nocopy(my_rpmsg, my_ept, remote_addr, tx_buf, len);
        if (result != 0)
        {
            assert(false);
        }
        /* Release held RPMsg rx buffer */
        result = rpmsg_queue_nocopy_free(my_rpmsg, rx_buf);
        if (result != 0)
        {
            assert(false);
        }
    }

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
