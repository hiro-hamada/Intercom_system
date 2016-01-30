#ifndef _USART_H
#define	_USART_H


#include <xc.h>
#include "pic_clock.h"


/* Setting Baudrate */
#define BAUDRATE       (9600)       // 9.6kbps


/* TXSTA Register Mask */
#define TXSTA_TX9D     (1 << 0)
#define TXSTA_TRMT     (1 << 1)
#define TXSTA_BRGH     (1 << 2)
#define TXSTA_SENDB    (1 << 3)
#define TXSTA_SYNC     (1 << 4)
#define TXSTA_TXEN     (1 << 5)
#define TXSTA_TX9      (1 << 6)
#define TXSTA_CSRC     (1 << 7)


/* RCSTA Register Mask */
#define RCSTA_RX9D     (1 << 0)
#define RCSTA_OERR     (1 << 1)
#define RCSTA_FERR     (1 << 2)
#define RCSTA_ADDEN    (1 << 3)
#define RCSTA_CREN     (1 << 4)
#define RCSTA_SREN     (1 << 5)
#define RCSTA_RX9      (1 << 6)
#define RCSTA_SPEN     (1 << 7)


/* Baud Rate Control Register Mask */
#define BAUDCTL_ABDEN  (1 << 0)
#define BAUDCTL_WUE    (1 << 1)
#define BAUDCTL_BRG16  (1 << 3)
#define BAUDCTL_SCKP   (1 << 4)
#define BAUDCTL_RCIDL  (1 << 6)
#define BAUDCTL_ABDOVF (1 << 7)


/* Calculate SPBRG */
#define SPBRG_DATA ((unsigned char)((_XTAL_FREQ / BAUDRATE / 16) - 1))


/* Prototype of Function */
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
void usart_init(void);


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
void put_char(char byte_data);


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
void put_string(char *str);


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
char get_char(void);


#endif	/* EUSART_H */

