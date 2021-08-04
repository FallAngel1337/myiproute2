CC=gcc
CFLAGS=-g -Wall -Werror

INCLUDE=src/include
LIB=src/lib
BIN=bin

UTILS: ifconfig # will add more

all: $(UTILS)

ifconfig: $(INCLUDE)/ifconfig/*.c $(INCLUDE)/ifconfig/*.h
	$(CC) $(CFLAGS) -o $(BIN)/$@ $<