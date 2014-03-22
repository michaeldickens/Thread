CC = gcc

CFLAGS = -std=c99 -Wall

DEBUG_FLAGS = -g -O0 -fno-inline
PROFILE_FLAGS = -g -pg -O2 -fno-inline
RELEASE_FLAGS = -O2 -DNDEBUG

DEFAULT_FLAGS = $(DEBUG_FLAGS)

LDLIBS = -lm -lpthread

PROGRAMS = semaphore_test

all: $(PROGRAMS)
all: CFLAGS += $(DEFAULT_FLAGS)

debug: $(PROGRAMS)
debug: CFLAGS += $(DEBUG_FLAGS)

profile: $(PROGRAMS)
profile: CFLAGS += $(PROFILE_FLAGS)

release: $(PROGRAMS)
release: CFLAGS += $(RELEASE_FLAGS)

semaphore_test: semaphore_test.o semaphore.o

clean: 
	rm -f $(programs) *.o callgrind.out.*
