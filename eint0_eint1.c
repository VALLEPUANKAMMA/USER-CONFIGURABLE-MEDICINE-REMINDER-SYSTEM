/*
 * ============================================================
 * FILE: external_interrupt.c
 * Author: Ankamma V
 * DESCRIPTION:
 * Configures External Interrupts (EINT0 & EINT1) on LPC21xx
 * using VIC (Vectored Interrupt Controller).
 *
 * FEATURES:
 * - Configures P0.1 : EINT0
 * - Configures P0.3 : EINT1
 * - Enables interrupts in VIC
 * - Links ISR functions to interrupt channels
 * ============================================================
 */

#include<lpc21xx.h>
#include"typedef.h"

/*
 * ------------------------------------------------------------
 * Macro Definitions
 * ------------------------------------------------------------
 */

/* PINSEL configuration values for EINT0 (P0.1) */
#define EINT0_INPUT_PIN 0x0000000C

/* VIC channel number for EINT0 */
#define EINT0_VIC_CHNO 14

/* PINSEL configuration values for EINT1 (P0.3) */
#define EINT1_INPUT_PIN 0x000000C0

/* VIC channel number for EINT1 */
#define EINT1_VIC_CHNO 15


/*
 * ------------------------------------------------------------
 * ISR Function Prototypes
 * ------------------------------------------------------------
 */
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

/*
 * ------------------------------------------------------------
 * FUNCTION: InitInt
 * ------------------------------------------------------------
 * Description:
 * Initializes external interrupts EINT0 and EINT1.
 *
 * Steps:
 *   1. Configure pins as interrupt inputs
 *   2. Enable interrupt channels in VIC
 *   3. Assign ISR addresses
 *   4. Configure edge-trigger mode
 * ------------------------------------------------------------
 */
void InitInt(void)
{
    /*
     * Step 1: Clear PINSEL bits for P0.1 and P0.3
     * (Reset previous configuration)
     */
    PINSEL0 &= ~((3 << 2) | (3 << 6));

    /*
     * Step 2: Configure pins as EINT0 and EINT1
     * P0.1 ? EINT0
     * P0.3 ? EINT1
     */
    PINSEL0 |= (EINT0_INPUT_PIN | EINT1_INPUT_PIN);

    /*
     * Step 3: Enable interrupts in VIC
     * Enable channel 14 (EINT0) and 15 (EINT1)
     */
    VICIntEnable = (1 << EINT0_VIC_CHNO) | (1 << EINT1_VIC_CHNO);

    /*
     * Step 4: Configure VIC vector control
     * Bit[5] = 1 ? Enable vector slot
     * Lower bits ? Channel number
     */
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;

    /*
     * Step 5: Assign ISR address for EINT0
     */
    VICVectAddr0 = (u32)eint0_isr;

    /*
     * Configure second interrupt (EINT1)
     */
    VICVectCntl1 = (1 << 5) | EINT1_VIC_CHNO;

    /*
     * Assign ISR address for EINT1
     */
    VICVectAddr1 = (u32)eint1_isr;

    /*
     * Step 6: Configure interrupt mode
     * EXTMODE bit:
     * 1 ? Edge triggered
     * 0 ? Level triggered
     *
     * Here both EINT0 and EINT1 are edge triggered
     */
    EXTMODE = (1 << 0) | (1 << 1);
}
