/*
 * File: rtc.c
 * Author: Ankamma V
 * Description:
 * ------------------------------------------------------------
 * This file implements Real-Time Clock (RTC) functions using
 * LPC214x microcontroller and displays time/date on 16x2 LCD.
 *
 * RTC provides time (HH:MM:SS), date (DD/MM/YYYY) and day.
 * LCD is interfaced in 8-bit mode using HD44780 controller.
 *
 * ------------------------------------------------------------
 */

#include <lpc214x.h>     /* LPC214x register definitions */
#include "typedef.h"     /* User-defined data types */
#include "lcd1.h"        /* LCD function declarations */
#include "LCD_defines.h" /* LCD command macros */


/*
 * ------------------------------------------------------------
 * SYSTEM CLOCK CONFIGURATION
 * ------------------------------------------------------------
 * FOSC  : Main oscillator frequency
 * CCLK  : CPU clock
 * PCLK  : Peripheral clock (used for RTC)
 */
#define FOSC 12000000
#define CCLK (5*FOSC)    
#define PCLK (CCLK/4)


/*
 * ------------------------------------------------------------
 * RTC PRESCALER CALCULATION
 * ------------------------------------------------------------
 * RTC requires 32.768 kHz for 1-second increment.
 * PREINT and PREFRAC divide PCLK to generate this.
 *
 * Formula:
 * PREINT  = (PCLK / 32768) - 1
 * PREFRAC = PCLK - ((PREINT + 1) * 32768)
 */
#define PREINT_VAL (int)((PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))


/*
 * ------------------------------------------------------------
 * RTC CONTROL REGISTER BITS
 * ------------------------------------------------------------
 */

/* Bit 0 : Enable RTC counter */
#define RTC_ENABLE (1<<0)

/* Bit 1 : Reset RTC counter */
#define RTC_RESET  (1<<1)

/*
* Bit 4 : Clock source selection (only for LPC2148)
* 0 : Internal clock (PCLK)
* 1 : External 32.768 kHz crystal
 */
#define RTC_CLKSRC (1<<4)


/*
 * ------------------------------------------------------------
 * DAY DEFINITIONS
 * ------------------------------------------------------------
 */
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6


/*
 * ------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ------------------------------------------------------------
 */
 /*
 * ------------------------------------------------------------
 * FUNCTION: RTC_Init
 * ------------------------------------------------------------
 * Description:
 * Initializes the RTC module by:
 *   - Resetting RTC counters
 *   - Setting prescaler values (PREINT, PREFRAC)
 *   - Enabling RTC
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void RTC_Init(void);
/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCTimeInfo
 * ------------------------------------------------------------
 * Description:
 * Reads current time from RTC registers.
 *
 * Parameters:
 *   hour   ? Pointer to store hour (0–23)
 *   minute ? Pointer to store minute (0–59)
 *   second ? Pointer to store second (0–59)
 *
 * Returns:
 *   None
 */
void GetRTCTimeInfo(s32 *,s32 *,s32 *);
/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCTime
 * ------------------------------------------------------------
 * Description:
 * Displays time on LCD in HH:MM:SS format.
 *
 * Parameters:
 *   hour   ? Hour value (0–23)
 *   minute ? Minute value (0–59)
 *   second ? Second value (0–59)
 *
 * Returns:
 *   None
 */
void DisplayRTCTime(u32,u32,u32);
/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCDateInfo
 * ------------------------------------------------------------
 * Description:
 * Reads current date from RTC registers.
 *
 * Parameters:
 *   date  ? Pointer to store day (1–31)
 *   month ? Pointer to store month (1–12)
 *   year  ? Pointer to store year (e.g., 2026)
 *
 * Returns:
 *   None
 */
void GetRTCDateInfo(s32 *,s32 *,s32 *);
/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCDate
 * ------------------------------------------------------------
 * Description:
 * Displays date on LCD in DD/MM/YYYY format.
 *
 * Parameters:
 *   date  ? Day (1–31)
 *   month ? Month (1–12)
 *   year  ? Year (4-digit)
 *
 * Returns:
 *   None
 */
void DisplayRTCDate(u32,u32,u32);
/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCTimeInfo
 * ------------------------------------------------------------
 * Description:
 * Sets RTC time registers.
 *
 * Parameters:
 *   hour   ? Hour (0–23)
 *   minute ? Minute (0–59)
 *   second ? Second (0–59)
 *
 * Returns:
 *   None
 */
void SetRTCTimeInfo(s32,s32,s32);
/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCDateInfo
 * ------------------------------------------------------------
 * Description:
 * Sets RTC date registers.
 *
 * Parameters:
 *   date  ? Day (1–31)
 *   month ? Month (1–12)
 *   year  ? Year (4-digit)
 *
 * Returns:
 *   None
 */
void SetRTCDateInfo(u32,u32,u32);
/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCDay
 * ------------------------------------------------------------
 * Description:
 * Reads current day of week from RTC.
 *
 * Parameters:
 *   dow ? Pointer to store day (0=SUN ... 6=SAT)
 *
 * Returns:
 *   None
 */
void GetRTCDay(s32 *);
/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCDay
 * ------------------------------------------------------------
 * Description:
 * Displays day of week on LCD (SUN–SAT).
 *
 * Parameters:
 *   dow ? Day of week (0=SUN ... 6=SAT)
 *
 * Returns:
 *   None
 */
void DisplayRTCDay(u32);
/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCDay
 * ------------------------------------------------------------
 * Description:
 * Sets day of week in RTC.
 *
 * Parameters:
 *   dow ? Day (0=SUN ... 6=SAT)
 *
 * Returns:
 *   None
 */
void SetRTCDay(u32);


/*
 * ------------------------------------------------------------
 * GLOBAL VARIABLES
 * ------------------------------------------------------------
 */
s32 hour,min,sec,date,month,year,day;

/*
 * Array to store week names
 * Index: 0=SUN ... 6=SAT
 */
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};


/* Define controller type */
#define _LPC2148


/*
 * ------------------------------------------------------------
 * FUNCTION: RTC_Init
 * ------------------------------------------------------------
 * Initializes RTC:
 * 1. Reset RTC
 * 2. Load prescaler values
 * 3. Enable RTC
 */
void RTC_Init(void) 
{
    /*
     * Reset RTC counters
     * RTC_RESET bit clears all time and date registers
     */
    CCR = RTC_RESET;

    /*
     * Load PREINT register
     * This sets the integer part of prescaler division
     * Formula:
     *   PREINT = (PCLK / 32768) - 1
     */
    PREINT = PREINT_VAL;

    /*
     * Load PREFRAC register
     * This sets the fractional correction part
     * Formula:
     *   PREFRAC = PCLK - ((PREINT + 1) * 32768)
     */
    PREFRAC = PREFRAC_VAL;

    /*
     * Enable RTC
     * RTC_ENABLE bit starts the RTC counters
     */
    CCR = RTC_ENABLE;

    /*
     * Optional: Use external 32.768 kHz crystal
     * Provides higher accuracy compared to internal clock
     *
     * Uncomment below line if external crystal is connected:
     *
     * CCR = RTC_ENABLE | RTC_CLKSRC;
     */
}

/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCTimeInfo
 * ------------------------------------------------------------
 * Reads current time from RTC registers
 * Outputs via pointers
 */
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
    /*
     * Read HOUR register
     * and store the value in the variable pointed by 'hour'
     */
    *hour = HOUR;

    /*
     * Read MIN register
     * and store the value in the variable pointed by 'minute'
     */
    *minute = MIN;

    /*
     * Read SEC register
     * and store the value in the variable pointed by 'second'
     */
    *second = SEC;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCTime
 * ------------------------------------------------------------
 * Displays time on LCD in HH:MM:SS format
 */
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
    /*
     * Move cursor to Line 1, Position 0
     * (Starting position of first row)
     */
    CmdLCD(GOTO_LINE1_POS0);

    /*
     * Display hour (2 digits)
	* hour/10 : tens digit
	* hour%10 : units digit
     * +48 ? convert number to ASCII
     */
    CharLCD(hour/10 + 48);
    CharLCD(hour%10 + 48);

    /*
     * Display ':' separator
     */
    CharLCD(':');

    /*
     * Display minute (2 digits)
     */
    CharLCD(minute/10 + 48);
    CharLCD(minute%10 + 48);

    /*
     * Display ':' separator
     */
    CharLCD(':');

    /*
     * Display second (2 digits)
     */
    CharLCD(second/10 + 48);
    CharLCD(second%10 + 48);
}

/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCDateInfo
 * ------------------------------------------------------------
 * Reads date from RTC registers
 */
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
    /*
     * Read Day Of Month (DOM) register
     * and store value in the variable pointed by 'date'
     */
    *date = DOM;

    /*
     * Read MONTH register
     * and store value in the variable pointed by 'month'
     */
    *month = MONTH;

    /*
     * Read YEAR register
     * and store value in the variable pointed by 'year'
     */
    *year = YEAR;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCDate
 * ------------------------------------------------------------
 * Displays date in DD/MM/YYYY format
 */
/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCDate
 * ------------------------------------------------------------
 * Description:
 * Displays the current date on the LCD in the format:
 *      DD/MM/YYYY
 *
 * This function takes numerical values of date, month, and year,
 * converts them into ASCII characters, and sends them to the LCD.
 *
 * Parameters:
 *   date  ? Day of the month (1–31)
 *   month ? Month (1–12)
 *   year  ? Year (e.g., 2026)
 *
 * Returns:
 *   None
 * ------------------------------------------------------------
 */
void DisplayRTCDate(u32 date, u32 month, u32 year)
{
    /*
     * Move cursor to the beginning of second line.
     * GOTO_LINE2_POS0 is a command (0xC0) that sets DDRAM address.
     */
    CmdLCD(GOTO_LINE2_POS0);

    /*
     * Display tens digit of date.
     * Example: if date = 25 ? 25/10 = 2
     * Adding 48 converts number ? ASCII ('2')
     */
    CharLCD(date/10 + 48);

    /*
     * Display units digit of date.
     * Example: 25 % 10 = 5 ? ASCII '5'
     */
    CharLCD(date%10 + 48);

    /*
     * Display separator '/'
     * Used to format date as DD/MM/YYYY
     */
    CharLCD('/');

    /*
     * Display tens digit of month.
     * Example: 08 ? 0
     */
    CharLCD(month/10 + 48);

    /*
     * Display units digit of month.
     * Example: 08 ? 8
     */
    CharLCD(month%10 + 48);

    /*
     * Display separator '/'
     */
    CharLCD('/');

    /*
     * Display full year (4-digit number).
     * Uses U32LCD() function which:
     *   - Converts integer to string
     *   - Displays each digit on LCD
     */
    U32LCD(year);
}


/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCTimeInfo
 * ------------------------------------------------------------
 * Sets RTC time registers
 */
void SetRTCTimeInfo(s32 hour, s32 minute, s32 second)
{
    /*
     * Write hour value to HOUR register.
     * This sets the current hour in RTC.
     * Range: 0 to 23 (24-hour format)
     */
    HOUR = hour;

    /*
     * Write minute value to MIN register.
     * This sets the current minute in RTC.
     * Range: 0 to 59
     */
    MIN  = minute;

    /*
     * Write second value to SEC register.
     * This sets the current second in RTC.
     * Range: 0 to 59
     */
    SEC  = second;
}


/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCDateInfo
 * ------------------------------------------------------------
 * Sets RTC date registers
 */
void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
    /*
     * Write day of month into DOM register.
     * DOM = Day Of Month
     * Range: 1 to 31
     */
    DOM = date;

    /*
     * Write month value into MONTH register.
     * Range: 1 to 12
     */
    MONTH = month;

    /*
     * Write year value into YEAR register.
     * Typically 4-digit year (e.g., 2026)
     */
    YEAR = year;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: GetRTCDay
 * ------------------------------------------------------------
 * Reads day of week from RTC
 */
void GetRTCDay(s32 *dow)
{
    /*
     * Read the Day Of Week (DOW) register
     * and store the value into the memory location
     * pointed by 'dow'.
     *
	* *dow : Dereferencing pointer to store value
     */
    *dow = DOW;
}


/*
 * ------------------------------------------------------------
 * FUNCTION: DisplayRTCDay
 * ------------------------------------------------------------
 * Displays day (SUN–SAT) on LCD
 */
void DisplayRTCDay(u32 dow)
{
    /*
     * Move cursor to Line 1, position 10 on LCD.
     * GOTO_LINE1_POS0 ? starting position of line 1
     * +10 ? shifts cursor to desired column
     */
    CmdLCD(GOTO_LINE1_POS0 + 10);

    /*
     * Print the corresponding day string on LCD.
     * week[dow] ? selects string like "SUN", "MON", etc.
     * StrLCD() ? prints entire string to LCD
     */
    StrLCD(week[dow]);
}

/*
 * ------------------------------------------------------------
 * FUNCTION: SetRTCDay
 * ------------------------------------------------------------
 * Sets day of week in RTC
 */
void SetRTCDay(u32 dow)
{
    /*
     * Write the given day value into DOW register.
     * This sets the current day of the week in RTC.
     */
    DOW = dow;
}
