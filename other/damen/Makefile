CFLAGS = -g

HEADERS = damen.h
DOBJS = damen.o util.o
SOBJS = shell.o util.o
TARGS = damen shell

all: $(TARGS)

%.o: %.c $(HEADERS)

damen: $(DOBJS)

shell: $(SOBJS)

clean:
	rm -rf $(TARGS) $(DOBJS) $(SOBJS)
