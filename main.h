/*
 * ============================================================
 * FILE: main.h (Application-Level Function Declarations)
 * DESCRIPTION:
 * Function prototypes for medicine reminder logic
 * using RTC (Real Time Clock).
 *
 * PURPOSE:
 * - Compare current RTC time with medicine timings
 * - Trigger alerts (LCD / Buzzer)
 * - Control main RTC display flow
 * ============================================================
 */
#ifndef _MAIN_
#define _MAIN_
#include "typedef.h"

/*
 * ------------------------------------------------------------
 * Compare_Medicine1:
 * Compares current time with Medicine 1 schedule
 *
 * Parameters:
 *   a ? Current hour
 *   b ? Current minute
 *   c ? Current second
 *
 * Returns:
 *   1 ? Time matches (Take Medicine)
 *   0 ? No match
 * ------------------------------------------------------------
 */
u32 Compare_Medicine1(u32 a,u32 b,u32 c);

/*
 * ------------------------------------------------------------
 * Compare_Medicine2:
 * Compares current time with Medicine 2 schedule
 *
 * Parameters:
 *   a : Current hour
 *   b : Current minute
 *   c : Current second
 *
 * Returns:
 *   1 : Time matches
 *   0 : No match
 * ------------------------------------------------------------
 */
u32 Compare_Medicine2(u32 a,u32 b,u32 c);

/*
 * ------------------------------------------------------------
 * RTC:
 * Main RTC handling function
 *
 * Operations:
 * - Reads current time/date/day
 * - Displays on LCD
 * - Calls medicine comparison functions
 * - Triggers alert if time matches
 * ------------------------------------------------------------
 */
void RTC(void);
#endif
