# Makefile to build the project
# NOTE: This file must not be changed.

# Parameters
CC = gcc
CFLAGS = -Wall

SRC = src/
INCLUDE = include/
BIN = bin/

# We can change this URL.
#URL = ftp://rcom:rcom@ftp.netlab.fe.up.pt/README
#URL = ftp://rcom:rcom@ftp.netlab.fe.up.pt/pipe.txt
#URL = ftp://rcom:rcom@ftp.netlab.fe.up.pt/files/crab.mp4
#URL = ftp://anonymous:anonymous@ftp.bit.nl/speedtest/100mb.bin
URL = ftp://demo:password@test.rebex.net/readme.txt
#URL = ftp://ftp.up.pt/pub/gnu/emacs/elisp-manual-21-2.8.tar.gz

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
