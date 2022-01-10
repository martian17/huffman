CC     = gcc
CFLAGS = -Wall -std=c11
DEPS   = src/all.h

SRCDIR=src
OBJDIR=obj

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

$(info $(shell mkdir -p $(OBJDIR)))

all: prog

prog: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

debug: clean
debug: CFLAGS += -Wall -Wextra -pedantic -g -fsanitize=address
debug: prog

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*