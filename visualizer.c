#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rubiks.h"

#define u toupper
#define rubiksnotation(x) (u(x) == 'U' ? 0 : u(x) == 'L' ? 1 : u(x) == 'F' ? 2 : u(x) == 'R' ? 3 : u(x) == 'B' ? 4 : u(x) == 'D' ? 5 : -1)

extern void stdin_set(), stdin_reset();

int main(void)
{
    initcube();
    stdin_set();

    while (1) {
        printf("\e[H\e[2J");
        printcube();
        printf("\e[0m\n\nMoves: ");
        int move = getchar();
        if (move == 'E')
            stdin_reset(), exit(EXIT_SUCCESS);
        move = rubiksnotation(move);
        turncube(move,1);
    }
}
