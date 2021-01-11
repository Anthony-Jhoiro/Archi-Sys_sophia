invoker : invoker.o tools.o
	gcc -o invoker invoker.o tools.o

invoker.o : invoker.c
	gcc -c invoker.c

tools.o : ../tools/tools.c
	gcc -c ../tools/tools.c
