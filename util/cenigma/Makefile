BINDIR := bin
OBJDIR := $(BINDIR)/objects
SRCDIR := src

vpath %.h include
vpath %.c src

CFLAGS += -Iinclude -ansi -g

create_bintree := $(eval $(shell mkdir -p $(OBJDIR)))

program := enigma
sources := util.c patchboard.c reflector.c rotor.c enigma.c main.c
objects := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(sources)))

.PHONY: all
all: $(BINDIR)/$(program)

.PHONY: clean
clean:
	$(RM) -r $(BINDIR)

$(BINDIR)/$(program): $(objects)
	$(LINK.c) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $< -o $@
