CC = g++
CFLAGS = -Wall -Wextra -std=c++98 -Iinclude -Iglad/include
LDFLAGS = -lSDL2 -lGL -lGLU -lglut

# Source files
CPP_SRC = src/main.cpp src/Shader.cpp src/Window.cpp
C_SRC = src/glad.c

# Object files
CPP_OBJ = $(CPP_SRC:.cpp=.o)
C_OBJ = $(C_SRC:.c=.o)
OBJ = $(CPP_OBJ) $(C_OBJ)

# Target executable
TARGET = FCyXt-98

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile C++ source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ)

fclean: clean
	rm $(TARGET)

re: fclean all

# Phony targets
.PHONY: all clean fclean