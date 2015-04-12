CC = gcc

all: quickstart

quickstart: usbreset hub-ctrl main.o 
	$(CC) main.o -o quickstart -lparse -ljson

main.o:
	$(CC) main.c -o main.o -c

usbreset:
	$(CC) usbreset.c -o usbreset 

hub-ctrl:
	$(CC) hub-ctrl.c -o hub-ctrl -lusb

clean:
	rm -fR quickstart main.o usbreset hub-ctrl

