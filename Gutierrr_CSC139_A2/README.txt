COMPILE:
gcc ProCon.c -lpthread -lrt

RUN:
a.out <SLEEP TIME> <# of Producer Threads> <# of Consumer Threads> 
	Example: a.out 10 1 1