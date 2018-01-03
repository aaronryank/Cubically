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

int DEBUG;
int codepage;
command *commands;

int main(int argc, char **argv)
{
    /* we're reading Unicode */
    setlocale(LC_ALL,"C.UTF-8");
    setlocale(LC_CTYPE,"C.UTF-8");

    CUBESIZE = 3;

    /* incorrect number of args or flag is -h */
    if (argc < 3 || strchr(argv[1],'h')) {
        fprintf(stderr,"Usage: %s <flags> <file | string> <size>"             "\n"\
                       "Flags: f | second argument is a file"                 "\n"\
                       "       s | second argument is a string"               "\n"\
                       "       u | read file/string as UTF-8"                 "\n"\
                       "       c | read file/string as Cubically SBCS"        "\n"\
                       "<size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.\n", argv[0]);
        return 0;
    }

    int flag_arg = 0;   enum { _FILE = 1, STRING = 2};
    int i, s = strlen(argv[1]);
    for (i = 0; i < s; i++) {
        switch (tolower(argv[1][i])) {
          case 'f': flag_arg  = _FILE; break;
          case 's': flag_arg  = STRING; break;
          case 'u': codepage  = CP_UTF8; break;
          case 'c': codepage  = CP_SBCS; break;
          case 'd': DEBUG     = 1; break;
        }
    }

    // if argument 3 is present, interpret as a number, set cubesize to it
    if (argv[3])
        CUBESIZE = atoi(argv[3]);

    // if invalid...
    if (CUBESIZE < 0)
        CUBESIZE = -CUBESIZE;
    if (CUBESIZE < 2)
        CUBESIZE = 3;

    initcube();

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

    int *source = parse_file(in);
    fclose(in);

    commands = parse_commands(source);
    free(source);

    interp();

    if (flag_arg == 2)
        remove(".cubically.tmp");

    free(cube);
    return 0;
}
