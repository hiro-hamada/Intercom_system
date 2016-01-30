#include <xc.h>
#include "usart.h"


/*=====================================================
 * @breif
 *     Initialize uart
 * @param
 *     ir_select:Select UART interrupt ON/OFF
 * @return
 *     void:
 * @note
 *     RC7(RX), RC6(TX)
 *     BRGH set -> High Speed Mode
 *==================================================*/
void usart_init(void)
{
    /* Initialize RX, TX pin by TRISC */
    PORTC  = 0x00;
    TRISC |= 0b10000000;  // RC7 is Input
 
    /* Initialize EUSART */
    SPBRG = SPBRG_DATA;
    TXSTA = (TXSTA_TXEN | TXSTA_BRGH);
    RCSTA = (RCSTA_SPEN | RCSTA_CREN);
}


/*=====================================================
 * @breif
 *     Transmit 1 Byte data
 * @param
 *     byte_data:1byte data to transmit
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void put_char(char byte_data)
{   
    /* Wait until TXREG is empty */
    while(TXIF == 0)
    {
        ;        
    }
    
    /* Write transmitted data */
    TXREG = byte_data;
}


/*=====================================================
 * @brief
 *     Transmit String
 * @param
 *     str:string array
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void put_string(char *str)
{
    while(*str != '\0')
    {
        put_char(*str);
        str++;
    }
    put_char('\0');
}


/*=====================================================
 * @brief
 *     Receive 1 Byte
 * @param
 *     none:
 * @return
 *     REREG:Receive Data
 * @note
 *     none
 *===================================================*/
char get_char(void)
{
    while(RCIF == 0)
    {
        ;        
    }
 
    return RCREG;
}


