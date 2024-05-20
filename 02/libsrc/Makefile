CC=gcc
DEPCC=$(CC) -MM
CFLAGS=-g -Iinclude -std=c11

# Simply compile all .c files found in this directory
FILES=$(shell find . -name '*.c')
OBJS=$(FILES:.c=.o)
DEPS=.depend
LIB=libroomman.a
DOC=doc

all: $(LIB)

# Build a library: "$@" is the make-rule's
# "Target" (here: "$(LIB)") and thus
# eventually (see above) "libroomman.a". 
$(LIB): $(OBJS)
	$(AR) -rv $@ $(OBJS)

-include $(DEPS)

$(OBJS):

$(DEPS):
	$(DEPCC) $(FILES) >$(DEPS)

# Build documentation using doxygen
$(DOC): include/roomman.h roommandef.h
	mkdir -p doc
	doxygen doxyfile

clean:
	rm -f $(OBJS) $(DEPS)

distclean: clean
	rm -f $(LIB) *~
	rm -rf doc
