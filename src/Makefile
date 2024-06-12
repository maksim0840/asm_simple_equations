calculus: calc_funcs.o value_funcs.o
	gcc -m32 calc_funcs.o value_funcs.o -o calculus.o

calc_funcs.o: calc_funcs.c calc_funcs.h
	gcc -m32 -c calc_funcs.c -o calc_funcs.o

value_funcs.o: value_funcs.asm
	nasm -f elf32 value_funcs.asm -o value_funcs.o