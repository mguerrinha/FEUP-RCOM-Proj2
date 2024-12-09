# Makefile to build the project
# NOTE: This file must not be changed.

# Parameters
CC = gcc
CFLAGS = -Wall

SRC = src/
INCLUDE = include/
BIN = bin/

# We can change this URL.
URL = ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

# Targets
.PHONY: all
all: $(BIN)/download

$(BIN)/download: main.c $(SRC)/*.c
	# Create bin/ directory if it doesn't exist
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDE)

.PHONY: run
run: $(BIN)/download
	./$(BIN)/download $(URL)

.PHONY: clean
clean:
	rm -f $(BIN)/download
