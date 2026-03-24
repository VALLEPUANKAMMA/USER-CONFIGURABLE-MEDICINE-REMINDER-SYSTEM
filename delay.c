/*
 * ============================================================
 * FILE: delay.c
 * Author: Ankamma V
 * DESCRIPTION:
 * Software delay functions using loop-based timing.
 *
 * NOTE:
 * - These delays are approximate (not exact).
 * - Depends on CPU clock frequency.
 * - Assumes ~60 MHz CPU clock (LPC2148).
 * ============================================================
 */

#include "typedef.h"

/*
 * ------------------------------------------------------------
 * FUNCTION: delay_us
 * ------------------------------------------------------------
 * Description:
 * Generates approximate delay in microseconds.
 *
 * Parameters:
 *   dlyus ? Delay time in microseconds
 *
 * Working:
 *   - Multiplies delay by 12 (loop calibration)
 *   - Each loop consumes some CPU cycles
 *
 * NOTE:
 *   - Accuracy depends on compiler optimization & clock
 * ------------------------------------------------------------
 */
void delay_us(u32 dlyus)
{
    /*
     * Multiply input to adjust loop count
     * (Calibration factor for microsecond delay)
     */
    for(dlyus *= 12; dlyus > 0; dlyus--);
}

/*
 * ------------------------------------------------------------
 * FUNCTION: delay_ms
 * ------------------------------------------------------------
 * Description:
 * Generates approximate delay in milliseconds.
 *
 * Parameters:
 *   dlyms ? Delay time in milliseconds
 *
 * Working:
 *   - Converts milliseconds to loop count
 *   - Uses 12000 factor (based on CPU frequency)
 * ------------------------------------------------------------
 */
void delay_ms(u32 dlyms)
{
    /*
     * Multiply delay for millisecond timing
     */
    for(dlyms *= 12000; dlyms > 0; dlyms--);
}

/*
 * ------------------------------------------------------------
 * FUNCTION: delay_s
 * ------------------------------------------------------------
 * Description:
 * Generates approximate delay in seconds.
 *
 * Parameters:
*   dlys : Delay time in seconds
 *
 * Working:
 *   - Converts seconds into loop iterations
 *   - Uses large multiplication factor
 * ------------------------------------------------------------
 */
void delay_s(u32 dlys)
{
    /*
     * Multiply delay for second timing
     */
    for(dlys *= 12000000; dlys > 0; dlys--);
}
