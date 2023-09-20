/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v9.0
processor: MIMX8ML8xxxLZ
package_id: MIMX8ML8DVNLZ
mcu_data: ksdk2_0
processor_version: 0.9.1
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm7}
- pin_list:
  - {pin_num: AJ5, peripheral: UART4, signal: uart_rx, pin_signal: UART4_RXD, PE: Disabled, PUE: Weak_Pull_Up, DSE: X6}
  - {pin_num: AH5, peripheral: UART4, signal: uart_tx, pin_signal: UART4_TXD, PE: Disabled, PUE: Weak_Pull_Up, DSE: X6}
  - {pin_num: AH20, peripheral: ECSPI2, signal: ecspi_miso, pin_signal: ECSPI2_MISO, PE: Disabled, HYS: Schmitt, DSE: X2}
  - {pin_num: AJ21, peripheral: ECSPI2, signal: ecspi_mosi, pin_signal: ECSPI2_MOSI, PE: Disabled, HYS: Schmitt, DSE: X2}
  - {pin_num: AH21, peripheral: ECSPI2, signal: ecspi_sclk, pin_signal: ECSPI2_SCLK, PE: Disabled, HYS: Schmitt, DSE: X2}
  - {pin_num: AJ22, peripheral: ECSPI2, signal: 'ecspi_ss, 0', pin_signal: ECSPI2_SS0, PE: Disabled, HYS: Schmitt, DSE: X2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M7F[m7] */
    IOMUXC_SetPinMux(IOMUXC_ECSPI2_MISO_ECSPI2_MISO, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ECSPI2_MISO_ECSPI2_MISO, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_ECSPI2_MOSI_ECSPI2_MOSI, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ECSPI2_MOSI_ECSPI2_MOSI, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_ECSPI2_SCLK_ECSPI2_SCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ECSPI2_SCLK_ECSPI2_SCLK, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_ECSPI2_SS0_ECSPI2_SS0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ECSPI2_SS0_ECSPI2_SS0, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART4_RXD_UART4_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RXD_UART4_RX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART4_TXD_UART4_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART4_TXD_UART4_TX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
