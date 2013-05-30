#include <msp430g2553.h>

#include "boolean.h"
#include "HD44780.h"
#include "delay.h"

#define LED BIT0

void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer.
	P1DIR |= LED;
	
	// Initialize the LCD driver.
	lcd_init(TRUE, TRUE);

	// Write stuff.
	lcd_putc('H');
	lcd_putc('e');
	lcd_putc('l');
	lcd_putc('l');
	lcd_putc('o');

	while (1) {
		// Turn the ON indicator.
		P1OUT ^= LED;
	}
}