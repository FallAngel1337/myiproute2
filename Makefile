CC=gcc
CFLAGS=-g -w

INCLUDE=src/include
LIB=src/lib
BIN=bin

UTILS: ifconfig # will add more

all: $(UTILS)

ifconfig: $(INCLUDE)/ifconfig/ifconfig.c $(LIB)/ifctl.o
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ 

$(LIB)/ifctl.o: $(INCLUDE)/ifconfig/ifctl.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@ $(RM) -r $(LIB)/* $(BIN)/*