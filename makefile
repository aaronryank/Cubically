all:
	gcc -g rubiks.c stdin.c visualizer.c -o visualizer
	gcc -g rubiks.c lang.c -o rubiks-lang
