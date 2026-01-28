CC = g++
CFLAGS = -Wall -Wextra -std=c++98 -Iinclude -Iglad/include
LDFLAGS = -lSDL2 -lGL -lGLU -lglut

# Object directory
OBJ_DIR = obj

# Source files (manually listed, not wildcards)
CPP_SOURCES = src/main.cpp src/Shader.cpp src/Window.cpp
C_SOURCES = src/glad.c

# Object files (automatically generated from sources)
CPP_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(CPP_SOURCES:.cpp=.o)))
C_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJ_FILES = $(CPP_OBJ) $(C_OBJ)

# Target executable
TARGET = FCyXt-98

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files to create executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Compile C++ files to obj directory
$(OBJ_DIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C files to obj directory
$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

# Phony targets
.PHONY: all clean fclean re