CC = gcc
CFLAGS = -Wall -g -c
LD = gcc
LDFLAGS = -g

OBJS = \
bin/karabiner.o \
bin/AES.o \
bin/misc.o

SRC = \
src/karabiner.c \
src/AES.c \
src/misc.c

INC = \
inc/karabiner.h \
inc/AES.h \
inc/misc.h

OUT = \
bin/karabiner

# All Target
all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) 

bin/%.o: src/%.c inc/%.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f bin/*.o bin/karabiner
