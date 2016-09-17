# Compiler
CC = gcc

# Linker
LD = gcc

# Compilerflags
CFLAGS = -g -DDEBUG -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -pg

# Linkerflags
LDFLAGS = -lm -lglut -lGLU -lGL -lXmu -lXi -lX11 -gp -p

# Objektdateien
OBJ = main.o io.o scene.o logic.o drawing.o gen.o level.o vector.o animation.o collision.o

# Outfile
OUT = ueb02

.PHONY: clean

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

ueb02 : $(OBJ)
	$(LD) -o $(OUT) $(OBJ) $(LDFLAGS)
	chmod +x $(OUT)

clean:
	rm -rf $(OBJ) $(OUT) 
