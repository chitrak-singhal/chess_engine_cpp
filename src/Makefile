##############################
#mingw32-make        # Build all .cpp files
#mingw32-make run    # Compiles and runs the program
#mingw32-make clean  # Remove only .o files
#mingw32-make clean-all  # Remove .o and .exe files
##############################

# Compiler
CXX := g++

# Compiler Flags
CXXFLAGS := -Wall -Wextra -std=c++17

# Find all .cpp files in the current directory
SRCS := $(wildcard *.cpp)

# Convert .cpp files to .o files
OBJS := $(SRCS:.cpp=.o)

# Output Executable
TARGET := Domino.exe

# Default Build Rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation Rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rules
clean:
	del /Q $(OBJS)  # Delete only .o files

clean-all:
	del /Q $(OBJS) $(TARGET)  # Delete .o and .exe files

# Run the compiled program
run: all
	@echo "Running $(TARGET)..."
	@$(TARGET)
