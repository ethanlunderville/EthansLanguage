# Compiler and linker flags
CC = g++ -g
CFLAGS = -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -I$(shell pwd) 

# Object files directory
OBJDIR = obj

# Output executable
TARGET = program

# Source files and object files
SRCS = Visitors/*.cpp TypeManager.cpp ./SymbolTable/Types/*.cpp Compiler.cpp  Visitors/*.cpp SyntaxTree/*.cpp ./SymbolTable/*.cpp  SyntaxTree/Operators/*.cpp SyntaxTree/Operands/*.cpp SyntaxTree/Rvalues/*.cpp SyntaxTree/Lvalues/*.cpp Lexer.cpp Parser.cpp    
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
	./$(TARGET) $(ARGS)

.PHONY: all clean run
