.PHONY:all clean run app

all: app

app: math_utils.o main.o
	gcc -Wall -Wextra -std=c11 main.o math_utils.o -o out	

main.o:main.c
	gcc -Wall -Wextra -std=c11 -c main.c -o main.o
math_utils.o:math_utils.c
	gcc -Wall -Wextra -std=c11 -c math_utils.c -o math_utils.o

run: out
	./out

clean :
	rm -f out *.o