# Compiler and linker flags
CC = g++ -g
CFLAGS = -00 -std=c11 -Wall -Wextra -pedantic -lsfml-audio
LDFLAGS = -I$(shell pwd) 

# Object files directory
OBJDIR = obj

# Output executable
TARGET = program

# Source files and object files
SRCS = ./Table.cpp Compiler.cpp #SyntaxTree/*.cpp SyntaxTree/Operators/*.cpp Visitors/*.cpp
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile source files into object files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object files directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up object files and the executable
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET) $(ARGS)test

.PHONY: all clean run
