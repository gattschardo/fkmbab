CLFAGS = -Wall
#DEFINES = -DARCH
TESTOBJS = fbtest.o ftk.o
TETROBJS = fbtetris.o blocks.o ftk.o
HASHOBJS = fhashira.o blocks.o ftk.o hashlogic.o

HEADERS = ftk.h blocks.h hashlogic.h
OBJS = $(TESTOBJS) $(TETROBJS) $(HASHOBJS)
TARGETS = fbtest fbtetris fhashira

.PHONY: all
all: $(TARGETS)

fbtest: $(TESTOBJS)

fbtetris: $(TETROBJS)

fhashira: $(HASHOBJS)

%.o: %.c $(HEADERS)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGETS)
