all: main find

main: main.c
	gcc -o main main.c

find: find.c
	gcc -o find find.c

clean:
	rm pregistro sregistro main find
