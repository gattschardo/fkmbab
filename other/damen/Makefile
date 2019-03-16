CFLAGS = -g

HEADERS = damen.h warn.h
DOBJS = damen.o util.o
SOBJS = shell.o util.o
TARGS = damen shell

all: $(TARGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

damen: $(DOBJS)

shell: $(SOBJS)

clean:
	$(RM) $(TARGS) $(DOBJS) $(SOBJS)
