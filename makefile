all:
	gcc rubiks.c stdin.c visualizer.c -o visualizer
	gcc rubiks.c lang.c -o rubiks-lang
