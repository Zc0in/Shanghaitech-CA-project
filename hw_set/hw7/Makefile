CC=g++
CFLAGS=-Werror -Wpedantic -Wall -Wextra -std=c++11

all: main

main: main.cpp ringbuffer.ipp ringbuffer.hpp
	${CC} ${CFLAGS} ringbuffer.hpp main.cpp -o main

memcheck: main
	valgrind --tool=memcheck --leak-check=full ./main

submit:
	tar -cvf hw7.tar ringbuffer.ipp

clean:
	rm -f ./main
	rm -f ./*.o

.PHONY: clean