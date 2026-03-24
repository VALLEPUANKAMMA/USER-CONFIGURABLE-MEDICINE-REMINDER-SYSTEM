/*
 * ============================================================
 * FILE: LCD_defines.h
 * Author: Ankamma V
 * DESCRIPTION:
 * Macros for LCD pin connections and command definitions
 * for 16x2 (or 20x4) character LCD.
 *
 * PURPOSE:
 * - Simplify LCD interfacing
 * - Provide readable command names
 * - Used with LPC21xx microcontroller
 * ============================================================
 */
#ifndef _LCD_DEFINES_
#define _LCD_DEFINES_
/*
 * ------------------------------------------------------------
 * LCD DATA PIN START POSITION
 * LCD data lines (D0–D7 or D4–D7 in 4-bit mode)
 * are connected starting from this bit position
 * ------------------------------------------------------------
 */
#define LCD_DATA 8   // Data lines connected from P0.8

/*
 * ------------------------------------------------------------
 * LCD CONTROL PINS
 * ------------------------------------------------------------
 */
#define RS 16   // Register Select (0 = Command, 1 = Data)
#define RW 17   // Read/Write (0 = Write, 1 = Read)
#define EN 18   // Enable pin (High ? Low pulse)

/*
 * ------------------------------------------------------------
 * LCD COMMANDS
 * ------------------------------------------------------------
 */

/* Clear entire display */
#define CLEAR_LCD 0X01

/* Return cursor to home position */
#define RET_CUR_HOME 0X02

/* Display OFF */
#define DISP_OFF 0X08

/* Display ON, cursor OFF */
#define DISP_ON 0X0C

/* Display ON, cursor ON */
#define DISP_ON_CUR_ON 0X0E

/* Display ON, cursor blinking */
#define DISP_ON_CUR_BLINK 0X0F

/*
 * ------------------------------------------------------------
 * LCD MODE SELECTION
 * ------------------------------------------------------------
 */

/* 8-bit mode, single line */
#define MODE_8BIT_1LINE 0X30

/* 8-bit mode, 2 lines */
#define MODE_8BIT_2LINE 0X38

/* 4-bit mode, single line */
#define MODE_4BIT_1LINE 0X20

/* 4-bit mode, 2 lines */
#define MODE_4BIT_2LINE 0X28

/*
 * ------------------------------------------------------------
 * CURSOR POSITION COMMANDS
 * ------------------------------------------------------------
 */

/* Go to Line 1, Position 0 */
#define GOTO_LINE1_POS0 0X80

/* Go to Line 2, Position 0 */
#define GOTO_LINE2_POS0 0XC0

/* Go to Line 3 (for 20x4 LCD) */
#define GOTO_LINE3_POS0 0X94

/* Go to Line 4 (for 20x4 LCD) */
#define GOTO_LINE4_POS0 0XD4

/*
 * ------------------------------------------------------------
 * CURSOR / DISPLAY SHIFT COMMANDS
 * ------------------------------------------------------------
 */

/* Shift cursor to right */
#define SHIFT_CUR_RIGHT 0X06

/* Shift cursor to left */
#define SHIFT_CUR_LEFT 0X10

/* Shift entire display to right */
#define SHIFT_DISP_RIGHT 0X14

/*
 * ------------------------------------------------------------
 * CGRAM (Custom Character Generator RAM)
 * Used to create custom characters
 * ------------------------------------------------------------
 */
#define GOTO_CGRAM 0X40
#endif
