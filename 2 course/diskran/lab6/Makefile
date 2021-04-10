CC = g++
FLAGS = -std=c++17 -O3 -pedantic -lm -Wall
FILES = longarithmetic.h main.cpp
PROG = solution
TAR_GZ_NAME = solution

.PHONY: all
all: build

.PHONY: build
build:
	$(CC) $(FLAGS) -o $(PROG) $(FILES)

.PHONY: run
run: build
	./$(PROG)

.PHONY: clean
clean:
	rm -f *.o $(PROG)

.PHONY: test
test:

.PHONY: build_tar
build_tar:
	mkdir $(TAR_GZ_NAME)
	cp $(FILES) Makefile $(TAR_GZ_NAME)/
	tar -czvf $(TAR_GZ_NAME).tar.gz $(TAR_GZ_NAME)
	rm -r --interactive=never $(TAR_GZ_NAME)/*
	rmdir $(TAR_GZ_NAME)
