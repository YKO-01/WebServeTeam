# Makefile for parsing project

# Compiler
CC = c++

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# Source files
SRCS = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = parsing

# Default target
all: $(TARGET) 

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS)
