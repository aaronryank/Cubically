#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rubiks.h"

#define rubiksnotation(x) (x == 'U' ? 0 : x == 'L' ? 1 : x == 'F' ? 2 : x == 'R' ? 3 : x == 'B' ? 4 : x == 'D' ? 5 : -1)

int main(int argc, char **argv)
{
    initcube();

    FILE *in = argc > 2 ? fopen(argv[1],"r") : stdin;

    int loop = 1;
    while (loop) {
        int face  = getc(in);
        int turns = getc(in);

        if (rubiksnotation(face)+1) {
            face = rubiksnotation(face);
            turns = (turns == '\'') ? 3 : turns - '0';
            turncube(face,turns);
        } else if (face == 'E' || face == -1) {
            loop = 0;
        }
    }

    putchar('\n');
    printcube();
}
