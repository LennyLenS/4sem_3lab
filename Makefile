CC=gcc 
CFLAGS=-c 
LIBRARY=malloc.a
SOURCES=$(wildcard malloc.c)
OBJECTS=$(SOURCES:.c=.o)


all : test

.c.o:
	$(CC) $(CFLAGS) $< -o $@


test : $(LIBRARY) main.c
	gcc main.c -std=c99 -Lmalloc -o test.exe
	./test.exe

$(LIBRARY): $(OBJECTS)
	ar -rc $(LIBRARY) $^
	ranlib $(LIBRARY)
