/*
 * ============================================================
 * FILE: main_test.c
 * Author: Ankamma V
 * PROJECT: Smart Medicine Reminder System
 * TARGET: LPC21xx / LPC2148
 *
 * DESCRIPTION:
 * This is the main program that integrates:
 *   - RTC (Real Time Clock)
 *   - LCD Display
 *   - Keypad (KPM)
 *   - External Interrupts
 *   - Buzzer Alert System
 *
 * FUNCTIONALITY:
 *   - Displays real-time clock (Time, Date, Day)
 *   - Allows user configuration via keypad
 *   - Triggers alerts based on medicine schedule
 *   - Uses interrupts for user interaction
 * ============================================================
 */

/* LPC21XX Register Definitions */
#include<lpc21xx.h>

/* RTC (Real Time Clock) Functions */
#include "rtc.h"

/* KPM (Keypad Matrix) Module Functions */
#include "kpm.h"

/* LCD (Liquid Crystal Display) Driver Functions */
#include "lcd1.h"

/* Interrupt Handling Functions */
#include "intrrupt.h"

/* LCD Commands and Macros */
#include "LCD_defines.h"

/* Main Function Declarations */
#include "main.h"

/*
 * ------------------------------------------------------------
 * FUNCTION: main
 * ------------------------------------------------------------
 * Description:
 * Entry point of the embedded system.
 *
 * Initializes all peripherals and runs the main loop
 * for real-time monitoring and control.
 * ------------------------------------------------------------
 */
int main()
{
    /*
     * Configure P0.24 as output for buzzer
     * IODIR0 ? GPIO direction register
     * 1 ? Output, 0 ? Input
     */
    IODIR0 = (1 << 24);

    /*
     * Initialize LCD Display
     */
    InitLCD();

    /*
     * Initialize Keypad Matrix Module
     */
    Initkpm();

    /*
     * Initialize External Interrupts (SW1, SW2)
     */
    InitInt();

    /*
     * Initialize Real Time Clock
     */
    RTC_Init();

    /*
     * Optional: Set initial RTC Date
     * Format: DD, MM, YYYY
     */
    // SetRTCDateInfo(05, 03, 2026);

    /*
     * Optional: Set initial RTC Time
     * Format: HH, MM, SS
     */
    // SetRTCTimeInfo(01, 59, 23);

    /*
     * --------------------------------------------------------
     * Infinite Loop (Main Application Loop)
     * --------------------------------------------------------
     */
    while(1)
    {
        /*
         * Continuously run RTC-based application
         * This function likely:
         *   - Reads current time from RTC
         *   - Displays time/date/day on LCD
         *   - Checks medicine schedule
         *   - Handles alerts and user input
         */
        RTC();
    }
}
