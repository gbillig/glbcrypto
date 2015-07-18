CC = gcc
CFLAGS = -Wall -g
LD = gcc
LDFLAGS = -g

OBJS = \
bin/karabiner.o

SRC = \
src/karabiner.c

INC = \
inc/karabiner.h

OUT = \
bin/karabiner

# All Target
all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) 

$(OBJS): $(SRC) $(INC)
	$(CC) $(CFLAGS) -o $(OBJS) -c $(SRC)

clean:
	rm -f bin/*.o bin/karabiner
