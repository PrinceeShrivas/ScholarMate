# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iinclude
SRCDIR = src
BUILDDIR = build
INCDIR = include

# Source files
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/core/scholarship.cpp \
          $(SRCDIR)/core/bst.cpp \
          $(SRCDIR)/core/uploader.cpp \
          $(SRCDIR)/utils/input_helpers.cpp \
          $(SRCDIR)/utils/string_utils.cpp \
          $(SRCDIR)/services/student_service.cpp \
          $(SRCDIR)/services/uploader_service.cpp

# Object files (Windows compatible)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Target executable (Windows .exe)
TARGET = $(BUILDDIR)/scholarmate.exe

# Default target
all: $(TARGET)

# Create build directories (Windows compatible)
$(BUILDDIR):
	if not exist "$(BUILDDIR)" mkdir "$(BUILDDIR)"
	if not exist "$(BUILDDIR)\core" mkdir "$(BUILDDIR)\core"
	if not exist "$(BUILDDIR)\utils" mkdir "$(BUILDDIR)\utils"
	if not exist "$(BUILDDIR)\services" mkdir "$(BUILDDIR)\services"

# Link object files to create executable
$(TARGET): $(BUILDDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files (Windows compatible)
clean:
	if exist "$(BUILDDIR)" rmdir /s /q "$(BUILDDIR)"

# Rebuild everything
rebuild: clean all

# Run the program (Windows .exe)
run: $(TARGET)
	$(TARGET)

# Show help
help:
	@echo Available targets:
	@echo   all      - Build the project (default)
	@echo   clean    - Remove build files
	@echo   rebuild  - Clean and build
	@echo   run      - Build and run the program
	@echo   help     - Show this help message

.PHONY: all clean rebuild run help