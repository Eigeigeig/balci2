# Makefile Exercise Sheet 02
CC=gcc

CFLAGS = -std=c11 -D_GNU_SOURCE -Og -g -W -Wall -Wno-unused-parameter \
         -Werror=implicit-function-declaration -Wfatal-errors -pthread \
		 -Iinclude -Ilibsrc/include

LDLIBS=-Llibsrc -lroomman
LIB=libsrc/libroomman.a

# compile all .c files found in src directory
FILES=$(shell find src -name '*.c')

.PHONY: all clean distclean test

BINARY := roommanager
OBJS=$(FILES:.c=.o)

all: $(BINARY)

$(BINARY): $(OBJS) $(LIB)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS)

$(LIB):
	make -C libsrc

clean:
	@rm -f $(BINARY) $(OBJS)

distclean: clean
	cd libsrc;$(MAKE) distclean
	rm -f $(PROGS) *~

test: $(BINARY)
	sh tests/testroomman.sh -n
