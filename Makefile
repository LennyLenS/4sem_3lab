all : test


test : malloc.c
	gcc malloc.c -o test.exe
	./test.exe