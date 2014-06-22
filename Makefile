CC=msp430-gcc
LD=msp430-gcc
RM=rm
SZ=msp430-size

LDFLAGS=-mmcu=msp430g2452 -Wl,-Map=main.map
CFLAGS+=-Wall -IInclude -I./

SOURCES=src/main.c src/uart.c

DEBUG ?= 0
ifeq ($(DEBUG), 0)
	CFLAGS += -O2
else
	CFLAGS += -O0 -g
endif

OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
PRJ_NAME=switcher

all: $(PRJ_NAME).elf

$(PRJ_NAME).elf: $(OBJECTS)
	$(LD) $(OBJECTS) -o $(PRJ_NAME).elf $(LDFLAGS)
	$(SZ) $(PRJ_NAME).elf

%.o: %.c $(SOURCES)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:

program:
	mspdebug rf2500 "prog $(PRJ_NAME).elf"

clean:
	$(RM) $(OBJECTS)
