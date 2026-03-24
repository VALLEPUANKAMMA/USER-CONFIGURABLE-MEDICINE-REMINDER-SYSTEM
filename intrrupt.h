/*
 * ============================================================
 * FILE: intrrupt.h (Interrupt Function Declarations)
 * Author:Ankamma V
 * DESCRIPTION:
 * Function prototypes for external interrupt handling
 * in LPC21xx microcontroller.
 *
 * PURPOSE:
 * - Initialize and configure external interrupts
 * - Handle interrupt service routines (ISR)
 * ============================================================
 */
 #ifndef _INT_
 #define _INT_
/*
 * ------------------------------------------------------------
 * InitInt:
 * Initializes external interrupts (EINT0, EINT1)
 *
 * Operations:
 * - Configure interrupt pins (PINSEL)
 * - Enable interrupts in VIC
 * - Assign ISR addresses
 * - Set interrupt triggering mode (edge/level)
 * ------------------------------------------------------------
 */
void InitInt(void);
#endif
