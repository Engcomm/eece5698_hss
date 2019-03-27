CC:=gcc 
TARGET:=meltdown
SRCS:=$(wildcard *.c)
OBJDIR=obj
OBJS:=$(SRCS:%.c=%.o)
CFLAGS?= -Wall
LDLIBS?=


all : clean $(TARGET)

debug : CFLAGS += -DDEBUG -g
debug : clean all

$(TARGET): $(OBJS) 
	$(CC) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) $(TARGET)