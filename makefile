
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d) 

build: example.exe

clean:
	rm -rf *.d
	rm -rf *.o
	rm -rf *.exe
	
all: clean build

test: test.exe
	./test.exe
	
example.exe: example.o json-maker.o
	gcc -std=c99 -Wall -o example.exe example.o json-maker.o

test.exe: test.o json-maker.o
	gcc -std=c99 -Wall -o test.exe test.o json-maker.o
	
-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@	
