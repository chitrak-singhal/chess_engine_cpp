##############################
# mingw32-make             # Build without optimizations
# mingw32-make OPT=2       # Build with -O2 optimization
# mingw32-make OPT=3       # Build with -O3 optimization
# mingw32-make run         # Compiles and runs the program
# mingw32-make run OPT=2   # Compiles with -O2 and runs the program
# mingw32-make run OPT=3   # Compiles with -O2 and runs the program
# mingw32-make clean       # Remove only .o files
# mingw32-make clean-all   # Remove .o and .exe files
##############################

# Compiler
CXX := g++

# Optimization Flags (set by user)
ifeq ($(OPT),2)
    OPTIMIZE := -O2
else ifeq ($(OPT),3)
    OPTIMIZE := -O3
else
    OPTIMIZE := 
endif

# Compiler Flags
CXXFLAGS := -Wall -Wextra -std=c++17 $(OPTIMIZE)

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
