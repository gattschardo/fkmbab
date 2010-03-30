# make

# config
#CFLAGS := -Wall -Wextra -march=native
CFLAGS := -Wall -Wextra -ggdb #-pg --coverage

OBJS := stdio-dfuncs.o nc-dfuncs.o pmt.o nct.o st.o prettymaze.o
programs := pmtest nctest sgtest ripoff prettymaze

# rules follow
#

# ALL:
.PHONY: all
all: $(programs)

sgtest: sgt.c
	$(CC) $(CFLAGS) -o $@ $<

ripoff: ripoff.c

# STDIO dumper:
pmtest: pmt.o stdio-dfuncs.o
	$(CC) $(CFLAGS) -DSTDIO -o $@ stdio-dfuncs.o pmt.o

pmt.o: pmt.c
	$(CC) $(CFLAGS) -c -DSTDIO -o $@ $<

stdio-dfuncs.o: dfuncs.c
	$(CC) $(CFLAGS) -c -DSTDIO -o $@ $<

# NCURSES versions:
nctest: nc-dfuncs.o nct.o
	$(CC) $(CFLAGS) -lncurses -DNCURSES -o $@ nc-dfuncs.o nct.o

nc-dfuncs.o: dfuncs.c
	$(CC) $(CFLAGS) -c -DNCURSES -o $@ $<

nct.o: nct.c
	$(CC) $(CFLAGS) -c -DNCURSES -o $@ $<

prettymaze: nc-dfuncs.o prettymaze.o
	$(CC) $(CFLAGS) -lncurses -DNCURSES -o $@ nc-dfuncs.o prettymaze.o

prettymaze.o: prettymaze.c
	$(CC) $(CFLAGS) -c -DNCURSES -o $@ $<

# CLEAN:
.PHONY: clean
clean:
	$(RM) $(OBJS) $(programs) *.gcda *.gcno gmon.out *.profile

# vim: set nodigraph:
