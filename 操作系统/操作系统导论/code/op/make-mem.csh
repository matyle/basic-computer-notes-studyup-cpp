#! /bin/csh -f

# ON MAC OS X: this removes address space randomization
# as documented on stack overflow:
# http://stackoverflow.com/questions/23897963/documented-way-to-disable-aslr-on-os-x
gcc -o mem mem.c -Wall -Wl,-no_pie

