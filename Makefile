
all: integral

integral: main.o calc_funcs.o value_funcs.o test.o
	gcc -m32 main.o calc_funcs.o value_funcs.o test.o -o integral

main.o: main.c src/calc_funcs.h test/test.h
	gcc -m32 -c main.c -o main.o

calc_funcs.o: src/calc_funcs.c src/calc_funcs.h
	gcc -m32 -c src/calc_funcs.c -o calc_funcs.o

test.o: test/test.c test/test.h
	gcc -m32 -c test/test.c -o test.o

value_funcs.o: src/value_funcs.asm
	nasm -f elf32 src/value_funcs.asm -o value_funcs.o

clean:
	rm integral *.o

test: integral
	./integral --test-root 4:5:0:1:0.001:0.448
	./integral --test-root 4:6:-2:-1:0.001:-1.821
	./integral --test-root 5:6:-1:0:0.001:-0.153

	./integral --test-integral 4:-1.821:0.448:0.00001:8.321
	./integral --test-integral 5:-0.153:0.448:0.00001:0.867
	./integral --test-integral 6:-1.821:-0.153:0.00001:2.334