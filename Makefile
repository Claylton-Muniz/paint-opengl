CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lGL -lGLU -lglut

SRC = paint.c gl_utils.c screen_utils.c
EXEC = paint

all:
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXEC)
