CFLAGS = -g

HEADERS = damen.h warn.h
BOBJS = batch.o damen.o
SOBJS = shell.o damen.o
TARGS = batch shell

all: $(TARGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

batch: $(BOBJS)

shell: $(SOBJS)

clean:
	$(RM) $(TARGS) $(BOBJS) $(SOBJS)

test: all
	./test.sh

.phony: all clean test
