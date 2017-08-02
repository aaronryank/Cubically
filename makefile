CC := gcc
CFLAGS := -g -lm -O3

OBJ := rubiks.o stdin.o lang.o
V_OBJ := rubiks.o stdin.o visualizer.o

lang := rubiks-lang
visualizer := visualizer

all: $(lang)

$(visualizer): $(V_OBJ)
	$(CC) $(CFLAGS) -DVISUALIZER $(V_OBJ) -o $(visualizer)

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

tio:
	gcc -lm -DTIO rubiks.c lang.c -o rubiks-tio
