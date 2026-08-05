# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -fsanitize=address


# Source files
SRC = main.c \
      src/memory.c \
      src/prompt.c \
      src/error.c \
	  src/lexer.c \
      src/string_builder.c \
      tests/test_memory.c \
	  tests/test_prompt.c \
	  tests/test_lexer.c \

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
