#Created By: Mark Shanklin
#My Makefile

CC = gcc
CFLAGS = -Wall -std='gnu11' -lncurses
PROG = Calc
#SOURCES = $(shell echo *.c)
SOURCES = calc.c
DEPS := $(shell echo *.h)
OBJECTS = $(SOURCES:.c=.o)
DEPENDS = depends.mk
$(shell touch ${DEPENDS})

LAB := $(shell basename ${PWD})
TAR_FILE := $(LAB)_${LOGNAME}.tar

all: depends $(PROG)

$(PROG): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f $(PROG) *.o $(DEPENDS)

include $(DEPENDS)

depends:
	$(CC) -MM $(SOURCES) > $(DEPENDS)

tar:
	rm -f $(TAR_FILE)
	tar cvf $(TAR_FILE) *.[ch] [Mm]akefile
