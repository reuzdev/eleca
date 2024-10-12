CC=gcc
CFLAGS=-Wall -Wextra
SRCS=$(wildcard src/*.c)
BIN_DIR=bin
BINARY=eleca

$(BINARY): $(BIN_DIR) $(SRCS)
	$(CC) -o $(BIN_DIR)/$(BINARY) $(SRCS) $(CFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

.PHONY: clean
