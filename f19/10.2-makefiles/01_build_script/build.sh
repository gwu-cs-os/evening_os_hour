#!/bin/sh

gcc -Wall -Wextra -I. -g -c x.c
gcc -Wall -Wextra -I. -g -c y.c
gcc y.o x.o -o bin
