#include <xc.h>
#include "pic_clock.h"
#include "oled_lcd_lib.h"


/* Prototype of Static Function */
static void lcd_write_4bit(uint8_t write_data);
static void lcd_read_4bit(uint8_t *p_read_buf);
static void check_busy_flag(void);

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
void oled_lcd_init(void)
{
    uint8_t i;    
        
    /* Pin I/O configuration -> all OUTPUT */
    LCD_RS_IO  = 0;
    LCD_RW_IO  = 0;
    LCD_EN_IO  = 0;
    DATAPIN_CONFIG_OUTPUT;

    /* All Data Pin Clear */
    LCD_RS  = 0;
    LCD_RW  = 0;
    LCD_EN  = 0;
    LCD_DB4 = 0;
    LCD_DB5 = 0;
    LCD_DB6 = 0;
    LCD_DB7 = 0;

    /* Wait for Power Stabilization 500ms */
    __delay_ms(500);

    /* Synchronization function */
    for(i = 0; i < 5; i++)
    {
        lcd_write_4bit(0x00);
    }
    
    /* Function Set */
    lcd_write_4bit(0b00000010);
    lcd_write(0b00101000, WRITE_COMMAND_REG);    // Function Set

    /* Display ON/OFF Control */
    lcd_write(0b00001100, WRITE_COMMAND_REG);
    
    /* Display Clear */
    lcd_write(0b00000001, WRITE_COMMAND_REG);
    
    /* Return Home */
    lcd_write(0b00000010, WRITE_COMMAND_REG);
    
    /* Entry Mode Set */
    lcd_write(0b00000110, WRITE_COMMAND_REG);
}


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
void lcd_write(uint8_t write_data, lcd_write_mode_t write_mode)
{
    /* Write Mode */
    LCD_RW = 0;

    /* Select Command Register or Data Register */
    if(write_mode == WRITE_COMMAND_REG)
    {
        LCD_RS = 0;    // Write Command Register
    }
    else
    {
        LCD_RS = 1;    // Write Date Register
    }
    
    /* Data pin configure OUTPUT */
    DATAPIN_CONFIG_OUTPUT;

    /* Write data to LCD */
    lcd_write_4bit(write_data >> 4);
    lcd_write_4bit(write_data);
    
    /* Check BusyFlag */
    check_busy_flag();
}


/*=====================================================
 * @brief
 *     Read data from LCD
 * @param
 *     p_read_buf:pointer to store gotten data
 *     read_mode :Select Status or Data Register
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void lcd_read(uint8_t *p_read_buf, lcd_read_mode_t read_mode)
{
    /* Initialize read buffer */
    *p_read_buf = 0x00;

    /* Read Mode */
    LCD_RW = 1;

    /* Select Status Register or Data Register */
    if(read_mode == READ_STATUS_REG)
    {
        LCD_RS = 0;
    }
    else
    {
        LCD_RS = 1;
    }

    /* Data pin configure INPUT */
    DATAPIN_CONFIG_INPUT;

    /* Read data from LCD */
    lcd_read_4bit(p_read_buf);
    lcd_read_4bit(p_read_buf);
}


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
void goto_graphic_mode(void)
{   
    /* Go to Graphic Mode */
    lcd_write(0b00011111, WRITE_COMMAND_REG);
    
    /* Clear Display */
    lcd_write(0b00000001, WRITE_COMMAND_REG);
}


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
void lcd_display_clear(void)
{
    /* Clear LCD Display */
    lcd_write(0b00000001, WRITE_COMMAND_REG);
}


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
void lcd_write_graphic(write_graphic_param_t *p_param)
{
    uint8_t i;

    /* Clear LCD Display */
    lcd_display_clear();

    /* Write Message */
    for(i = 0; i < p_param->message_len; i++)
    {
        lcd_write(p_param->x_axis_address, WRITE_COMMAND_REG);
        lcd_write(p_param->y_axis_address, WRITE_COMMAND_REG);
        lcd_write(p_param->p_message_buf[i], WRITE_DATA_REG);
        p_param->x_axis_address++;
    }    
}


/*-----------------------------------------------------
 * @brief
 *     Write data to LCD
 * @param
 *     write_data:data transmitted to LCD
 * @return
 *     none:
 * @note
 *     none
 *---------------------------------------------------*/
static void lcd_write_4bit(uint8_t write_data)
{
    /* Write data to I/O PORT */
    LCD_DB4 = ((write_data >> 0) & 0x01);
    LCD_DB5 = ((write_data >> 1) & 0x01);
    LCD_DB6 = ((write_data >> 2) & 0x01);
    LCD_DB7 = ((write_data >> 3) & 0x01);

    /* Transmit data to LCD */
    ENABLE_PULSE;
}




/*-----------------------------------------------------
 * @brief
 *     Read data from LCD
 * @param
 *     p_read_buf :pointer to store gotten data
 * @return
 *     none:
 * @note
 *     none
 *---------------------------------------------------*/
static void lcd_read_4bit(uint8_t *p_read_buf)
{   
    /* Read data */
    LCD_EN = 1;     // Start receiving data

    (*p_read_buf) <<= 1;
    *p_read_buf |= LCD_DB7;
    (*p_read_buf) <<= 1;
    *p_read_buf |= LCD_DB6;
    (*p_read_buf) <<= 1;
    *p_read_buf |= LCD_DB5;
    (*p_read_buf) <<= 1;
    *p_read_buf |= LCD_DB4;

    LCD_EN = 0;    // End receiving data
}


/*-----------------------------------------------------
 * @brief
 *     Check BusyFlag
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *---------------------------------------------------*/
static void check_busy_flag(void)
{
    uint8_t busy_flag = 0x80;
    
    while(busy_flag & 0x80)
    {
        lcd_read(&busy_flag, READ_STATUS_REG);
    }
}

