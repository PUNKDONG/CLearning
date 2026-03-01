
.PHONY: run clean

app: main.o math_utils.o
	gcc -Wall -Wextra -std=c11 main.o math_utils.o -o out

main.o:main.c math_utils.h
	gcc -Wall -Wextra -std=c11 -c main.c -o main.o

math_utils.o:math_utils.c math_utils.h
	gcc -Wall -Wextra -std=c11 -c math_utils.c -o math_utils.o

run: out
	./out
clean:
	rm -f out *.o