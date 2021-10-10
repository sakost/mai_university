FLAGS = -std=c++17 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -Ofast -pedantic
FILES = main.cpp graph.h graph.cpp johnson.cpp johnson.h

.PHONY: all
all:
	g++ ${FLAGS} ${FILES} -o solution

.PHONY: copy
copy:
	cd .. && cp -f ${FILES} solution && cd -

.PHONY: clean
clean:
	rm -rf *.o solution
