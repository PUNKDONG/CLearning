.PHONY: all run  clean

all: app

app: main.c
	gcc -Wall -Wextra -std=c11 main.c -o appc

run: appc
	./appc

clean:
	rm -f appc *.o
