#ifndef _OLED_LCD_LIB_H
#define _OLED_LCD_LIB_H

#include <xc.h>

/* Pin Configuration */
#define LCD_RS  RB1     // RS Select (Instruction/Data)
#define LCD_RW  RB2     // R/W Select Read or Write
#define LCD_EN  RB3     // Make ChipEnable Pulse
#define LCD_DB4 RB4     // Data bit 4
#define LCD_DB5 RB5     // Data bit 5
#define LCD_DB6 RB6     // Data bit 6
#define LCD_DB7 RB7     // Data bit 7


/* Pin I/O Configuration */
#define LCD_RS_IO  TRISBbits.TRISB1
#define LCD_RW_IO  TRISBbits.TRISB2
#define LCD_EN_IO  TRISBbits.TRISB3
#define LCD_DB4_IO TRISBbits.TRISB4
#define LCD_DB5_IO TRISBbits.TRISB5
#define LCD_DB6_IO TRISBbits.TRISB6
#define LCD_DB7_IO TRISBbits.TRISB7


/* Data Pin Configure INPUT */
#define DATAPIN_CONFIG_INPUT \
do                           \
{                            \
    ANSELBbits.ANSB1 = 0;    \
    ANSELBbits.ANSB2 = 0;    \
    ANSELBbits.ANSB3 = 0;    \
    ANSELBbits.ANSB4 = 0;    \
    ANSELBbits.ANSB5 = 0;    \
    LCD_DB4_IO = 1;          \
    LCD_DB5_IO = 1;          \
    LCD_DB6_IO = 1;          \
    LCD_DB7_IO = 1;          \
} while(0)


/* Data Pin Configure OUTPUT */
#define DATAPIN_CONFIG_OUTPUT \
do                            \
{                             \
    LCD_DB4_IO = 0;           \
    LCD_DB5_IO = 0;           \
    LCD_DB6_IO = 0;           \
    LCD_DB7_IO = 0;           \
} while(0)


/* ChipEnable Pulse */
#define ENABLE_PULSE \
do                   \
{                    \
    LCD_EN = 1;      \
    asm("nop");      \
    asm("nop");      \
    LCD_EN = 0;      \
} while(0)


/* Write Graphic Parameter */
typedef struct
{
    uint8_t x_axis_address;
    uint8_t y_axis_address;
    uint8_t *p_message_buf;
    uint8_t message_len;
} write_graphic_param_t;



/* RS - R/W Function Table */
/*-------------------------------  
| RS | R/W | Function           |
---------------------------------
|  L |  L  | Write Command Reg  |
---------------------------------
|  H |  L  | Write Data Reg     |
---------------------------------
|  L |  H  | Read Status Reg    |
---------------------------------
|  H |  H  | Read Data Reg      |
-------------------------------*/
/* Write mode type */
typedef enum
{
    WRITE_COMMAND_REG,
    WRITE_DATA_REG,
} lcd_write_mode_t;


/* Read mode type */
typedef enum
{
    READ_STATUS_REG,
    READ_DATA_REG,    
} lcd_read_mode_t;


/* char -> int8_t */
typedef char          int8_t;
typedef unsigned char uint8_t;


/* Prototype of Extern Function */
/*=====================================================
 * @brief
 *     LCD Initialize
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *  Function Set
 *  --------------------------------------------------
 *  | 0b 0 0 1 DL N F FT1 FT0                        |
 *  | DL : 0  (4bit mode)                            |
 *  | N  : 1  (2-line display)                       |
 *  | F  : 0  (5 * 8dots)                            |
 *  | FT : 00 (English Japanese character font table)|
 *  --------------------------------------------------
 *
 *  Display ON/OFF Control
 *  --------------------------------------------------
 *  | 0b 0 0 0 0 1 D C B                             |
 *  | D : 1 (Display turned ON)                      |
 *  | C : 0 (Cursor disabled)                        |
 *  | b : 0 (Cursor blinking disabled)               |
 *  --------------------------------------------------
 *===================================================*/
void oled_lcd_init(void);


/*=====================================================
 * @brief
 *     Write data to LCD
 * @param
 *     write_data:data transmitted to LCD
 *     write_mode:Select Command or Data Register
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void lcd_write(uint8_t write_data, lcd_write_mode_t write_mode);


/*=====================================================
 * @brief
 *     Read data from LCD
 * @param
 *     read_buf:pointer to store gotten data
 *     read_mode:Select Status or Data Register
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void lcd_read(uint8_t *read_buf, lcd_read_mode_t read_mode);


/*=====================================================
 * @brief
 *     Go to Graphic Mode
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void goto_graphic_mode(void);


/*=====================================================
 * @brief
 *     Clear LCD Display
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void lcd_display_clear(void);


/*=====================================================
 * @brief
 *     Write Graphic to LCD
 * @param
 *     p_param:pointer to write graphic parameter
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void lcd_write_graphic(write_graphic_param_t *p_param);



#endif  /* _OLED_LCD_LIB_H */
