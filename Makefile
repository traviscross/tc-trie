.PHONY: all clean

CC ?= clang
CFLAGS ?= -g3 -O3
CFLAGS_ASAN ?= -g3 -O1 -fsanitize=address -fno-omit-frame-pointer

SRCs := trie.c
OBJs := $(SRCs:.c=.o)
DEPs := $(OBJs:.o=.d)

all: trie.so trie.a

-include $(DEPs)

clean:
	rm -f *.so *.a *.o *.d test

%.o: %.c Makefile
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<
	$(CC) $(CFLAGS) -fPIC -MM $< -MF $*.d

%.asan.o: %.c Makefile
	$(CC) $(CFLAGS_ASAN) -fPIC -c -o $@ $<
	$(CC) $(CFLAGS_ASAN) -fPIC -MM $< -MF $*.asan.d

trie.so: trie.o Makefile
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

trie-asan.so: trie.asan.o Makefile
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

trie.a: trie.o Makefile
	ar crs $@ trie.o

trie-asan.a: trie.asan.o Makefile
	ar crs $@ trie.asan.o

test: test.c trie-asan.a cJSON.asan.o Makefile
	$(CC) $(CFLAGS_ASAN) -fPIE -pie -lm -o $@ $< trie-asan.a cJSON.asan.o
	./test.sh

test-opt: test.c trie.a cJSON.o Makefile
	$(CC) $(CFLAGS) -lm -o $@ $< trie.a cJSON.o
