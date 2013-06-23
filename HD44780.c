/**
 *	HD44780.c
 *	A Hitachi HD44780 driver for the MSP430G2553.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include <stdint.h>

#include "boolean.h"
#include "HD44780.h"
#include "delay.h"
#include "bitio.h"

// Define the LCD pins.
#define RS BIT1
#define EN BIT2
#define D4 BIT3
#define D5 BIT4
#define D6 BIT5
#define D7 BIT6

// Private function declarations.
void _lcd_clear();

/**
 * Initialize the LCD driver.
 * 
 * @param cur Cursor ON/OFF.
 * @param blk Blinking cursor ON/OFF.
 */
void lcd_init(bool cur, bool blk) {
	// Setup the pins and wait for the driver to stabilize.
	lcd_setup();
	delay_ms(45);

	// Initialization. For more information about this madness check page
	// 46 of the datasheet.
	lcd_init_function_set();
	delay_us(4500);
	lcd_init_function_set();
	delay_us(150);
	lcd_init_function_set();
	lcd_function_set();  // The last madness.

	lcd_display_control(TRUE, cur, blk);
	_lcd_clear();
	lcd_entry_mode();
}

/**
 * Setup the LCD pins.
 */
void lcd_setup() {
	// Set the outputs and turn XIN into P2.6
	P2DIR |= (RS + EN + D4 + D5 + D6 + D7);
	P2SEL &= ~D7;
}

/**
 * The initialization function set.
 */
void lcd_init_function_set() {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0011)
	P2OUT &= ~(RS + D6 + D7);
	P2OUT |= (D5 + D4);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * LCD function set. (001D NF00)
 */
void lcd_function_set() {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0010)
	P2OUT &= ~(RS + D4 + D6 + D7);
	P2OUT |= D5;
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;

	//////////////////////////////////////////

	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0010)
	P2OUT &= ~(RS + D4 + D6 + D7);
	P2OUT |= D5;
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;
	
	
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 NF00)
	P2OUT &= ~(RS + D4 + D5 + D6);
	P2OUT |= D7;  // Assumes a 2-line display.
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Sets the LCD controllers. (0000 1DCB)
 * 
 * @param disp Sets the entire display ON/OFF.
 * @param cur Cursor ON/OFF.
 * @param blk Blink cursor ON/OFF.
 */
void lcd_display_control(bool disp, bool cur, bool blk) {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0000)
	P2OUT &= ~(RS + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 1DCB)
	P2OUT &= ~RS;
	P2OUT |= D7;
	
	bit_to_pin(disp, 0, &P2OUT, D6);
	bit_to_pin(cur, 0, &P2OUT, D5);
	bit_to_pin(blk, 0, &P2OUT, D4);
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Sets the LCD entry mode. (0000 0110)
 */
void lcd_entry_mode() {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0000)
	P2OUT &= ~(RS + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0110)
	P2OUT &= ~(RS + D4 + D7);
	P2OUT |= (D5 + D6);
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Clears the LCD. (0000 0001)
 */
void _lcd_clear() {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0000)
	P2OUT &= ~(RS + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0001)
	P2OUT &= ~(RS + D5 + D6 + D7);
	P2OUT |= D4;
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Clears the LCD with the correct delay.
 */
void lcd_clear() {
	_lcd_clear();
	delay_ms(1);
}

/**
 * Returns both display and cursor to the original position. (0000 0010)
 */
void lcd_return_home() {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0000)
	P2OUT &= ~(RS + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0010)
	P2OUT &= ~(RS + D4 + D6 + D7);
	P2OUT |= D5;
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 *	Sets the cursor position.
 *
 *	@param line The line.
 *	@param col The column.
 */
void lcd_set_cursor(unsigned int line, unsigned int col) {
	char offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	char pos = offsets[line] + col;

	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 1AAA)
	P2OUT &= ~RS;
	P2OUT |= D7;
	bit_to_pin(pos, 4, &P2OUT, D4);
	bit_to_pin(pos, 5, &P2OUT, D5);
	bit_to_pin(pos, 6, &P2OUT, D6);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 AAAA)
	P2OUT &= ~RS;
	bit_to_pin(pos, 0, &P2OUT, D4);
	bit_to_pin(pos, 1, &P2OUT, D5);
	bit_to_pin(pos, 2, &P2OUT, D6);
	bit_to_pin(pos, 3, &P2OUT, D7);
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Put a character in the screen.
 * 
 * @param c Character.
 */
void lcd_putc(const char c) {
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0100)
	P2OUT |= RS;

	bit_to_pin(c, 4, &P2OUT, D4);
	bit_to_pin(c, 5, &P2OUT, D5);
	bit_to_pin(c, 6, &P2OUT, D6);
	bit_to_pin(c, 7, &P2OUT, D7);

	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0001)
	P2OUT |= RS;

	bit_to_pin(c, 0, &P2OUT, D4);
	bit_to_pin(c, 1, &P2OUT, D5);
	bit_to_pin(c, 2, &P2OUT, D6);
	bit_to_pin(c, 3, &P2OUT, D7);
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

/**
 * Print a string in the screen.
 * 
 * @param string The string.
 */
void _lcd_print(const char *string) {
	while (*string != '\0') {
		lcd_putc(*string++);
	}
}

/**
 *	Print a string at a position.
 *
 *	@param string The string.
 *	@param line Screen line.
 *	@param col Screen column.
 */
void lcd_print(const char *string, unsigned int line, unsigned int col) {
	lcd_set_cursor(line, col);
	_lcd_print(string);
}
