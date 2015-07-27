CC = gcc
CFLAGS = -Wall -g -c
LD = gcc
LDFLAGS = -g
LDLIBS = -lm

OBJS = \
bin/karabiner.o \
bin/aes.o \
bin/aes_helper.o \
bin/misc.o \
bin/sha.o

SRC = \
src/karabiner.c \
src/aes.c \
src/aes_helper.c \
src/misc.c \
src/sha.c

INC = \
inc/karabiner.h \
inc/aes.h \
inc/misc.h \
inc/sha.h

OUT = \
bin/karabiner

# All Target
all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f bin/*.o bin/karabiner
