CC=gcc
CFLAGS=-I.
DEPS = lab.h
OBJ = lab.o main.o

all: lab

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

lab: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

run: all
		./lab

.PHONY: clean

clean:
	rm -f $(OBJ) lab
