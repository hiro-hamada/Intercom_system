#include <xc.h>
#include "button_interrupt.h"


/*=====================================================
 * @breif
 *     Initialize Interrupt
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     RB0, RB1 is enabled
 *===================================================*/
void button_interrupt_init(void)
{
    /* Initialize PORTB */
    PORTB            = 0x00;
    TRISB            = 0b00000001;  // RB0 is input
    ANSELBbits.ANSB0 = 0;           // No Analog 

    /* Setting about Edge */
    IOCBP = 0b00000000;
    IOCBN = 0b00000001;

    /* Clear Flag */
    IOCBF            = 0x00;
    INTCONbits.IOCIF = 0;

    /* Enable Module */
    INTCONbits.GIE   = 1;  // Global interrupt enable
    INTCONbits.PEIE  = 1;  // Peripheral equipment interrupt enable
    INTCONbits.IOCIE = 1;  // Status change Interrupt Enable
}




