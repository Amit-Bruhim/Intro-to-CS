# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Source directories
SRC_DIR = .
UTILS_DIR = ./utils

# Main executable
MAIN = main

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
UTILS_SRC = $(UTILS_DIR)/utils.c
EX2_SRC = $(SRC_DIR)/Ex2/ex2.c
EX3_SRC = $(SRC_DIR)/Ex3/ex3.c
EX4_SRC = $(SRC_DIR)/Ex4/ex4.c $(SRC_DIR)/Ex4/main.c
EX5_SRC = $(SRC_DIR)/Ex5/ex5.c
EX6_SRC = $(SRC_DIR)/Ex6/ex6.c

# Header files
EX4_HDR = $(SRC_DIR)/Ex4/ex4.h

# Executables for each exercise
EX2_OUT = $(SRC_DIR)/Ex2/a.out
EX3_OUT = $(SRC_DIR)/Ex3/a.out
EX4_OUT = $(SRC_DIR)/Ex4/a.out
EX5_OUT = $(SRC_DIR)/Ex5/a.out
EX6_OUT = $(SRC_DIR)/Ex6/a.out

# Default target: build everything
all: $(MAIN) $(EX2_OUT) $(EX3_OUT) $(EX4_OUT) $(EX5_OUT) $(EX6_OUT)

# Main executable depends on main.c and utils
$(MAIN): $(MAIN_SRC) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(MAIN_SRC) $(UTILS_SRC) -o $(MAIN)

# Exercise 2
$(EX2_OUT): $(EX2_SRC) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(EX2_SRC) $(UTILS_SRC) -o $(EX2_OUT)

# Exercise 3
$(EX3_OUT): $(EX3_SRC) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(EX3_SRC) $(UTILS_SRC) -o $(EX3_OUT)

# Exercise 4
$(EX4_OUT): $(EX4_SRC) $(EX4_HDR) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(EX4_SRC) $(UTILS_SRC) -I Ex4 -o $(EX4_OUT)

# Exercise 5
$(EX5_OUT): $(EX5_SRC) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(EX5_SRC) $(UTILS_SRC) -o $(EX5_OUT)

# Exercise 6
$(EX6_OUT): $(EX6_SRC) $(UTILS_SRC)
	$(CC) $(CFLAGS) $(EX6_SRC) $(UTILS_SRC) -o $(EX6_OUT)

# Run main
run: $(MAIN)
	./$(MAIN)

# Clean all binaries
clean:
	rm -f $(MAIN) $(EX2_OUT) $(EX3_OUT) $(EX4_OUT) $(EX5_OUT) $(EX6_OUT)

# Compile only a specific exercise, e.g., make ex2 or make ex3
ex2: $(EX2_OUT)
ex3: $(EX3_OUT)
ex4: $(EX4_OUT)
ex5: $(EX5_OUT)
ex6: $(EX6_OUT)
