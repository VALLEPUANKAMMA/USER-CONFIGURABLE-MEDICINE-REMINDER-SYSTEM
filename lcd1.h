/*
 * ============================================================
 * FILE: lcd1.h (Function Declarations)
 * Author:Ankamma V
 * DESCRIPTION:
 * LCD driver function prototypes for interfacing
 * 16x2 LCD with LPC21xx microcontroller.
 *
 * PURPOSE:
 * - Display characters, strings, numbers on LCD
 * - Provide abstraction for LCD operations
 * ============================================================
 */
#ifndef _LCD1_H
#define _LCD1_H
#include "typedef.h"

/*
 * ------------------------------------------------------------
 * WriteLCD:
 * Low-level function to send raw data/command to LCD.
 *
 * Parameter:
 *   data ? 8-bit data to be written
 *
 * NOTE:
 * Used internally by other LCD functions
 * ------------------------------------------------------------
 */
void WriteLCD(u8 data);

/*
 * ------------------------------------------------------------
 * CmdLCD:
 * Sends command to LCD (RS = 0)
 *
 * Parameter:
 *   data ? Command (e.g., clear display, cursor move)
 * ------------------------------------------------------------
 */
void CmdLCD(u8 data);

/*
 * ------------------------------------------------------------
 * CharLCD:
 * Displays a single character on LCD
 *
 * Parameter:
 *   ascii ? ASCII value of character
 * ------------------------------------------------------------
 */
void CharLCD(u8 ascii);

/*
 * ------------------------------------------------------------
 * InitLCD:
 * Initializes LCD (16x2)
 *
 * Operations:
 * - Set 8-bit / 4-bit mode
 * - Display ON
 * - Cursor settings
 * - Clear display
 * ------------------------------------------------------------
 */
void InitLCD(void);

/*
 * ------------------------------------------------------------
 * StrLCD:
 * Displays a string on LCD
 *
 * Parameter:
 *   p ? Pointer to string
 * ------------------------------------------------------------
 */
void StrLCD(char* p);

/*
 * ------------------------------------------------------------
 * U32LCD:
 * Displays unsigned 32-bit integer on LCD
 *
 * Parameter:
 *   n ? Number to display
 * ------------------------------------------------------------
 */
void U32LCD(u32 n);

/*
 * ------------------------------------------------------------
 * S32LCD:
 * Displays signed 32-bit integer on LCD
 *
 * Parameter:
 *   n ? Number to display (can be negative)
 * ------------------------------------------------------------
 */
void S32LCD(s32 n);

/*
 * ------------------------------------------------------------
 * F32LCD:
 * Displays floating point number
 *
 * Parameters:
 *   fnum ? Float number
 *   nDP  ? Number of decimal points
 * ------------------------------------------------------------
 */
void F32LCD(f32 fnum, u8 nDP);

/*
 * ------------------------------------------------------------
 * HEXLCD:
 * Displays number in hexadecimal format
 *
 * Parameter:
 *   n ? Number to display
 * ------------------------------------------------------------
 */
void HEXLCD(u32 n);

/*
 * ------------------------------------------------------------
 * BinLCD:
 * Displays number in binary format
 *
 * Parameters:
 *   n   ? Number to display
 *   nbd ? Number of bits to display
 * ------------------------------------------------------------
 */
void BinLCD(u32 n, u8 nbd);
#endif
