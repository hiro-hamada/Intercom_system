#include <xc.h>
#include "pic_clock.h"
#include "oled_lcd_lib.h"
#include "word_graphic.h"
#include "usart.h"


// CONFIG1
#pragma config FOSC     = HS  // Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE     = OFF // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE    = ON  // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE    = OFF // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP       = OFF // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD      = OFF // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN    = ON  // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO     = OFF // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN    = OFF // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT      = OFF // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN   = OFF // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN    = OFF // PLL Enable (4x PLL disabled)
#pragma config STVREN   = OFF // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV     = LO  // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP      = OFF // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


/* Prototype of Static Function */
static void pic_port_init(void);
static void interrupt isr(void);
static void receive_sequence(void);


/******************************************************
 * main function
 *****************************************************/
int main(void)
{      
    /* Initialize Sequence */
    pic_port_init();
    usart_init();
    button_interrupt_init();
    __delay_ms(500);
    
    oled_lcd_init();

    /* Go to Graphic mode */
    goto_graphic_mode();
    __delay_ms(1000);
    
    /* Write Default Message */
    write_default_message();
    
    while(1)
    {
        ;
    }
    
    return 0;
}



/*-----------------------------------------------------
 * Initialize PIC PORT (Configure all PORT as OUTPUT)
 *---------------------------------------------------*/
static void pic_port_init(void)
{
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
}


/*------------------------------------------------------
 * Interrupt Function
 *----------------------------------------------------*/
static void interrupt isr(void)
{
    unsigned char counter = 0;
    
    /* Button0(RB0) Interrupt */
    if(IOCBF0)
    {
        /* Prevent a chattering */
        __delay_ms(100);

        /* Write Call Message */
        write_call_message();

        /* Transmit Notification via Bluetooth */
        put_char(0x01);

        /* Wait for 20[s] or to receive Responce */
        while(counter < 40)
        {
            if(RCIF)
            {
                receive_sequence();
                break;                
            }
            __delay_ms(500);
            counter++;
        }

        /* If Responce timeout , Write Not Here Message */
        if(counter == 40)
        {
            write_not_here_message();
            __delay_ms(10000);
            __delay_ms(10000);
        }
        
        /* Clear Flag */
        IOCBF            = 0x00;
        INTCONbits.IOCIF = 0;   

        /* Return display to Default Message */
    	write_default_message();
    }    
}


/*-----------------------------------------------------
 * Receive Sequence
 *---------------------------------------------------*/
static void receive_sequence(void)
{
    unsigned char receive_data;
    
    receive_data = RCREG;

    switch(receive_data)
    {
        case 1: 
            write_responce_message(RESPONCE1);
            __delay_ms(10000);
            __delay_ms(10000);
            break;
                        
        case 2:
            write_responce_message(RESPONCE2);
            __delay_ms(10000);
            __delay_ms(10000);
            break;
    }
}
