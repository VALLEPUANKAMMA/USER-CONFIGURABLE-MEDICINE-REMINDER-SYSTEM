/*
 * ============================================================
 * FILE: keypad_time.c
 * Author: Ankamma V
 * DESCRIPTION:
 * 4x4 Matrix Keypad Interface + Time Input using LCD
 * Target: LPC21xx / LPC2148
 *
 * FEATURES:
 * - Keypad scanning (Row + Column method)
 * - Key detection using lookup table
 * - Number reading from keypad
 * - Time setting with validation (HH:MM:SS)
 * ============================================================
 */

#include<lpc21xx.h>
#include"typedef.h"
#include"defines.h"
#include"kpm_defines.h"
#include"lcd1.h"
#include"delay.h"

/*
 * ------------------------------------------------------------
 * 4x4 Keypad Lookup Table
 * Maps row & column to actual key values
 * ------------------------------------------------------------
 */
u8 kpmLUT[4][4] =
{
    {'7','8','9','/'},
    {'4','5','6','*'},
    {'1','2','3','-'},
    {'C','0','=','+'}
};

/*
 * ------------------------------------------------------------
 * FUNCTION: Initkpm
 * ------------------------------------------------------------
 * Description:
 * Initializes keypad rows as output pins.
 * ------------------------------------------------------------
 */
void Initkpm(void)
{
    WNIBBLE(IODIR1, ROW0, 15);   /* Set ROW pins as output */
}

/*
 * ------------------------------------------------------------
 * FUNCTION: ColScan
 * ------------------------------------------------------------
 * Description:
 * Checks if any key is pressed by scanning columns.
 *
 * Returns:
 *   0 ? Key pressed
 *   1 ? No key pressed
 * ------------------------------------------------------------
 */
u32 ColScan(void)
{
    return ((RNIBBLE(IOPIN1, COL0) < 15) ? 0 : 1);
}

/*
 * ------------------------------------------------------------
 * FUNCTION: RowCheck
 * ------------------------------------------------------------
 * Description:
 * Identifies which row is active (key pressed).
 *
 * Returns:
 *   Row number (0–3)
 * ------------------------------------------------------------
 */
u32 RowCheck(void)
{
    u32 rno;

    for(rno = 0; rno < 4; rno++)
    {
        /* Activate one row at a time (LOW) */
        WNIBBLE(IOPIN1, ROW0, ~(1 << rno));

        if(ColScan() == 0)   /* Key detected */
        {
            break;
        }
    }

    /* Reset rows to default state */
    WNIBBLE(IOPIN1, ROW0, 0x0);

    return rno;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: ColCheck
 * ------------------------------------------------------------
 * Description:
 * Identifies which column has the pressed key.
 *
 * Returns:
 *   Column number (0–3)
 * ------------------------------------------------------------
 */
u32 ColCheck(void)
{
    u32 cno;

    for(cno = 0; cno < 4; cno++)
    {
        if(RBIT(IOPIN1, (COL0 + cno)) == 0)
        {
            break;
        }
    }

    return cno;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: keyScan
 * ------------------------------------------------------------
 * Description:
 * Detects key press and returns corresponding character.
 *
 * Steps:
 *   1. Wait for key press
 *   2. Identify row and column
 *   3. Get key from lookup table
 *   4. Wait for key release
 * ------------------------------------------------------------
 */
u32 keyScan(void)
{
    u32 rno, cno, keyV;

    while(ColScan());          /* Wait for press */

    rno = RowCheck();          /* Find row */
    cno = ColCheck();          /* Find column */

    keyV = kpmLUT[rno][cno];   /* Lookup key */

    while(!ColScan());         /* Wait for release */

    return keyV;
}

/*
 * ------------------------------------------------------------
 * FUNCTION: ReadNum
 * ------------------------------------------------------------
 * Description:
 * Reads numeric input from keypad and forms integer.
 *
 * Keys:
*   0–9 : Digits
*   '=' : End input
*   'C' : Clear screen
 * ------------------------------------------------------------
 */
u32 ReadNum(void)
{
    u8 key;
    u32 sum = 0;

    while(1)
    {
        key = keyScan();

        if(key >= '0' && key <= '9')
        {
            CharLCD(key);
            sum = (sum * 10) + (key - '0');
        }
        else if(key == '=')
        {
            break;
        }
        else if(key == 'C')
        {
            CmdLCD(0x01); /* Clear LCD */
        }
    }

    return sum;
}

/*
 * ------------------------------------------------------------
 * Buffer to store HHMMSS digits (ASCII)
 * ------------------------------------------------------------
 */
char arr1[6];

/*
 * ------------------------------------------------------------
 * FUNCTION: TimeSET
 * ------------------------------------------------------------
 * Description:
 * Reads time (HH:MM:SS) from keypad with validation.
 *
 * Features:
 *   - Input validation for each digit
 *   - Backspace support (C key)
 *   - Skip option (= key)
 *   - Exit option (* key)
 *
 * Parameters:
 *   hh : Pointer to store hours
 *   mm : Pointer to store minutes
 *   ss : Pointer to store seconds
 * ------------------------------------------------------------
 */
void TimeSET(s32 *hh, s32 *mm, s32 *ss)
{
    char key;
    int i = 0;

    while(i < 6)
    {
        /* Display prompts */
        if(i == 0)
        {
            StrLCD("Enter hour");
            CmdLCD(0xC0 + i);
        }
        else if(i == 2)
        {
            CmdLCD(0x80);
            StrLCD("Enter minutes");
            CmdLCD(0xC0 + i + 1);
        }
        else if(i == 4)
        {
            CmdLCD(0x80);
            StrLCD("Enter seconds");
            CmdLCD(0xC0 + i + 2);
        }

        key = keyScan();

        /* Exit */
        if(key == '*')
            break;

        /* Skip field */
        if(key == '=')
        {
            i += 2;
        }

        /* Backspace */
        else if(key == 'C')
        {
            if(i > 0)
            {
                i--;
                CmdLCD(0x10); /* Cursor left */

                if(i == 1 || i == 3)
                    CmdLCD(0x10);

                CharLCD(' ');
                CmdLCD(0x10);

                arr1[i] = 0;
            }
        }

        /* Hour first digit (0–2) */
        else if(i == 0)
        {
            if(key >= '0' && key <= '2')
            {
                arr1[i++] = key;
                CharLCD(key);
            }
        }

        /* Hour second digit */
        else if(i == 1)
        {
            if((arr1[0] == '2' && key <= '3') ||
               (arr1[0] != '2' && key <= '9'))
            {
                arr1[i++] = key;
                CharLCD(key);
                CharLCD(':');
            }
        }

        /* Minute/Second first digit (0–5) */
        else if(i == 2 || i == 4)
        {
            if(key >= '0' && key <= '5')
            {
                arr1[i++] = key;
                CharLCD(key);
            }
        }

        /* Minute/Second second digit (0–9) */
        else if(i == 3 || i == 5)
        {
            if(key >= '0' && key <= '9')
            {
                arr1[i++] = key;
                CharLCD(key);
                CharLCD(':');
            }
        }
    }

    /* Convert ASCII to integer */
    *hh = (arr1[0]-'0')*10 + (arr1[1]-'0');
    *mm = (arr1[2]-'0')*10 + (arr1[3]-'0');
    *ss = (arr1[4]-'0')*10 + (arr1[5]-'0');
}
