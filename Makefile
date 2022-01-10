CC     = gcc
CFLAGS = -Wall -std=c11
DEPS   = src/all.h

SRCDIR=src
OBJDIR=obj

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJ_ENCODER=$(filter-out $(OBJDIR)/decoder.o, $(OBJ))
OBJ_DECODER=$(filter-out $(OBJDIR)/encoder.o, $(OBJ))

$(info $(shell mkdir -p $(OBJDIR)))

all: encode decode

encode: $(OBJ_ENCODER)
	$(CC) $(CFLAGS) $(OBJ_ENCODER) -o $@

decode: $(OBJ_DECODER)
	$(CC) $(CFLAGS) $(OBJ_DECODER) -o $@




debug: clean
debug: CFLAGS += -Wall -Wextra -pedantic -g -fsanitize=address
debug: encode
debug: decode


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*


