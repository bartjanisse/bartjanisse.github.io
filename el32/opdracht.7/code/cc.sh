#!/bin/bash

# CCC="/usr/local/crosscompile/i386-gnulibc/i386-unknown-linux-gnu/bin/gcc -I /usr/local/crosscompile/i386-gnulibc/i386-unknown-linux-gnu/bin/include"

CCC="gcc"
FLAGS="-Wall -W -Wshadow -Wcast-qual -Wwrite-strings"

for i in $(ls *.c); do
    echo `basename $i .c`.o
    $CCC -o `basename $i .c`.o -g3 -c $FLAGS $i || exit
done

echo

for i in $(ls T*.o); do
    echo `basename $i .o`
    $CCC -o `basename $i .o` $i A*.o C*.o  H*.o -lrt $FLAGS || exit
done


