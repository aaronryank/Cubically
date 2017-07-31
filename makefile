all:
	gcc -g -lm -DVISUALIZER rubiks.c stdin.c visualizer.c -o visualizer
	gcc -g -lm rubiks.c lang.c -o rubiks-lang
tio:
	gcc -lm -DTIO rubiks.c lang.c -o rubiks-tio
