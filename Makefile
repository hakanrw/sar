# Compiler
CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OBJ_DIR = obj
BUILD_DIR = bin
TEST_DIR = test
CLI_DIR = cli

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRCS)))))
LIB = $(BUILD_DIR)/libsar.so

SAR = $(BUILD_DIR)/sar
UNSAR = $(BUILD_DIR)/unsar

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -I$(INCLUDE_DIR)
LDFLAGS = -shared

# Targets
.PHONY: all clean install uninstall test


all: $(LIB) $(SAR) $(UNSAR)

$(LIB): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/sar.h $(OBJ_DIR)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(SAR): $(CLI_DIR)/sar.c $(INCLUDE_DIR)/sar.h
	$(CC) $(CFLAGS) -L$(BUILD_DIR) -Wl,-rpath=/usr/local/lib64 -o $@ $< -lsar

$(UNSAR): $(CLI_DIR)/unsar.c $(INCLUDE_DIR)/sar.h
	$(CC) $(CFLAGS) -L$(BUILD_DIR) -Wl,-rpath=/usr/local/lib64 -o $@ $< -lsar


clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)/*.o test/test

install:
	cp $(INCLUDE_DIR)/sar.h /usr/include
	cp $(LIB) /usr/local/lib64
	cp $(SAR) /usr/local/bin
	cp $(UNSAR) /usr/local/bin

uninstall:
	rm -f /usr/include/sar.h /usr/local/lib64/libsar.so /usr/local/bin/sar /usr/local/bin/unsar

test: all
	$(CC) $(CFLAGS) -L$(BUILD_DIR) $(TEST_DIR)/test.c -lsar -o $(TEST_DIR)/test
	LD_LIBRARY_PATH=$(BUILD_DIR) ./$(TEST_DIR)/test
