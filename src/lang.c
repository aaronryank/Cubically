#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <locale.h>
#include "rubiks.h"
#include "lang.h"

/* globals */
int DEBUG, PRINTMOVES, SOLVEMODE;
int codepage;
command *commands;

/* optimizations */
extern int asize, *rotated;

int main(int argc, char **argv)
{
    /* we're reading Unicode */
    setlocale(LC_ALL, "C.UTF-8");
    setlocale(LC_CTYPE, "C.UTF-8");

    CUBESIZE = 3;

    /* incorrect number of args or flag is -h */
    if (argc < 3 || (argc == 2 && strchr(argv[1], 'h'))) {
        fprintf(stderr, "Usage: %s <flags> <file | string> <size>\n"           "\n"\
                        "Flags: f | second argument is a file"                 "\n"\
                        "       s | second argument is a string"               "\n"\
                        "       u | read file/string as UTF-8"                 "\n"\
                        "       c | read file/string as Cubically SBCS"        "\n", argv[0]);

        if (argc == 2 && strchr(argv[1], 'h'))
            fprintf(stderr, "       S | solve-mode"                       "\n"\
                            "       p | print moves as they are executed" "\n"\
                            "       d | print debugging information"      "\n"\
                            "       r | read a cube from standard input and set the memory cube to it\n");

        fprintf(stderr, "\n<size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.\n");
        return 0;
    }

    /* parse command-line arguments */
    int flag_arg = 0;   enum { _FILE = 1, STRING = 2};
    int readcubeflag = 0;
    int i, s = strlen(argv[1]);
    for (i = 0; i < s; i++) {
        switch (tolower(argv[1][i])) {
          case 'f': flag_arg   = _FILE; break;
          case 's': flag_arg   = STRING; break;
          case 'u': codepage   = CP_UTF8; break;
          case 'c': codepage   = CP_SBCS; break;
          case 'd': DEBUG      = 1; break;
          case 'p': PRINTMOVES = 1; break;
          case 'S': SOLVEMODE  = 1; break;
          case 'r': readcubeflag = 1; break;
        }
    }

    /* if argument 3 is present, interpret as a number, set cubesize to it */
    if (argv[3])
        CUBESIZE = atoi(argv[3]);

    /* if invalid... */
    if (CUBESIZE < 0)
        CUBESIZE = -CUBESIZE;
    if (CUBESIZE < 2)
        CUBESIZE = 3;

    initcube();

    /* optimizations to speed up turns */
    asize = 6 * CUBESIZE * CUBESIZE * sizeof(int);
    rotated = malloc(asize);

    if (readcubeflag)
        readcube(stdin);

    FILE *in;

    if (flag_arg == _FILE) {
        in = fopen(argv[2], "r");
    }
    else if (flag_arg == STRING) {
        in = fopen(".cubically.tmp", "w+");
        fwrite(argv[2], sizeof(char), strlen(argv[2]), in);
        fclose(in);
        in = fopen(".cubically.tmp", "r");
    }

    if (!in) {
        fprintf(stderr, "Could not open input file for reading\n");
        return 1;
    }

    /* parse Unicode/SBCS file into SBCS string */
    int *source = parse_file(in);
    fclose(in);

    /* parse SBCS string into command structure */
    commands = parse_commands(source);
    free(source);

    interp();

    /* debugging info at end of program (dump memory) */
    fprintf(stderr, "\nNotepad: %d\n\n", mem);
    printcube(stderr);

    /* remove temporary file if we created it */
    if (flag_arg == 2)
        remove(".cubically.tmp");

    free(cube);
    return 0;
}
