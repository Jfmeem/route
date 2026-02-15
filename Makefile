CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -Iheaders
LDFLAGS = -lm

# Source files
SOURCES = main.cpp \
          src/mode.cpp \
          src/nodesAndEdges.cpp \
          src/csvParse.cpp \
          src/problem1.cpp \
          src/problem2.cpp \
          src/problem3.cpp \
          src/problem4.cpp \
          src/problem5.cpp \
          src/problem6.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = dhaka_transport

# Default target
all: $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Makefile targets:"
	@echo "  make        - Build the project"
	@echo "  make clean  - Remove build artifacts"
	@echo "  make run    - Build and run the program"
	@echo "  make help   - Show this help message"

.PHONY: all clean run help
