#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include "rubiks2.h"
#include "lang.h"

int32_t mem, input;

struct {
    long int pos;
    int faces[7];
} jumps[1000] = {0, {0}};
int parens, jumpnum;

int do_else;

FILE *in;

int CUBESIZE;

int main(int argc, char **argv)
{
    CUBESIZE = 3;

    if (argc < 3 || strchr(argv[1],'h')) {
        fprintf(stderr,"Usage: %s <flags> <file | string> <size>"             "\n"\
                       "Flags: f | second argument is a file"                 "\n"\
                       "       s | second argument is a string"               "\n"\
                       "       u | read file/string as UTF-8"                 "\n"\
                       "       c | read file/string as Cubically SBCS"        "\n"\
                       "<size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.\n",argv[0]);
        return -1;
    }

    int flag_arg = 0;   // 1 = file, 2 = string
    int flag_cp = 0;    // 1 = UTF-8, 2 = Cubically SBCS
    int i, s = strlen(argv[1]);
    for (i = 0; i < s; i++) {
        switch (tolower(argv[1][i])) {
          case 'f': flag_arg = 1; break;
          case 's': flag_arg = 2; break;
          case 'u': flag_cp  = 1; break;
          case 'c': flag_cp  = 2; break;
        }
    }

    if (argv[3])
        CUBESIZE = atoi(argv[3]);

    if (CUBESIZE < 0)
        CUBESIZE = -CUBESIZE;
    if (CUBESIZE < 2)
        CUBESIZE = 3;

    initcube();

    if (flag_arg == 1)
        in = fopen(argv[2],"r");
    else if (flag_arg == 2)
        in = fmemopen(argv[2],strlen(argv[2]),"r");

    if (!in) {
        fprintf(stderr,"Error: could not read %s `%s`: %s\n",flag_arg == 2 ? "string" : "file",argv[2],strerror(errno));
        return -1;
    }

    int loop = 1, command, args = 0;
    while (loop)
    {
        int c = getc(in);
        DEBUG && fprintf(stderr,"Read %c (%d)\n",c,c);

        if (isdigit(c)) {
            args++;
            if (command == '(' || command == ')')
                jumps[jumpnum].faces[c - '0'] = 1;
            else
                loop = execute(command,c - '0');
        } else {
            if (c == EOF)
                loop = 0;

            if ((!args && implicit(command)) || special(command)) {
                int retval = execute(command,-1);
                if (retval == -1)
                    loop = 0;
                else
                    loop = loop || retval;
            }

            DEBUG && fprintf(stderr,"executed %c (%d), loop = %d, pos = %ld\n",command,command,loop,ftell(in));

            command = c;
            args = 0;
        }
    }

    fprintf(dbg,"\nNotepad: %d\n\n",mem);
    printcube();

#ifdef LOOP_DEBUG
    int i, j;
    for (i = 0; i < jumpnum; i++) {
        printf("Jump %d: ", i);
        for (j = 0; j < 7; j++)
            printf("%d-%d ",j,jumps[i].faces[j]);
        printf("@%d\n",jumps[i].pos);
    }
#endif
}

int do_jump(void)
{
    int i, count, _do_jump1, _do_jump2;
    for (i = count = _do_jump1 = 0; i < 7; i++)
    {
        if (jumps[jumpnum-1].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump1 = 1;
        }
    }
    if (!count)
        _do_jump1 = 1;

    for (i = count = _do_jump2 = 0; i < 7; i++)
    {
        if (jumps[jumpnum].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump2 = 1;
        }
    }
    if (!count)
        _do_jump2 = 1;

    DEBUG && fprintf(stderr,"Jumping (fpos %ld)\n",ftell(in));

    if (_do_jump1 && _do_jump2) {
        fseek(in, jumps[jumpnum-1].pos, SEEK_SET);
        DEBUG && fprintf(stderr,"Jumped (fpos %ld)\n",ftell(in));
        clear_jump(0);
        return 1;
    }
    else {
        clear_jump(-1);
        jumpnum--;
        return 0;
    }
}

int32_t _faceval(int face)
{
    if (face == 7)
        return input;
    else if (face == 6)
        return mem;
    else {
        int32_t retval = 0;
        size_t i, j;
        for (i = 0; i < CUBESIZE; i++)
            for (j = 0; j < CUBESIZE; j++)
                retval += (int32_t) CUBE(face,i,j);
        return retval;
    }
}

int execute(int command, int arg)
{
    if (do_else && !(command == '!' && arg == -1))
        do_else = 0;

    if (rubiksnotation(command)+1) {
        int face  = rubiksnotation(command);
        int turns = arg;
        turncube(face,turns);
    }
    else if (command == '+') {
        mem += faceval;
    }
    else if (command == '-') {
        mem -= faceval;
    }
    else if (command == '/') {
        mem && (mem /= faceval);
    }
    else if (command == '*') {
        mem *= faceval;
    }
    else if (command == '=') {
        mem = (mem == faceval);
    }
    else if (command == '$') {
        int retval = scanf("%d",&input);
        if (retval < 0)
            input = 0;
    }
    else if (command == '~') {
        input = getchar();
    }
    else if (command == '%') {
        if (arg == 6)
            printf("%d",mem);
        else
            printf("%d",faceval);
        fflush(stdout);
    }
    else if (command == '@') {
        if (arg == 6)
            putchar(mem % 128);
        else
            putchar(faceval % 128);
        fflush(stdout);
    }
    else if (command == ':') {
        mem = faceval;
    }
    else if (command == '^') {
        mem = pow(mem,faceval);
    }
    else if (command == '<') {
        mem = (mem < faceval);
    }
    else if (command == '>') {
        mem = (mem > faceval);
    }
    else if (command == '_') {
        mem = (mem % faceval);
    }
    else if (command == '"') {
        mem = (mem & faceval);
    }
    else if (command == '|') {
        mem = (mem | faceval);
    }
    else if (command == '`') {
        mem = (mem ^ faceval);
    }
    else if (command == 'E' || command == '&') {
        if (arg == -1 || faceval)
            return -1;
    }
    else if (command == '(') {
        jumps[jumpnum++].pos = ftell(in) - 1;
        DEBUG && fprintf(stderr,"jumps[jumpnum-1].pos = %ld\n",jumps[jumpnum-1].pos);
    }
    else if (command == ')') {
        int retval = do_jump();
        return retval;
    }
    else if (command == '!') {
        if (faceval || (arg == -1 && do_else)) {
            do_skip();
            do_else = 0;
        }
    }
    else if (command == '?') {
        if (!faceval) {
            do_skip();
            do_else = 1;
        } else {
            do_else = 0;
        }
    }
    else if (command == EOF) {
        return 0;
    }

    return 1;
}

int rubiksnotation(char x)
{
    switch (x) {
      case 'U': return 0;
      case 'L': return 1;
      case 'F': return 2;
      case 'R': return 3;
      case 'B': return 4;
      case 'D': return 5;
      default:  return -1;
    }
}

void do_skip(void)
{
    int c = getc(in);
    if (c == '{') {
        int loop = 1, level = 0;
        while (loop) {
            c = getc(in);
            if (c == '{')
                level++;
            else if (c == '}')
                loop = level--;
        }
    }
    else {
        while (isdigit(c = getc(in)));
        ungetc(c,in);
    }
}
