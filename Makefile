# Compiler to use
CC = gcc

# Include directories
INCLUDES = -I./include -I./src

# Library directories
LIBDIRS = -L./lib

# Linker flags
LDFLAGS = -lntdll

# Full path to the library
LIBS = ./lib/libMinHook.x64.lib

# Source files
SOURCES = src/main.c src/hook.c src/rawData.c

# Output executable names
DEBUG_OUTPUT = debug.exe
RELEASE_OUTPUT = release.exe

# Compiler flags for debug and release builds
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Build targets
all: release

debug: $(SOURCES)
	$(CC) $(DEBUG_FLAGS) $(INCLUDES) $(SOURCES) -o $(DEBUG_OUTPUT) $(LIBDIRS) $(LIBS) $(LDFLAGS)

release: $(SOURCES)
	$(CC) $(RELEASE_FLAGS) $(INCLUDES) $(SOURCES) -o $(RELEASE_OUTPUT) $(LIBDIRS) $(LIBS) $(LDFLAGS)

# Clean target
clean:
	rm -f $(DEBUG_OUTPUT) $(RELEASE_OUTPUT)
