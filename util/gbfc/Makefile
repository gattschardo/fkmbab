target := bf2

all:

test: test.b
	./$(target) $<
	as -o $@.o $@.s
	ld -o $@ $@.o

all: $(target)
	chmod +x compile.sh

clean:
	$(RM) $(target)
