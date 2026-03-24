/*
 * File: LCD.c
 * Author: Ankamma V
 * Description:
 * ------------
 * This file contains driver functions for interfacing a 16x2 LCD
 * module (HD44780 controller) with LPC21xx microcontroller in 8-bit mode.
 *
 * ---------------------------------------------------------------
 * LCD SPECIFICATIONS
 * ---------------------------------------------------------------
 * Controller     : HD44780 (or compatible)
 * Display Type   : Alphanumeric LCD
 * Display Format : 16 Columns × 2 Rows (16x2)
 * Character Size : 5 × 8 Dot Matrix 
 */
#include<lpc21xx.h>        
#include "LCD_defines.h"  
#include "typedef.h"      
#include "defines.h"      
#include "delay.h"        

/*
 * Function: WriteLCD
 * ------------------
 * Writes data or command to LCD.
 * RW = 0 ? Write operation
 * EN pin is toggled to latch data into LCD
 */
void WriteLCD(u8 data)
{
    SCLRBIT(IOCLR0,RW);          
    WBYTE(IOPIN0,LCD_DATA,data); 
    SSETBIT(IOSET0,EN);          
    delay_us(1);                 
    SCLRBIT(IOCLR0,EN);          
    delay_ms(2);                 
}

/*
 * Function: CmdLCD
 * ----------------
 * Sends command to LCD.
 * RS = 0 ? Command register selected
 */
void CmdLCD(u8 cmd)
{
    SCLRBIT(IOCLR0,RS);   
    WriteLCD(cmd);        
}

/*
 * Function: CharLCD
 * -----------------
 * Sends a single character to LCD.
 * RS = 1 ? Data register selected
 */
void CharLCD(u8 ascii)
{
    SSETBIT(IOSET0,RS);   
    WriteLCD(ascii);      
}

/*
 * Function: InitLCD
 * -----------------
 * Initializes LCD in 8-bit mode.
 * Configures GPIO pins and sends initialization commands.
 */
void InitLCD(void)
{
    WBYTE(IODIR0,LCD_DATA,255); /* Set P0.8–P0.15 as OUTPUT (data lines) */
    SETBIT(IODIR0,RS);          /* Set RS pin as output */
    SETBIT(IODIR0,RW);          /* Set RW pin as output */
    SETBIT(IODIR0,EN);          /* Set EN pin as output */
    
    delay_ms(15);               /* Initial delay after power ON */

    /* Send initialization commands (as per LCD datasheet) */
    CmdLCD(MODE_8BIT_1LINE);
    delay_ms(5);
    CmdLCD(MODE_8BIT_1LINE);
    delay_us(100);
    CmdLCD(MODE_8BIT_1LINE);

    CmdLCD(MODE_8BIT_2LINE);    /* Set 2-line mode */
    CmdLCD(DISP_ON);            /* Display ON, cursor OFF */
    CmdLCD(CLEAR_LCD);          /* Clear display */
    CmdLCD(SHIFT_CUR_RIGHT);    /* Cursor moves right */
}

/*
 * Function: StrLCD
 * ----------------
 * Displays a string on LCD until NULL character.
 */
void StrLCD(u8* p)
{
    while(*p)
        CharLCD(*p++);
}

/*
 * Function: U32LCD
 * ----------------
 * Displays unsigned 32-bit integer on LCD.
 */
void U32LCD(u32 n)
{
    u8 a[10];   /* Array to store digits */
    s32 i=0;

    if(n==0)
    {
        CharLCD('0');
    }
    else
    {
        /* Extract digits in reverse order */
        while(n)
        {
            a[i++]=(n%10)+48; /* Convert digit to ASCII */
            n/=10;
        }

        /* Print digits in correct order */
        for(--i;i>=0;i--)
            CharLCD(a[i]);
    }
}

/*
 * Function: S32LCD
 * ----------------
 * Displays signed 32-bit integer on LCD.
 */
void S32LCD(s32 n)
{
    if(n<0)
    {
        CharLCD('-');  /* Print negative sign */
        n=-n;
    }
    U32LCD(n);        /* Print remaining number */
}

/*
 * Function: F32LCD
 * ----------------
 * Displays floating-point number with nDP decimal points.
 */
void F32LCD(f32 fnum, u8 nDP)
{
    u32 n;
    s32 i;

    if(fnum<0)
    {
        CharLCD('-'); /* Handle negative numbers */
    }

    n=fnum;           /* Extract integer part */
    U32LCD(n);        /* Print integer part */
    CharLCD('.');     /* Print decimal point */

    /* Print decimal digits */
    for(i=0; i<nDP; i++)
    {
        fnum=(fnum-n)*10;
        n=fnum;
        CharLCD(n+48);
    }
}

/*
 * Function: HEXLCD
 * ----------------
 * Displays hexadecimal value on LCD.
 */
void HEXLCD(u32 n)
{
    u8 a[8],rem;
    s32 i=0;

    if(n==0)
    {
        CharLCD('0');
    }
    else
    {
        /* Convert number to hex */
        while(n)
        {
            rem = n%16;
					(rem<10)?(rem+=48):(rem+=55); /* 0-9 : '0'-'9', 10-15 : 'A'-'F' */
            a[i++]=rem;
            n/=16;
        }

        /* Print in correct order */
        for(--i; i>=0; i--)
        {
            CharLCD(a[i]);
        }
    }
}

/*
 * Function: BinLCD
 * ----------------
 * Displays binary value with given number of bits (nbd).
 */
void BinLCD(u32 n, u8 nbd)
{
    s32 i;

    for(i=(nbd-1); i>=0; i--)
    {
        CharLCD(((n>>i)&1)+48); /* Extract each bit and convert to ASCII */
    }
}

/*
 * Function: BuildCGRAM
 * --------------------
 * Stores custom characters into LCD CGRAM.
 */
void BuildCGRAM(u8* p, u8 nb)
{
    s32 i;

    CmdLCD(GOTO_CGRAM);  /* Move to CGRAM address */

    for(i=0; i<=nb; i++)
    {
        CharLCD(p[i]);   /* Write pattern data */
    }

    CmdLCD(GOTO_LINE1_POS0); /* Return to normal display */
}
