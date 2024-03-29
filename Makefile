# Compiler and linker flags
CC = g++ -g #-std=c++17 -Wall -Wextra -pedantic
CFLAGS = 
LDFLAGS = -I$(shell pwd) 

# Object files directory
OBJDIR = obj

# Output executable
TARGET = neoawk

# Source files and object files
SRCS = Parser.cpp ./SymbolTable/*.cpp  Library/*.cpp Visitors/*.cpp TypeManager.cpp ./SymbolTable/Types/*.cpp Compiler.cpp  Visitors/*.cpp SyntaxTree/*.cpp   SyntaxTree/Operators/*.cpp SyntaxTree/Operands/*.cpp SyntaxTree/Rvalues/*.cpp SyntaxTree/Lvalues/*.cpp SyntaxTree/Declarations/*.cpp Lexer.cpp     
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
	./$(TARGET) Examples/test1.c

.PHONY: all clean run
