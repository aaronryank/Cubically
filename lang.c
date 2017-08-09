#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include "rubiks.h"
#include "lang.h"

int32_t mem, input;

struct {
    long int pos;
    int faces[7];
} jumps[1000] = {0, {0}};
int parens, jumpnum;

int do_else;

FILE *in;

int main(int argc, char **argv)
{
    initcube();

    in = argc >= 2 ? fopen(argv[1],"r") : stdin;

    if (!in) {
        fprintf(stderr,"Error: could not open source file %s\n",argv[1]);
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
    else
        return (int32_t) (cube[face][0][0] + cube[face][0][1] + cube[face][0][2] + cube[face][1][0] + cube[face][1][1] + cube[face][1][2] + cube[face][2][0] + cube[face][2][1] + cube[face][2][2]);
}

int execute(int command, int arg)
{
    if (do_else && !(command == '!' && arg == -1))

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
        while (getc(in) != '}');
    }
    else {
        while (isdigit(c = getc(in)));
        ungetc(c,in);
    }
}
