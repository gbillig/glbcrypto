CC = gcc
CFLAGS = -Wall -g -c
LD = gcc
LDFLAGS = -g -static -L./lib
LDLIBS = -lglbcrypto -lm
AR = ar
ARFLAGS = rcs

OBJS = \
bin/misc.o \
bin/glbcrypto.o

OUT = \
bin/glbcrypto_tests

LIB_OBJS = \
bin/misc.o \
bin/mode_of_operation.o \
bin/aes_helper.o \
bin/aes_cipher.o \
bin/aes.o \
bin/sha_helper.o \
bin/sha.o 

LIB_OUT = \
lib/libglbcrypto.a

# All Target
all: $(LIB_OUT) $(OUT)
	rm -f bin/*.o

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(LIB_OUT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

clean:
	rm -f bin/*.o bin/glbcrypto_tests lib/libglbcrypto.a
