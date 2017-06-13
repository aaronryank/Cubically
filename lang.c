#include <stdio.h>
#include "rubiks.h"

#define rubiksnotation(x) (x == 'U' ? 0 : x == 'L' ? 1 : x == 'F' ? 2 : x == 'R' ? 3 : x == 'B' ? 4 : x == 'D' ? 5 : -1)

int main(void)
{
    initcube();

    while (1) {
        printf("\e[H\e[2J");
        printcube();
        printf("\e[0m\n\nMoves: ");
        int move = getchar();
        move = rubiksnotation(move);
        turncube(move,1);
    }

/* this will be used in the language
    FILE *in = fopen("code.txt","r");

    while (!feof(in)) {
        int face  = getc(in);
        int turns = getc(in);
        face  = face["ULFRBD"];
        turns = (turns == '\'') ? 3 : turns - '0';
        turncube(face,turns);
    }
*/
}
