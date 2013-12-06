CC = gcc
# If you have a 64-bit computer, you may want to use this instead.
#CC = gcc -m32
CFLAGS =-std=c99 -pedantic -Wall -Werror 
OPTFLAG =-O2
DEBUGFLAG =-g

# This is the name of the static archive to produce
# Don't change this line
LIBRARY = malloc

# The C and H files
# Your test file SHOULD NOT be in this line
CFILES = my_malloc.c my_sbrk.c list.c
HFILES = my_malloc.h list.h

PROGRAM = hw13

# Targets:
# test -- runs your test program
# debug -- runs a the supplied structure verifier. THIS IS NOT THE GRADER THE TAs USE!
# check -- runs a the supplied structure verifier. THIS IS NOT THE GRADER THE TAs USE!
# clean -- removes compiled code from the directory
 

test: CFLAGS += $(OPTFLAG) 
test: $(PROGRAM)-test
	./$(PROGRAM)-test

debug: CFLAGS += $(DEBUGFLAG) 
debug: $(PROGRAM)-debug
	gdb ./$(PROGRAM)-debug

check: CFLAGS += $(OPTFLAG) 
check: $(PROGRAM)-verifier
	@for i in 1 2 3 4; do \
		./$(PROGRAM)-verifier $$i && exit; \
	done; \
	echo "Passed all cases"


$(PROGRAM)-test: lib$(LIBRARY).a test.c
	$(CC) $(CFLAGS) test.c -L . -l$(LIBRARY) -o $@

$(PROGRAM)-debug: lib$(LIBRARY).a test.c
	$(CC) $(CFLAGS) test.c -L . -l$(LIBRARY) -o $@

$(PROGRAM)-verifier: student_verifier.o lib$(LIBRARY).a
	$(CC) $(CFLAGS) $< -L . -l$(LIBRARY) -o $@

OFILES = $(patsubst %.c,%.o,$(CFILES))

lib$(LIBRARY).a: $(OFILES)
	ar -cr lib$(LIBRARY).a $(OFILES)
%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf lib$(LIBRARY).a $(PROGRAM)-test $(PROGRAM)-debug $(PROGRAM)-verifier $(OFILES)
