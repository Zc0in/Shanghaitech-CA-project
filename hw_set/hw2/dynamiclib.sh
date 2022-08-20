#!/bin/bash
gcc -c -fPIC ringbuffer.c ringbuffer.h
gcc -shared -fPIC -o libringbuffer.so ringbuffer.o 
gcc -o dynamicringbuffer test.c -L. -Wl,-rpath=$(pwd) -lringbuffer 