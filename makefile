CC := gcc
CFLAGS := -g -lm -O3

OBJ := rubiks.o codepage.o lang.o

lang := rubiks-lang

all: $(lang)

visualizer:
	$(CC) $(CFLAGS) -DVISUALIZER rubiks.c visualizer.c stdin.c -o visualizer

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

tio:
	gcc -lm -DTIO rubiks.c codepage.c lang.c -o rubiks-tio
clean:
	rm -f $(OBJ)

clean_visualizer:
	rm -f $(V_OBJ)
sandbox:
	$(CC) $(CFLAGS) rubiks.c sandbox.c -o sandbox
