/*
 * ============================================================
 * FILE: kpm.h (Keypad Matrix Function Declarations)
 * Author:Ankamma V
 * DESCRIPTION:
 * Function prototypes for interfacing a 4x4 keypad matrix
 * with LPC21xx microcontroller.
 *
 * PURPOSE:
 * - Detect key press
 * - Scan rows and columns
 * - Read numeric input from keypad
 * - Set time using keypad input
 * ============================================================
 */
#ifndef _KPM_
#define _KPM_
#include "typedef.h"

/*
 * ------------------------------------------------------------
 * Initkpm:
 * Initializes keypad hardware
 *
 * Operations:
 * - Configure row pins as OUTPUT
 * - Configure column pins as INPUT
 * ------------------------------------------------------------
 */
void Initkpm(void);

/*
 * ------------------------------------------------------------
 * ColScan:
 * Checks if any key is pressed (column detection)
 *
 * Returns:
 *   0 ? Key pressed
 *   1 ? No key pressed
 *
 * NOTE:
 * Used for initial key press detection
 * ------------------------------------------------------------
 */
u32 ColScan(void);

/*
 * ------------------------------------------------------------
 * RowCheck:
 * Identifies which row the key is pressed in
 *
 * Returns:
 *   Row number (0 to 3)
 *
 * Operation:
 * - Drives each row LOW one by one
 * - Checks column response
 * ------------------------------------------------------------
 */
u32 RowCheck(void);

/*
 * ------------------------------------------------------------
 * ColCheck:
 * Identifies which column the key is pressed in
 *
 * Returns:
 *   Column number (0 to 3)
 * ------------------------------------------------------------
 */
u32 ColCheck(void);

/*
 * ------------------------------------------------------------
 * keyScan:
 * Complete key detection function
 *
 * Returns:
 *   Key value (character from keypad)
 *
 * Operation:
 * - Waits for key press
 * - Finds row and column
 * - Returns corresponding key using lookup table
 * ------------------------------------------------------------
 */
u32 keyScan(void);

/*
 * ------------------------------------------------------------
 * ReadNum:
 * Reads numeric input from keypad
 *
 * Returns:
 *   Integer value entered by user
 *
 * Operation:
 * - Accepts digits (0–9)
 * - '=' ? End input
 * - 'C' ? Clear input
 * ------------------------------------------------------------
 */
u32 ReadNum(void);

/*
 * ------------------------------------------------------------
 * TimeSET:
 * Used to set time (HH:MM:SS) using keypad
 *
 * Parameters:
 *   hh ? Pointer to store hours
 *   mm ? Pointer to store minutes
 *   ss ? Pointer to store seconds
 *
 * Operation:
 * - Accepts validated time input
 * - Displays on LCD
 * - Performs range checking
 * ------------------------------------------------------------
 */
void TimeSET(s32 *hh,s32 *mm,s32 *ss);
#endif
