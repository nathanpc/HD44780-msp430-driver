NAME = lcd-driver
DEVICE= msp430g2553
PROJECT = $(NAME)-$(DEVICE)
CC = msp430-gcc -std=c99 -Os -mmcu=$(DEVICE)
MSPDEBUG = sudo mspdebug rf2500
SRC = HD44780_test.c HD44780.c delay.c

all: $(PROJECT).elf

$(PROJECT).elf: $(SRC)
	$(CC) -o $(PROJECT).elf $(SRC)

write: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf"

run: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf" "reset" "run"

reset:
	$(MSPDEBUG) "reset"
#on:
#	$(PK2) -T

#off:
#	$(PK2) -W

#erase:
#	$(PK2) -E

clean:
	rm -f *.elf
