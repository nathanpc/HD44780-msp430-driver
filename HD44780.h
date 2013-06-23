/**
 *	HD44780.h
 *	A Hitachi HD44780 driver for the MSP430G2553.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef _HD44780_LCD_DRIVER_H_
#define _HD44780_LCD_DRIVER_H_

#include <stdint.h>
#include "boolean.h"

void lcd_setup();
void lcd_init_function_set();
void lcd_function_set();
void lcd_display_control(bool disp, bool cur, bool blk);
void lcd_entry_mode();
void lcd_init(bool cur, bool blk);

void lcd_return_home();
void lcd_clear();

void lcd_set_cursor(unsigned int line, unsigned int col);
void lcd_putc(const char c);
void _lcd_print(const char *string);
void lcd_print(const char *string, unsigned int line, unsigned int col);

#endif  // _HD44780_LCD_DRIVER_H_
