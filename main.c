/*==========================================================
   FILE NAME   : medicine_timer.c
	 Author: Ankamma V
   DESCRIPTION : Medicine Reminder System using LPC21xx
                 - Uses Keypad for input
                 - LCD for display
                 - RTC for time tracking
==========================================================*/


/*----------------------------------------------------------
   LPC21XX Register Definitions
   - Provides access to all Special Function Registers (SFRs)
   - Required to control hardware peripherals
----------------------------------------------------------*/
#include <lpc21xx.h>


/*----------------------------------------------------------
   Keypad Matrix (KPM) Header File
   - Contains functions for keypad scanning
   - Used to read user input (numbers, keys)
----------------------------------------------------------*/
#include "kpm.h"


/*----------------------------------------------------------
   LCD Header File
   - Functions to control LCD display
   - Used to show messages, time, medicine alerts
----------------------------------------------------------*/
#include "lcd1.h"


/*----------------------------------------------------------
   Interrupt Header File
   - Contains ISR (Interrupt Service Routine) declarations
   - Used for handling external/internal interrupts
----------------------------------------------------------*/
#include "intrrupt.h"


/*----------------------------------------------------------
   RTC (Real Time Clock) Header File
   - Used to get and set current time and date
   - Important for medicine scheduling
----------------------------------------------------------*/
#include "rtc.h"


/*----------------------------------------------------------
   LCD Command Definitions
   - Contains command macros like clear, cursor move
----------------------------------------------------------*/
#include "LCD_defines.h"


/*----------------------------------------------------------
   Delay Functions
   - Software delay functions (ms, us)
   - Used for timing control in LCD and keypad
----------------------------------------------------------*/
#include "delay.h"


/*==========================================================
   FUNCTION PROTOTYPES
==========================================================*/

/*----------------------------------------------------------
   Function: User_Day_Set
   Description:
   - Used to set the day of the month (1–31)
   - Takes pointer to store the entered day value
----------------------------------------------------------*/
void User_Day_Set(s32 *day1);


/*----------------------------------------------------------
   Function: User_Date_set
   Description:
   - Used to set full date (day, month, year)
   - Values are stored using pointers
----------------------------------------------------------*/
void User_Date_set(s32 *day1, s32 *month1, s32 *year1);


/*==========================================================
   GLOBAL VARIABLES
==========================================================*/

/*----------------------------------------------------------
   Array: arr[60]
   Description:
   - Stores medicine timing data
   - Each element can represent a time or event
   - Size = 50 ? can store up to 20 timings(HH:MM:SS)

   Example:
   arr[0] = 09;{ It Stores the Hour(00-23)}
	 arr[1]=  55;(00-59){ It Stores the Minute (00-59) }
   arr[1] = 35;(00-59){ It Stores the second (00-59) }
----------------------------------------------------------*/
s32 arr[60];
/*==========================================================
   GLOBAL FLAG VARIABLES
==========================================================*/

/*----------------------------------------------------------
   res   : Stores result values (general purpose variable)
   flag  : Indicates whether medicine time is set or not
           0 ? Not set
           1 ? Set

   flag1 : Used to check whether Interrupt 2 (Buzzer) is triggered
           0 ? Not triggered
           1 ? Triggered

   found : Checks whether entered medicine time already exists
           (used to avoid duplicate entries)

   found1: Checks whether Interrupt 1 is triggered

   fud   : Indicates multiple medicine timings entry
           0 ? Single timing
           1 ? Multiple timings entered
----------------------------------------------------------*/
u32 res;
u32 flag  = 0;
u32 flag1 = 0;
u32 found = 0;
u32 found1= 0;
u32 fud   = 0;


/*==========================================================
   TIME VARIABLES (RTC RELATED)
==========================================================*/

/*----------------------------------------------------------
   hh : Stores Hour value   (0–23)
   mm : Stores Minute value (0–59)
   ss : Stores Second value (0–59)
----------------------------------------------------------*/
s32 hh = 0;
s32 mm = 0;
s32 ss = 0;


/*==========================================================
   MEDICINE COUNT VARIABLE
==========================================================*/

/*----------------------------------------------------------
   n : Stores number of medicine timings entered
       Example:
       n = 1 ? One medicine time
       n = 3 ? Three medicine timings
----------------------------------------------------------*/
u8 n;


/*==========================================================
   DATE VARIABLES (RTC RELATED)
==========================================================*/

/*----------------------------------------------------------
   date1  : Stores Date of Month (1–31)
   month1 : Stores Month (1–12)
   year1  : Stores Year (e.g., 2026)
   day1   : Stores Day of Week (1–7)
----------------------------------------------------------*/
s32 date1  = 0;
s32 month1 = 0;
s32 year1  = 0;
s32 day1;
/*==========================================================
   FUNCTION: MedicineSet1
   DESCRIPTION:
   - Sets first medicine timing
   - Stores HH, MM, SS in arr[0], arr[1], arr[2]
==========================================================*/
void MedicineSet1()
{
    /* Indicate medicine timing is set */
    flag = 1;

    /* Clear LCD and display input format */
    CmdLCD(0x01);
    StrLCD("Time HH:MM:SS");

    /* Get time input from user (Call by reference) */
    TimeSET(&arr[0], &arr[1], &arr[2]);

    /* Clear LCD and show success message */
    CmdLCD(0x01);
    StrLCD("Saved 1");

    /* Small delay to view message */
    delay_ms(400);

    /* Clear LCD */
    CmdLCD(0x01);
}


/*==========================================================
   FUNCTION: MedicineSet2
   DESCRIPTION:
   - Sets second medicine timing
   - Avoids duplicate with first timing
==========================================================*/
void MedicineSet2()
{
    do
    {
        found = 0;   // Reset flag before checking

        CmdLCD(0x01);
        StrLCD("Time HH:MM:SS");

        /* Get time input */
        TimeSET(&arr[3], &arr[4], &arr[5]);

        CmdLCD(0x01);

        /* Check duplicate with first timing */
        if (arr[0] == arr[3] &&
            arr[1] == arr[4] &&
            arr[2] == arr[5])
        {
            found = 1;

            StrLCD("Same Timings");
            delay_ms(400);

            CmdLCD(0x01);
            StrLCD("Re-Enter");
            delay_ms(400);
        }

    } while (found == 1);

    /* Save success message */
    CmdLCD(0x01);
    StrLCD("Saved 2");
    delay_ms(400);

    CmdLCD(0x01);
}


/*==========================================================
   FUNCTION: MedicineSet3
   DESCRIPTION:
   - Sets third medicine timing
   - Avoids duplicate with previous timings
==========================================================*/
void MedicineSet3()
{
    do
    {
        found = 0;   // Reset flag

        CmdLCD(0x01);
        StrLCD("Time HH:MM:SS");

        /* Get time input */
        TimeSET(&arr[6], &arr[7], &arr[8]);

        CmdLCD(0x01);

        /* Check duplicate with second timing */
        if (arr[3] == arr[6] &&
            arr[4] == arr[7] &&
            arr[5] == arr[8])
        {
            found = 1;

            StrLCD("Same Timings");
            delay_ms(400);

            CmdLCD(0x01);
            StrLCD("Re-Enter");
            delay_ms(400);
        }

    } while (found == 1);

    /* Success message */
    CmdLCD(0x01);
    StrLCD("Saved 3");
    delay_ms(400);

    CmdLCD(0x01);
}

/*==========================================================
   ISR: EINT0 (External Interrupt 0)
   DESCRIPTION:
   - Triggered when user presses button (menu key)
   - Used to display menu or enter settings mode
==========================================================*/
void eint0_isr(void) __irq
{
	/* Set flag : indicates interrupt occurred */
    found1 = 1;

	/* CmdLCD is a Function To Pass LCD Commands
	Ex:0x01 Command For Clear Display	*/
    CmdLCD(0x01);

    /* Clear interrupt flag for EINT0
       (Write 1 to clear) */
    EXTINT = (1 << 0);

    /* Acknowledge interrupt to VIC */
    VICVectAddr = 0;
}


/*==========================================================
   ISR: EINT1 (External Interrupt 1)
   DESCRIPTION:
   - Triggered to stop buzzer (medicine alert)
   - User presses button to acknowledge alarm
==========================================================*/
void eint1_isr(void) __irq
{
    /* Set flag ? buzzer interrupt triggered */
    flag1 = 1;

    /* Turn OFF buzzer (assuming P0.7 connected) */
    IOCLR0 = (1 << 7);

	/* CmdLCD is a Function To Pass LCD Commands
	Ex:0x01 Command For Clear Display	*/
    CmdLCD(0x01);

    /* Clear interrupt flag for EINT1 */
    EXTINT = (1 << 1);

    /* Acknowledge interrupt to VIC */
    VICVectAddr = 0;
}
/*==========================================================
   FUNCTION: Compare_Medicine1
   DESCRIPTION:
   - Compares RTC time with Medicine Timing 1
   - arr[0], arr[1], arr[2] : HH, MM, SS
   - Returns:
1 : Match found
0 : No match
==========================================================*/
u32 Compare_Medicine1(u32 a, u32 b, u32 c)
{
    if (a == arr[0] && b == arr[1] && c == arr[2])
        return 1;

    return 0;
}


/*==========================================================
   FUNCTION: Compare_Medicine2
   DESCRIPTION:
   - Compares RTC time with Medicine Timing 2
==========================================================*/
u32 Compare_Medicine2(u32 a, u32 b, u32 c)
{
    if (a == arr[3] && b == arr[4] && c == arr[5])
        return 1;

    return 0;
}


/*==========================================================
   FUNCTION: Compare_Medicine3
   DESCRIPTION:
   - Compares RTC time with Medicine Timing 3
==========================================================*/
u32 Compare_Medicine3(u32 a, u32 b, u32 c)
{
    if (a == arr[6] && b == arr[7] && c == arr[8])
        return 1;

    return 0;
}
/*==========================================================
   FUNCTION: RtcTimeUpdate
   DESCRIPTION:
   - Triggered when interrupt flag (found1) is set
   - Displays menu for:
        1. RTC Edit
        2. Medicine Timing
        3. Exit
==========================================================*/
void RtcTimeUpdate(void)
{
    if (found1 == 1)
    {
			/* Reset interrupt flag Found */
        found1 = 0;  
/*Display Menu CmdLCD is a Function To PassCommands
	GOTO_LINE1_POS0 (0x80) Is Macro For Cursor Moves On LCD First Position */
        CmdLCD(GOTO_LINE1_POS0);
/* StrLCD it's a Function to Display the String On LCD Size 16*2			*/ 
        StrLCD("1.RtcEdit 3.Exit");
	/* CmdLCD is a Function To Pass LCD Commands
	Ex:0xC0 Command For GOTO_LINE2_POS0 On Display	*/
        CmdLCD(GOTO_LINE2_POS0);
/* StrLCD it's a Function to Display the String On LCD Size 16*2			*/ 
        StrLCD("2.Medicine");
/* Switch Case For Which Option to be selected 
			keyScan() Funtion For scaning the Key form Keypad */
        switch (keyScan())
        {
            /*================ RTC TIME UPDATE ================*/
            case '1':
				/* CmdLCD is a Function To Pass LCD Commands
						Ex:0x01 Command For Clear Display	*/
                CmdLCD(0x01);
                /* Set Day Of Week */
                User_Day_Set(&day1);
						/* Set Day Of Month,Month, Year */
                User_Date_set(&date1, &month1, &year1);
				/* CmdLCD is a Function To Pass LCD Commands
						Ex:0x01 Command For Clear Display	*/
                CmdLCD(0x01);
/* StrLCD it's a Function to Display the String On LCD Size 16*2 */ 						
                StrLCD("Rtc HH:MM:SS");
						/* Delay Function for Software delay to See display message on LCD */
                delay_ms(400);
				/* CmdLCD is a Function To Pass LCD Commands
						Ex:0x01 Command For Clear Display	*/
                CmdLCD(0x01);
						/* TimeSET it's User Define Function to Get time input For Update the Time on RTC */
                TimeSET(&hh, &mm, &ss);
                /* Save RTC on SFR's */
                SetRTCTimeInfo(hh, mm, ss);
								/* SetRTCDateInfo is a user define function to update date,month,year */
                SetRTCDateInfo(date1, month1, year1);

        /* CmdLCD is a Function To Pass LCD Commands
						  Ex:0x01 Command For Clear Display	*/
                CmdLCD(0x01);
				/* StrLCD function to display RTC Saved */
                StrLCD("RTC Saved");
				/* Delay Function for Software delay to See display message on LCD */
								delay_ms(400);
        /* CmdLCD is a Function To Pass LCD Commands
						  Ex:0x01 Command For Clear Display	*/
                CmdLCD(0x01);
                break;
case '2':
{
    /*
    -----------------------------------------------------------
    MEDICINE SETTING MODE
    -----------------------------------------------------------
    - This case is selected when user presses '2'
    - Allows user to set up to 3 medicine timings
    - First medicine is mandatory
    - Second and third medicines are optional
    -----------------------------------------------------------
    */

    CmdLCD(0x01);

    /*
    Clear LCD display
    - Removes previous menu or data
    - Cursor returns to home position
    */
    
    MedicineSet1();

    /*
    Set FIRST medicine timing
    - User enters time (HH:MM:SS)
    - Stored in arr[0], arr[1], arr[2]
    */

    CmdLCD(0x01);

    /*
    Clear LCD before asking user input
    */

    StrLCD("Add More? 1=Yes");

    /*
    Display message:
    - Ask user if they want to add another medicine
    - Press '1' for YES
    */

    n = keyScan();

    /*
    Read keypad input
    - Stores pressed key in variable 'n'
    */

    if (n == '1')
    {
        /*
        If user selects YES:
        - Enable flag for additional medicine entries
        */

        fud = 1;

        /*
        'fud' flag indicates that multiple medicines are being added
        */

        MedicineSet2();

        /*
        Set SECOND medicine timing
        - Stored in arr[3], arr[4], arr[5]
        */
    }

    if (fud == 1)
    {
        /*
        Check if second medicine was added
        - Only then allow third medicine entry
        */

        CmdLCD(0x01);

        /*
        Clear LCD again for next prompt
        */

        StrLCD("Add More? 1=Yes");

        /*
        Ask user again for adding third medicine
        */

        n = keyScan();

        /*
        Read keypad input again
        */

        if (n == '1')
        {
            MedicineSet3();

            /*
            Set THIRD medicine timing
            - Stored in arr[6], arr[7], arr[8]
            */
        }
    }

    CmdLCD(0x01);

    /*
    Final LCD clear
    - Clean screen before exiting this case
    */

    break;

    /*
    Exit from switch-case
    - Return to main program flow
    */
		// Exit from switch-case
		/*================ EXIT =================*/
            case '3':
                CmdLCD(0x01);
                return;
        }
    }
				CmdLCD(0x01);
	}
}
/*==========================================================
   FUNCTION: RTC
   DESCRIPTION:
   - Reads RTC time/date
   - Displays on LCD
   - Compares with medicine timings
   - Triggers buzzer if match found
==========================================================*/
void RTC(void)
{
    /*
    -----------------------------------------------------------
    RTC MAIN FUNCTION
    -----------------------------------------------------------
    - Handles RTC update, display, and medicine alert system
    - Works continuously in main loop
    - Performs:
        1. Menu handling (interrupt-based)
        2. RTC time/date/day reading
        3. Display on LCD
        4. Compare with stored medicine timings
        5. Trigger alarm (buzzer + message)
    -----------------------------------------------------------
    */

    /*---------------------------------------------------------
      HANDLE USER MENU (RTC EDIT / MEDICINE SETTING)
    ---------------------------------------------------------*/
    RtcTimeUpdate();

    /*
    - Checks interrupt flag (found1)
    - Allows user to edit RTC or set medicine timings
    */


    /*---------------------------------------------------------
      READ CURRENT RTC VALUES
    ---------------------------------------------------------*/
    GetRTCTimeInfo(&hh, &mm, &ss);
    GetRTCDateInfo(&date1, &month1, &year1);
    GetRTCDay(&day1);

    /*
    - hh  : Hours
    - mm  : Minutes
    - ss  : Seconds
    - date1, month1, year1 : Date
    - day1 : Day of week
    */


    /*---------------------------------------------------------
      DISPLAY RTC INFORMATION ON LCD
    ---------------------------------------------------------*/
    DisplayRTCTime(hh, mm, ss);
    DisplayRTCDay(day1);
    DisplayRTCDate(date1, month1, year1);

    /*
    - Shows time, day, and date on LCD
    - Keeps updating continuously
    */


    /*---------------------------------------------------------
      COMPARE CURRENT TIME WITH MEDICINE TIMINGS
    ---------------------------------------------------------*/
    res = Compare_Medicine1(hh, mm, ss);

    if (!res)
        res = Compare_Medicine2(hh, mm, ss);

    if (!res)
        res = Compare_Medicine3(hh, mm, ss);

    /*
    - Compare RTC time with stored medicine times
    - Stops checking once a match is found
    - res = 1 ? match found
    - res = 0 ? no match
    */


    /*---------------------------------------------------------
      IF MATCH FOUND ? TRIGGER MEDICINE ALERT
    ---------------------------------------------------------*/
    if (flag == 1 && res == 1)
    {
        int cnt = 0;

        /*
        flag == 1 ? ensures alarm triggers only once
        res  == 1 ? time matches medicine schedule
        */

        flag = 0;   // prevent repeated triggering
        res = 0;    // reset comparison result

        CmdLCD(0x01);
        StrLCD("Take Medicine");

        /*
        Display alert message on LCD
        */

        /*-----------------------------------------------------
          TURN ON BUZZER
        -----------------------------------------------------*/
        IOSET0 = (1 << 24);

        /*
        - Sets GPIO pin P0.24 HIGH
        - Activates buzzer
        */


        /*-----------------------------------------------------
          WAIT LOOP (MAX 60 SECONDS OR USER STOP)
        -----------------------------------------------------*/
        while (1)
        {
            delay_s(1);
            cnt++;

            /*
            - Waits 1 second each loop
            - Counts total duration
            */

            /* Stop conditions */
            if (flag1 == 1 || cnt >= 60)
                break;

            /*
            flag1 == 1 ? user pressed stop button
            cnt >= 60 ? timeout (60 seconds)
            */
        }


        /*-----------------------------------------------------
          TURN OFF BUZZER
        -----------------------------------------------------*/
        IOCLR0 = (1 << 24);

        /*
        - Clears GPIO pin P0.24
        - Deactivates buzzer
        */

        CmdLCD(0x01);

        /*
        Clear LCD after alert
        */

        if (cnt >= 60)
            StrLCD("Not Taken");

        /*
        If timeout occurs (user didn’t respond)
        - Display "Not Taken"
        */

        delay_ms(500);

        CmdLCD(0x01);

        /*
        Final LCD clear before returning
        */
    }
}
/*
    -----------------------------------------------------------
    FUNCTION: User_Day_Set
    -----------------------------------------------------------
    - Allows user to select day of the week using keypad
    - Displays days on LCD
    - Navigation:
        '=' ? Next day
        'C' ? Previous day
        '*' ? Confirm selection
    - Selected day is stored in RTC
    -----------------------------------------------------------
 */
void User_Day_Set(s32 *day1)
{
    /*---------------------------------------------------------
      ARRAY OF WEEK DAYS
    ---------------------------------------------------------*/
    u8 msg[][6] = {
        "1:SUN","2:MON","3:TUE",
        "4:WED","5:THU","6:FRI","7:SAT"
    };

    /*
    msg[0] ? Sunday
    msg[1] ? Monday
    ...
    msg[6] ? Saturday
    */

    int key;

    /*---------------------------------------------------------
      INITIAL VALUE VALIDATION
    ---------------------------------------------------------*/
    if (*day1 < 0 || *day1 > 6)
        *day1 = 0;

    /*
    - Ensures day index is within valid range (0–6)
    - If invalid, default to Sunday
    */


    /*---------------------------------------------------------
      MAIN LOOP FOR USER INPUT
    ---------------------------------------------------------*/
    while (1)
    {
        /* Display selected day on LCD */
        CmdLCD(0x80);
        StrLCD(msg[*day1]);

        /*
        - 0x80 ? Cursor at first line, position 0
        - Displays current selected day
        */

        key = keyScan();

        /*
        Read keypad input from user
        */

        /*-----------------------------------------------------
          CONFIRM SELECTION
        -----------------------------------------------------*/
        if (key == '*')
        {
            break;
        }

        /*
        - '*' key confirms selected day
        - Exit loop
        */


        /*-----------------------------------------------------
          NEXT DAY SELECTION
        -----------------------------------------------------*/
        else if (key == '=')
        {
            (*day1)++;

            if (*day1 >= 7)
                *day1 = 0;
        }

        /*
        - '=' key moves forward
        - Wrap-around:
            Saturday ? Sunday
        */


        /*-----------------------------------------------------
          PREVIOUS DAY SELECTION
        -----------------------------------------------------*/
        else if (key == 'C')
        {
            if (*day1 == 0)
                *day1 = 6;
            else
                (*day1)--;
        }

        /*
        - 'C' key moves backward
        - Wrap-around:
            Sunday ? Saturday
        */
    }


    /*---------------------------------------------------------
      SAVE SELECTED DAY TO RTC
    ---------------------------------------------------------*/
    SetRTCDay(*day1);

    /*
    - Updates RTC register with selected day
    */


    /*---------------------------------------------------------
      DISPLAY CONFIRMATION MESSAGE
    ---------------------------------------------------------*/
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Day Saved");
    delay_ms(500);

    /*
    - Shows confirmation to user
    - Delay allows user to read message
    */


    /*---------------------------------------------------------
      CLEAR LCD
    ---------------------------------------------------------*/
    CmdLCD(0x01);

    /*
    - Clears display before returning
    */
}
/*==========================================================
   FUNCTION: User_Date_set
   DESCRIPTION:
   - User enters date in format DD/MM/YYYY
   - Supports:
'=' : skip field
'C' : backspace
'*' : confirm with prvious data (or) Skip The Entire Function
==========================================================*/
u8 arr2[8]={0};
void User_Date_set(s32 *date1, s32 *month1, s32 *year1)
{
    /*
    -----------------------------------------------------------
    FUNCTION: User_Date_set
    -----------------------------------------------------------
    - Allows user to enter Date in format: DD/MM/YYYY
    - Uses keypad for input
    - Supports:
        '*' ? Exit / Confirm
        '=' ? Skip field
        'C' ? Backspace
    - Stores values in global buffer arr2[8]
    - Converts ASCII input into integer values
    -----------------------------------------------------------
    */

    char key;
    int i = 0, k;

    /*---------------------------------------------------------
      CLEAR INPUT BUFFER
    ---------------------------------------------------------*/
    for (k = 0; k < 8; k++)
        arr2[k] = 0;

    /*
    - arr2 stores user input characters
    - Clearing prevents garbage values
    */


    /*---------------------------------------------------------
      DISPLAY FORMAT ON LCD
    ---------------------------------------------------------*/
    CmdLCD(0x01);
    StrLCD("DD/MM/YYYY");
    CmdLCD(GOTO_LINE2_POS0);

    /*
    - First line shows format
    - Second line used for user input
    */


    /*---------------------------------------------------------
      INPUT LOOP (MAX 8 DIGITS)
    ---------------------------------------------------------*/
    while (i < 8)
    {
        key = keyScan();

        /*
        Read keypad input from user
        */


        /*-----------------------------------------------------
          EXIT / CONFIRM INPUT
        -----------------------------------------------------*/
        if (key == '*')
            break;

        /*
        - '*' ends input early
        */


        /*-----------------------------------------------------
          SKIP FIELD
        -----------------------------------------------------*/
        if (key == '=')
        {
            if (i == 0 || i == 2)
                i += 2;
            else if (i == 4)
                i += 4;

            continue;
        }

        /*
        - Skip DATE ? move to MONTH
        - Skip MONTH ? move to YEAR
        - Skip YEAR ? exit
        */


        /*-----------------------------------------------------
          BACKSPACE FUNCTION
        -----------------------------------------------------*/
        if (key == 'C')
        {
            if (i > 0)
            {
                i--;

                CmdLCD(0x10);  // Move cursor left

                if (i == 1 || i == 3)
                    CmdLCD(0x10); // Skip '/'

                CharLCD(' ');   // Erase character
                CmdLCD(0x10);   // Move back again

                arr2[i] = 0;    // Clear buffer
            }
            continue;
        }

        /*
        - Deletes previous character
        - Handles '/' position correctly
        */


        /*================ DATE (DD) =================*/

        if (i == 0)
        {
            if (key >= '0' && key <= '3')
            {
                arr2[i++] = key;
                CharLCD(key);
            }
        }
        else if (i == 1)
        {
            if ((arr2[0] == '3' && key <= '1') ||
                (arr2[0] != '3' && key <= '9'))
            {
                arr2[i++] = key;
                CharLCD(key);
                CharLCD('/');
            }
        }

        /*
        - Valid range: 01–31
        - Automatically adds '/'
        */


        /*================ MONTH (MM) =================*/

        else if (i == 2)
        {
            if (key >= '0' && key <= '1')
            {
                arr2[i++] = key;
                CharLCD(key);
            }
        }
        else if (i == 3)
        {
            if ((arr2[2] == '1' && key <= '2') ||      // 10–12
                (arr2[2] == '0' && key >= '1' && key <= '9')) // 01–09
            {
                arr2[i++] = key;
                CharLCD(key);
                CharLCD('/');
            }
        }

        /*
        - Valid range: 01–12
        - Automatically adds '/'
        */


        /*================ YEAR (YYYY) =================*/

        else if (i >= 4 && i < 8)
        {
            if (i == 4 && key != '2')   // Force 20xx format
                continue;

            if (key >= '0' && key <= '9')
            {
                arr2[i++] = key;
                CharLCD(key);
            }
        }

        /*
        - Accepts 4-digit year
        - Restricts to 2000–2099
        */


        /*-----------------------------------------------------
          DEBOUNCE DELAY
        -----------------------------------------------------*/
        delay_ms(150);

        /*
        - Prevents multiple key detections
        */
    }


    /*---------------------------------------------------------
      CONVERT ASCII TO INTEGER
    ---------------------------------------------------------*/
    *date1  = (arr2[0]-'0')*10 + (arr2[1]-'0');
    *month1 = (arr2[2]-'0')*10 + (arr2[3]-'0');
    *year1  = (arr2[4]-'0')*1000 +
              (arr2[5]-'0')*100 +
              (arr2[6]-'0')*10 +
              (arr2[7]-'0');

    /*
    - Converts character digits to numeric values
    - Example:
		'2','5' : 25
		'2','0','2','6' : 2026
    */


    /*---------------------------------------------------------
      CLEAR LCD AFTER INPUT
    ---------------------------------------------------------*/
    CmdLCD(0x01);

    /*
    - Clears screen before returning
    */
}
