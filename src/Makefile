
CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lX11 -lXpm

EXE = xdiamonds
OBJECTS = background.o ball.o block.o color.o debug.o error.o event.o \
	font.o image.o level.o main.o menu.o score.o

.SUFFIXES: .o .c .h

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) $(LDFLAGS)

.c.o: $*.h $*.c xdiamonds.h
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f $(EXE) $(OBJECTS) core

