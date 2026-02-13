CC = gcc
CXX = g++

FLAGS_OPTIMISATION = -O3 -flto
FLAGS_SANITIZE = -fsanitize=address,undefined,leak
FLAGS_WARN = -Wall -Wextra -Wpadded
#FLAGS_WARN = -Wall -Wextra -Wpadded -Weffc++

CFLAGS = $(FLAGS_WARN) $(FLAGS_OPTIMISATION) $(FLAGS_SANITIZE) -Iinclude
CXXFLAGS = $(FLAGS_WARN) -std=c++98 -pedantic $(FLAGS_OPTIMISATION) $(FLAGS_SANITIZE) -Iinclude

LDFLAGS = -lSDL2 -lGL -lGLU -lglut -ldl $(FLAGS_OPTIMISATION) $(FLAGS_SANITIZE)

# Object directory
OBJ_DIR = obj

# Source files
CPP_SOURCES = src/main.cpp src/VertexShader.cpp src/FragmentShader.cpp src/Window.cpp src/Fractal.cpp src/Program.cpp src/App.cpp
C_SOURCES = src/glad.c

# Object files
CPP_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(CPP_SOURCES:.cpp=.o)))
C_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJ_FILES = $(CPP_OBJ) $(C_OBJ)
DEP_FILES = $(OBJ_FILES:.o=.d)

# Target executable
TARGET = Raymarching

# Default target
all: $(TARGET)

# Create obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Compile C++ files
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Compile C files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

# Include dependencies only if they exist
-include $(DEP_FILES)

.PHONY: all clean fclean re