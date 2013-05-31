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

// Define the LCD pins.
#define RS BIT0
#define RW BIT1
#define EN BIT2
#define D4 BIT3
#define D5 BIT4
#define D6 BIT5
#define D7 BIT6

// Private function declarations.
unsigned int get_byte(char b, unsigned int pos);
void bit_to_pin(char c, unsigned int pos, unsigned int pin);
void _lcd_clear();


/**
 * Get a single bit from a byte.
 * 
 * @param b A byte.
 * @param pos The bit position to be extracted.
 * @return A bit.
 */
unsigned int get_byte(char b, unsigned int pos) {
	return (b & (1 << pos));
}

/**
 * Puts the desired bit into a pin. It's used to get the bits in a char
 * to send to the LCD.
 * 
 * @param c The character.
 * @param pos Bit position.
 * @param pin The pin to be set.
 */
void bit_to_pin(char c, unsigned int pos, unsigned int pin) {
	if (get_byte(c, pos)) {
		P2OUT |= pin;
	} else {
		P2OUT &= ~pin;
	}
}

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
	P2DIR |= (RS + RW + EN + D4 + D5 + D6 + D7);
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
	P2OUT &= ~(RS + RW + D6 + D7);
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
	P2OUT &= ~(RS + RW + D4 + D6 + D7);
	P2OUT |= D5;
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;

	//////////////////////////////////////////

	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// First 4 bits of the packet. (00 0010)
	P2OUT &= ~(RS + RW + D4 + D6 + D7);
	P2OUT |= D5;
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;
	
	
	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 NF00)
	P2OUT &= ~(RS + RW + D4 + D5 + D6);
	P2OUT |= D7;
	
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
	P2OUT &= ~(RS + RW + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 1DCB)
	P2OUT &= ~(RS + RW);
	P2OUT |= D7;
	
	bit_to_pin(disp, 0, D6);
	bit_to_pin(cur, 0, D5);
	bit_to_pin(blk, 0, D4);
	
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
	P2OUT &= ~(RS + RW + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0110)
	P2OUT &= ~(RS + RW + D4 + D7);
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
	P2OUT &= ~(RS + RW + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0001)
	P2OUT &= ~(RS + RW + D5 + D6 + D7);
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
	P2OUT &= ~(RS + RW + D4 + D5 + D6 + D7);
	
	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0010)
	P2OUT &= ~(RS + RW + D4 + D6 + D7);
	P2OUT |= D5;
	
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
	P2OUT &= ~RW;

	bit_to_pin(c, 4, D4);
	bit_to_pin(c, 5, D5);
	bit_to_pin(c, 6, D6);
	bit_to_pin(c, 7, D7);

	// End the packet transaction.
	delay_us(200);
	P2OUT &= ~EN;


	// Put EN HIGH to start sending data.
	P2OUT |= EN;
	delay_us(200);
	
	// Last 4 bits of the packet. (00 0001)
	P2OUT |= RS;
	P2OUT &= ~RW;

	bit_to_pin(c, 0, D4);
	bit_to_pin(c, 1, D5);
	bit_to_pin(c, 2, D6);
	bit_to_pin(c, 3, D7);
	
	// End the data transaction.
	delay_us(200);
	P2OUT &= ~EN;
}

void lcd_print(const char *string) {
	while (*string != '\0') {
		//if (*string == '\n') {
		lcd_putc(*string++);
	}
}
