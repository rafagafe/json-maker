

build: example.exe

clean:
	rm -rf *.o
	rm -rf *.exe

all: clean build

test: test.exe
	./test.exe
	
example.exe: example.o json-maker.o
	gcc -std=c99 -Wall -o example.exe example.o json-maker.o

test.exe: test.o json-maker.o
	gcc -std=c99 -Wall -o test.exe test.o json-maker.o
	
json-maker.o: json-maker.c json-maker.h
	gcc -std=c99 -Wall -c json-maker.c

example.o: example.c json-maker.h
	gcc -std=c99 -Wall -c example.c
	
test.o: test.c json-maker.h
	gcc -std=c99 -Wall -c test.c	
