#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rubiks2.h"

#define u toupper
#define rubiksnotation(x) (u(x) == 'U' ? 0 : u(x) == 'L' ? 1 : u(x) == 'F' ? 2 : u(x) == 'R' ? 3 : u(x) == 'B' ? 4 : u(x) == 'D' ? 5 : -1)

extern void stdin_set(), stdin_reset();

int main(int argc, char **argv)
{
    int move;

    if (argc > 1)
        CUBESIZE = atoi(argv[1]);

    if (CUBESIZE < 0)
        CUBESIZE = -CUBESIZE;
    if (CUBESIZE < 2)
        CUBESIZE = 3;

    initcube();
    stdin_set();

    while (1) {
        printf("\e[H\e[2J");
        printcube();
        move = getchar();

        if (move == 'E')
            stdin_reset(), exit(EXIT_SUCCESS);

        move = rubiksnotation(move);
        turncube(move,1);
    }
}
