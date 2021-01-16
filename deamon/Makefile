deamon : deamon.o tools.o fifo_tools.o log_tools.o 
	gcc -o deamon deamon.o tools.o fifo_tools.o log_tools.o 

deamon.o : deamon.c
	gcc -c deamon.c

tools.o : ../tools/tools.c
	gcc -c ../tools/tools.c

fifo_tools.o : ../tools/fifo_tools.c
	gcc -c ../tools/fifo_tools.c

log_tools.o : ../tools/log_tools.c
	gcc -c ../tools/log_tools.c