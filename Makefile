all : test


test : test.c
	gcc test.c -o test.exe
	./test.exe