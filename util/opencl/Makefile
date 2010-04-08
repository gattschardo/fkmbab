progs := test matrix1
objs := util.o $(addsuffix .o,$progs);

#CFLAGS += -g

.PHONY: all
all: $(progs)

test: util.o

matrix1: CFLAGS += --std=c99
matrix1: LDLIBS = -lOpenCL
matrix1: util.o

.PHONY: clean
clean:
	$(RM) $(objs)
