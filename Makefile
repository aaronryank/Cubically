CC := gcc
CFLAGS := -g -lm -O3

OBJ := src/rubiks.o src/codepage.o src/lang.o

lang := cubically

all: $(lang)

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

visualizer:
	$(CC) $(CFLAGS) -DVISUALIZER src/rubiks.c src/visualizer.c src/stdin.c -o visualizer

clean:
	rm -f $(OBJ)
