/*
 * ============================================================
 * KEYPAD MATRIX PIN DEFINITIONS
 * ============================================================
 * FILE NAME:kpm_defines.h
 * Author: Ankamma V
 * DESCRIPTION:
 * Defines GPIO pin connections for a 4x4 keypad matrix.
 * Rows are connected to Port1 pins 16–19
 * Columns are connected to Port1 pins 20–23
 *
 * HARDWARE:
 * - Microcontroller: LPC21xx (ARM7)
 * - Keypad: 4x4 Matrix Keypad
 *
 * NOTE:
 * Rows are typically configured as OUTPUT
 * Columns are configured as INPUT
 * ============================================================
 */
#ifndef _KPM_DEFINES_
#define _KPM_DEFINES_
/*
 * ------------------------------------------------------------
 * ROW DEFINITIONS (OUTPUT PINS)
 * Each row is driven LOW one at a time during scanning
 * ------------------------------------------------------------
 */
#define ROW0 16   // Connected to P1.16
#define ROW1 17   // Connected to P1.17
#define ROW2 18   // Connected to P1.18
#define ROW3 19   // Connected to P1.19

/*
 * ------------------------------------------------------------
 * COLUMN DEFINITIONS (INPUT PINS)
 * Each column is read to detect key press
 * ------------------------------------------------------------
 */
#define COL0 20   // Connected to P1.20
#define COL1 21   // Connected to P1.21
#define COL2 22   // Connected to P1.22
#define COL3 23   // Connected to P1.23
#endif
