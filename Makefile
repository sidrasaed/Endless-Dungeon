CC = cc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib

# For macOS specific linking
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

# For Linux
ifeq ($(shell uname), Linux)
    LDFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Executable name
EXEC = endless_dungeon

# Source files
SOURCES = endless_dungeon.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Main target
all: $(EXEC)

# Link object files to create executable
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(EXEC) $(OBJECTS)

# Run target
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run