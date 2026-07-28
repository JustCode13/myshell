# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -Iinclude

# Source files
SRC = main.c src/memory.c tests/test_memory.c

# Output executable
TARGET = program

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Remove generated files
clean:
	rm -f $(TARGET)

# Rebuild everything
re: clean all

# Prevent conflicts with files named 'all', 'clean', etc.
.PHONY: all run clean re