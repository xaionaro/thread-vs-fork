all: build run

build:
	gcc          -O3 -Wall -pedantic -pthread context-switch.c -o context-switch-fork
	gcc -DTHREAD -O3 -Wall -pedantic -pthread context-switch.c -o context-switch-thread
	gcc          -O3 -Wall -pedantic -pthread create-and-destroy.c -o create-and-destroy-fork
	gcc -DTHREAD -O3 -Wall -pedantic -pthread create-and-destroy.c -o create-and-destroy-thread

run:
	echo thread
	time ./context-switch-thread
	time ./create-and-destroy-thread
	echo fork
	time ./context-switch-fork
	time ./create-and-destroy-fork

clean:
	rm -f context-switch-fork context-switch-thread create-and-destroy-fork create-and-destroy-thread
