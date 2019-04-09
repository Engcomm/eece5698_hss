SHELL:=/bin/bash
CC:=gcc 
SRCS:=$(wildcard *.c)
OBJDIR=obj
OBJS:=$(SRCS:%.c=%.o)
CFLAGS?= -Wall
LDLIBS?=


all : clean meltdown measure_execution_window

debug : CFLAGS += -DDEBUG -g
debug : clean all


meltdown : meltdown.o 
	$(CC) $< $(LDLIBS) -o $@ 

# .PHONY : measure_execution_window

measure_execution_window : 
	number=1 ; while [[ $$number -le 200 ]] ; do \
		echo $$number ; \
		((number = number + 1)) ; \
		$(CC) measure_execution_window.c -DWINDOW=$$number -DDEBUG $(LDLIBS) -o $@ ; \
		./$@ ; \
	done


%.o : %.c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf *.o meltdown measure_execution_window