/*
 * ============================================================
 * FILE: rtc.h (Real Time Clock Function Declarations)
 * DESCRIPTION:
 * Function prototypes for Real Time Clock (RTC)
 * operations in LPC21xx microcontroller.
 *
 * PURPOSE:
 * - Initialize RTC
 * - Set and get time/date/day
 * - Display time/date/day on LCD
 * ============================================================
 */
#ifndef _RTC_
#define _RTC_
#include "typedef.h"

/*
 * ------------------------------------------------------------
 * RTC_Init:
 * Initializes the Real Time Clock
 *
 * Operations:
 * - Reset RTC counters
 * - Load prescaler values
 * - Enable RTC
 * ------------------------------------------------------------
 */
void RTC_Init(void);

/*
 * ------------------------------------------------------------
 * GetRTCTimeInfo:
 * Reads current time from RTC registers
 *
 * Parameters:
 *   (hour, minute, second) ? pointers to store values
 * ------------------------------------------------------------
 */
void GetRTCTimeInfo(s32 *,s32 *,s32 *);

/*
 * ------------------------------------------------------------
 * DisplayRTCTime:
 * Displays time on LCD
 *
 * Parameters:
 *   hour, minute, second
 * ------------------------------------------------------------
 */
void DisplayRTCTime(u32,u32,u32);

/*
 * ------------------------------------------------------------
 * GetRTCDateInfo:
 * Reads current date from RTC registers
 *
 * Parameters:
 *   date, month, year ? pointers
 * ------------------------------------------------------------
 */
void GetRTCDateInfo(s32 *,s32 *,s32 *);

/*
 * ------------------------------------------------------------
 * DisplayRTCDate:
 * Displays date on LCD
 *
 * Parameters:
 *   date, month, year
 * ------------------------------------------------------------
 */
void DisplayRTCDate(u32,u32,u32);

/*
 * ------------------------------------------------------------
 * SetRTCTimeInfo:
 * Sets RTC time
 *
 * Parameters:
 *   hour, minute, second
 * ------------------------------------------------------------
 */
void SetRTCTimeInfo(u32,u32,u32);

/*
 * ------------------------------------------------------------
 * SetRTCDateInfo:
 * Sets RTC date
 *
 * Parameters:
 *   date, month, year
 * ------------------------------------------------------------
 */
void SetRTCDateInfo(u32,u32,u32);

/*
 * ------------------------------------------------------------
 * GetRTCDay:
 * Reads current day (weekday)
 *
 * Parameter:
 *   Pointer to store day value
 * ------------------------------------------------------------
 */
void GetRTCDay(s32 *);

/*
 * ------------------------------------------------------------
 * DisplayRTCDay:
 * Displays weekday on LCD
 *
 * Parameter:
 *   Day value (0–6 or 1–7 depending on config)
 * ------------------------------------------------------------
 */
void DisplayRTCDay(u32);

/*
 * ------------------------------------------------------------
 * SetRTCDay:
 * Sets weekday in RTC
 *
 * Parameter:
 *   Day value
 * ------------------------------------------------------------
 */
void SetRTCDay(u32);
#endif
