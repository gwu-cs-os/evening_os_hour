CC=gcc
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -Wextra -g $(INCDIRS) $(OPT)

CFILES=x.c y.c
OBJECTS=x.o y.o

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# regular expression where % is a wildcard
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTS)
