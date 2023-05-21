all : test


test : malloc.c main.c
	gcc -c malloc.c -o malloc.o
	gcc malloc.c main.c -o test.exe
	./test.exe

test2 : malloc.c 
	gcc  malloc.c -o test.exe
	./test.exe