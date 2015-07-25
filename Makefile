CC = gcc
CFLAGS = -Wall -g -c
LD = gcc
LDFLAGS = -g
LDLIBS = -lm

OBJS = \
bin/karabiner.o \
bin/AES.o \
bin/misc.o \
bin/SHA.o

SRC = \
src/karabiner.c \
src/AES.c \
src/misc.c \
src/SHA.c

INC = \
inc/karabiner.h \
inc/AES.h \
inc/misc.h \
inc/SHA.h

OUT = \
bin/karabiner

# All Target
all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

bin/%.o: src/%.c inc/%.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f bin/*.o bin/karabiner
