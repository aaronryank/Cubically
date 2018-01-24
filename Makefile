CC  := gcc
OBJ := src/rubiks.o src/codepage.o src/parse.o src/interp.o src/main.o src/solve/coordcube.o src/solve/cubiecube.o src/solve/facecube.o src/solve/search.o src/solve/solve.o

override CFLAGS += -g -O3 -lm

lang := cubically

all: $(lang)

$(lang): $(OBJ)
	$(CC) $(OBJ) -o $(lang) $(CFLAGS)

clean:
	rm -f $(OBJ)
