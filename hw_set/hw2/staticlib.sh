#!/bin/bash
gcc -c ringbuffer.c -o ringbuffer.o
gcc -c test.c -o test.o
ar rcs libringbuffer.a ringbuffer.o
ld -o staticringbuffer -L. test.o -lringbuffer -lc
