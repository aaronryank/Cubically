CC := gcc
CFLAGS := -g -lm -O3

OBJ := rubiks2.o codepage.o lang.o
V_OBJ := rubiks2.o stdin.o visualizer.o

lang := rubiks-lang
visualizer := visualizer

all: $(lang)

$(visualizer): $(V_OBJ)
	$(CC) $(CFLAGS) -DVISUALIZER $(V_OBJ) -o $(visualizer)

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

tio:
	gcc -lm -DTIO rubiks2.c codepage.c lang.c -o rubiks-tio
clean:
	rm -f $(OBJ)

clean_visualizer:
	rm -f $(V_OBJ)
