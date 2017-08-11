CC := gcc
CFLAGS := -g -lm -O3

OBJ := rubiks2.o codepage.o lang.o

lang := rubiks-lang

all: $(lang)

visualizer:
	$(CC) $(CFLAGS) -DVISUALIZER rubiks2.c visualizer.c stdin.c -o visualizer

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

tio:
	gcc -lm -DTIO rubiks2.c codepage.c lang.c -o rubiks-tio
clean:
	rm -f $(OBJ)

clean_visualizer:
	rm -f $(V_OBJ)
