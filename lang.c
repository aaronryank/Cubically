#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rubiks.h"

#define rubiksnotation(x) (x == 'U' ? 0 : x == 'L' ? 1 : x == 'F' ? 2 : x == 'R' ? 3 : x == 'B' ? 4 : x == 'D' ? 5 : -1)

int main(int argc, char **argv)
{
    int turns, face;
    initcube();

    FILE *in = fopen(argv[1],"r");

    while (1) {
        if (fscanf(in," %c",&face) == EOF)
            break;
        if (fscanf(in," %d",&turns) == EOF)
            break;

        printf("{%c,%d}",face,turns);

        if (rubiksnotation(face)+1) {
            face = rubiksnotation(toupper(face));
            turns = (turns == '\'') ? 3 : turns - '0';
            turncube(face,turns);
        } else if (toupper(face) == 'E') {
            printf("End\n");
        }
    }

    putchar('\n');
    printcube();
}
