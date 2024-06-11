
all: integral

integral: integral.o functions_value.o
	gcc -m32 integral.o functions_value.o -o integral

integral.o: integral.c
	gcc -m32 -c integral.c -o integral.o

functions_value.o: functions_value.asm
	nasm -felf32 functions_value.asm -o functions_value.o

clean:
	rm integral *.o

test: integral
	./integral --test-root 1:2:3:5:0.001:3.848
	./integral --test-root 1:3:0:2:0.001:0.854
	./integral --test-root 2:3:2:4:0.001:3.244

	./integral --test-integral 1:0.854:3.848:0.001:13.205336
	./integral --test-integral 2:3.244:3.848:0.001:1.7130169
	./integral --test-integral 3:0.854:3.244:0.001:4.0038937