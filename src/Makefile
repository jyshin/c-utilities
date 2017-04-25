OBJDIR 	:= $(CURDIR)/../build
HDRS 	:= $(wildcard *.h)
SRCS 	:= $(wildcard *.c)
OBJS 	:= $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

LD 	:= ld
CC	:= gcc
CFLAGS	:= -Wall -O2 -g
LFLAGS	:= -lpthread -lm

all: $(OBJDIR) $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *~ $(OBJDIR)

#depend:
#	gccmakedep 

#OBJS := $(SRCS:.c=.o)
