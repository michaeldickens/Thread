CC = gcc

CFLAGS = -std=c99 -Wall

DEBUG_FLAGS = -g -O0 -fno-inline
PROFILE_FLAGS = -g -pg -O2 -fno-inline
RELEASE_FLAGS = -O2 -DNDEBUG

DEFAULT_FLAGS = $(DEBUG_FLAGS)

LDLIBS = -lm -lpthread

PROGRAMS = atom_test queue_test semaphore_test

all: $(PROGRAMS)
all: CFLAGS += $(DEFAULT_FLAGS)

debug: $(PROGRAMS)
debug: CFLAGS += $(DEBUG_FLAGS)

profile: $(PROGRAMS)
profile: CFLAGS += $(PROFILE_FLAGS)

release: $(PROGRAMS)
release: CFLAGS += $(RELEASE_FLAGS)

atom_test: atom_test.o
queue_test: queue_test.o
semaphore_test: semaphore_test.o semaphore.o

atom_test.o: atom.h
queue_test.o: queue.h
semaphore_test.o: semaphore.h

clean: 
	rm -f $(programs) *.o callgrind.out.*
