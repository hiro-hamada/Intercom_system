#ifndef _WORD_GRAPHIC_H
#define _WORD_GRAPHIC_H


#include <xc.h>


/* Responce Type */
typedef enum
{
    RESPONCE1,
    RESPONCE2,    
} responce_t;


/* Prototype of Function */
/*=====================================================
 * @brief
 *     Write default line to LCD
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void write_default_message(void);


/*=====================================================
 * @brief
 *     Write Call Message to LCD
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void write_call_message(void);


/*=====================================================
 * @brief
 *     Not Here Message to LCD
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void write_not_here_message(void);


/*=====================================================
 * @brief
 *     Write Responce Message to LCD
 * @param
 *     responce:Choose RESPONCE1 or 2
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void write_responce_message(responce_t responce);


#endif
