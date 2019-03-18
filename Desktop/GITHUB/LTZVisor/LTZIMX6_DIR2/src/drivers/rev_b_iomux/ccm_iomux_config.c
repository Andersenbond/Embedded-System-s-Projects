/*
 * Copyright (c) 2012, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// File: ccm_iomux_config.c

/* ------------------------------------------------------------------------------
 * <auto-generated>
 *     This code was generated by a tool.
 *     Runtime Version:3.4.0.0
 *
 *     Changes to this file may cause incorrect behavior and will be lost if
 *     the code is regenerated.
 * </auto-generated>
 * ------------------------------------------------------------------------------
*/

#include <iomux_config.h>
#include <regsiomuxc.h>

// Function to configure IOMUXC for ccm module.
void ccm_iomux_config(void)
{
    // Config ccm.CCM_CLKO1 to pad GPIO00(T5)
    // HW_IOMUXC_SW_MUX_CTL_PAD_GPIO00_WR(0x00000000);
    // HW_IOMUXC_SW_PAD_CTL_PAD_GPIO00_WR(0x0001B0B0);
    // Mux Register:
    // IOMUXC_SW_MUX_CTL_PAD_GPIO00(0x020E0220)
    //   SION [4] - Software Input On Field Reset: DISABLED
    //              Force the selected mux mode Input path no matter of MUX_MODE functionality.
    //     DISABLED (0) - Input Path is determined by functionality of the selected mux mode (regular).
    //     ENABLED (1) - Force input path of pad.
    //   MUX_MODE [2:0] - MUX Mode Select Field Reset: ALT5
    //                    Select iomux modes to be used for pad.
    //     ALT0 (0) - Select instance: ccm signal: CCM_CLKO1
    //     ALT2 (2) - Select instance: kpp signal: KEY_COL5
    //     ALT3 (3) - Select instance: asrc signal: ASRC_EXT_CLK
    //     ALT4 (4) - Select instance: epit1 signal: EPIT1_OUT
    //     ALT5 (5) - Select instance: gpio1 signal: GPIO1_IO00
    //     ALT6 (6) - Select instance: usb signal: USB_H1_PWR
    //     ALT7 (7) - Select instance: snvs signal: SNVS_VIO_5
    HW_IOMUXC_SW_MUX_CTL_PAD_GPIO00_WR(
            BF_IOMUXC_SW_MUX_CTL_PAD_GPIO00_SION_V(DISABLED) |
            BF_IOMUXC_SW_MUX_CTL_PAD_GPIO00_MUX_MODE_V(ALT0));
    // Pad Control Register:
    // IOMUXC_SW_PAD_CTL_PAD_GPIO00(0x020E05F0)
    //   HYS [16] - Hysteresis Enable Field Reset: ENABLED
    //     DISABLED (0) - CMOS input
    //     ENABLED (1) - Schmitt trigger input
    //   PUS [15:14] - Pull Up / Down Config. Field Reset: 100K_OHM_PU
    //     100K_OHM_PD (0) - 100K Ohm Pull Down
    //     47K_OHM_PU (1) - 47K Ohm Pull Up
    //     100K_OHM_PU (2) - 100K Ohm Pull Up
    //     22K_OHM_PU (3) - 22K Ohm Pull Up
    //   PUE [13] - Pull / Keep Select Field Reset: PULL
    //     KEEP (0) - Keeper Enabled
    //     PULL (1) - Pull Enabled
    //   PKE [12] - Pull / Keep Enable Field Reset: ENABLED
    //     DISABLED (0) - Pull/Keeper Disabled
    //     ENABLED (1) - Pull/Keeper Enabled
    //   ODE [11] - Open Drain Enable Field Reset: DISABLED
    //              Enables open drain of the pin.
    //     DISABLED (0) - Output is CMOS.
    //     ENABLED (1) - Output is Open Drain.
    //   SPEED [7:6] - Speed Field Reset: 100MHZ
    //                 NOTE: Read Only Field
    //                 The value of this field is fixed and cannot be changed.
    //     TBD (0) - TBD
    //     50MHZ (1) - Low (50 MHz)
    //     100MHZ (2) - Medium (100 MHz)
    //     200MHZ (3) - Maximum (200 MHz)
    //   DSE [5:3] - Drive Strength Field Reset: 40_OHM
    //     HIZ (0) - HI-Z
    //     240_OHM (1) - 240 Ohm
    //     120_OHM (2) - 120 Ohm
    //     80_OHM (3) - 80 Ohm
    //     60_OHM (4) - 60 Ohm
    //     48_OHM (5) - 48 Ohm
    //     40_OHM (6) - 40 Ohm
    //     34_OHM (7) - 34 Ohm
    //   SRE [0] - Slew Rate Field Reset: SLOW
    //             Slew rate control.
    //     SLOW (0) - Slow Slew Rate
    //     FAST (1) - Fast Slew Rate
    HW_IOMUXC_SW_PAD_CTL_PAD_GPIO00_WR(
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_HYS_V(ENABLED) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_PUS_V(100K_OHM_PU) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_PUE_V(PULL) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_PKE_V(ENABLED) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_ODE_V(DISABLED) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_DSE_V(40_OHM) |
            BF_IOMUXC_SW_PAD_CTL_PAD_GPIO00_SRE_V(SLOW));
}
