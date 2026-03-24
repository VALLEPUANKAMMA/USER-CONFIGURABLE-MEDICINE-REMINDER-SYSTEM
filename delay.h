/*
 * ============================================================
 * FILE: delay.h (Function Declarations)
 * Author: Ankamma V
 * DESCRIPTION:
 * Delay function prototypes for generating time delays
 * in microseconds, milliseconds, and seconds.
 *
 * PURPOSE:
 * - Used for timing control in embedded systems
 * - Required for peripherals like LCD, keypad, etc.
 * ============================================================
 */
#ifndef _DELAY_H
#define _DELAY_H
#include "typedef.h"

/*
 * ------------------------------------------------------------
 * delay_us:
 * Generates delay in microseconds (µs)
 *
 * Parameter:
 *   dlyus ? Delay time in microseconds
 *
 * NOTE:
 * Depends on CPU clock frequency
 * ------------------------------------------------------------
 */
void delay_us(u32 dlyus);

/*
 * ------------------------------------------------------------
 * delay_ms:
 * Generates delay in milliseconds (ms)
 *
 * Parameter:
 *   dlyms ? Delay time in milliseconds
 *
 * NOTE:
 * Commonly used for LCD commands and debounce
 * ------------------------------------------------------------
 */
void delay_ms(u32 dlyms);

/*
 * ------------------------------------------------------------
 * delay_s:
 * Generates delay in seconds (s)
 *
 * Parameter:
 *   dlys ? Delay time in seconds
 *
 * NOTE:
 * Used for long delays (e.g., buzzer, alerts)
 * ------------------------------------------------------------
 */
void delay_s(u32 dlys);
#endif
