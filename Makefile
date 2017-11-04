CC := gcc
CFLAGS := -g -lm -O3

OBJ := rubiks.o codepage.o lang.o

lang := cubically

all: $(lang)

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

visualizer:
	$(CC) $(CFLAGS) -DVISUALIZER rubiks.c visualizer.c stdin.c -o visualizer

clean:
	rm -f $(OBJ)