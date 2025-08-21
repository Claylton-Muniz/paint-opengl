CC = gcc
SRC = *.c
EXEC = paint

# Detecta o sistema
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    FLAGS = -lGL -lGLU -lglut
endif

ifeq ($(OS), Windows_NT)
    FLAGS = -lopengl32 -lglu32 -lfreeglut
endif

all:
	$(CC) $(SRC) -o $(EXEC) $(FLAGS) && ./$(EXEC)

clean:
	rm -f $(EXEC)
