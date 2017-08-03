#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "rubiks.h"

#pragma GCC diagnostic ignored "-Wunused-result"

int mem, input;

struct {
  int pos, faces[7];
} jumps[1000] = {0, {0}};
int parens, jumpnum;

int rubiksnotation(char);
int execute(int,int);
int do_jump(void);
int _faceval(int);

FILE *in;

int main(int argc, char **argv)
{
    initcube();

    in = argc >= 2 ? fopen(argv[1],"r") : stdin;

    int loop = 1, command;
    while (loop)
    {
        int c = getc(in);

        if (c == EOF)
            loop = 0;

        if (isdigit(c)) {
            if (command == '(' || command == ')')
                jumps[jumpnum].faces[c - '0'] = 1;
            else
                loop = loop && execute(command,c - '0');
        } else {
            if (command == '(')
                jumps[jumpnum++].pos = ftell(in) - 1;
            else if (command == ')')
                do_jump();
            command = c;
        }
    }

#ifdef DEBUG
        printf("Command %c argument %c\n",command,isdigit(c)?c:command);
#endif

    fprintf(dbg,"\nNotepad: %d\n\n",mem);
    printcube();

    int i, j;
    for (i = 0; i < jumpnum; i++) {
        printf("Jump %d: ", i);
        for (j = 0; j < 7; j++)
            printf("%d-%d ",j,jumps[i].faces[j]);
        printf("@%d\n",jumps[i].pos);
    }
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

    if (_do_jump1 && _do_jump2)
        fseek(in, jumps[jumpnum-1].pos, SEEK_SET);
    else
        jumpnum--;
}

int _faceval(int face)
{
    if (face == 7)
        return input;
    else if (face == 6)
        return mem;
    else
        return (cube[face][0][0] + cube[face][0][1] + cube[face][0][2] + cube[face][1][0] + cube[face][1][1] + cube[face][1][2] + cube[face][2][0] + cube[face][2][1] + cube[face][2][2]);
}

#define faceval _faceval(arg)

int execute(int command, int arg)
{
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
        mem /= faceval;
    }
    else if (command == '*') {
        mem *= faceval;
    }
    else if (command == '=') {
        mem = (mem == faceval);
    }
    else if (command == '$') {
        scanf("%d",&input);
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
/*
    else if (command == '(') {
        jumps[jnum++] = ftell(in);
    }
    else if (command == ')') {
        if (jnum) {
            fseek(in,jumps[jnum-1],SEEK_SET);
        }
    }
    else if (command == ']') {
        if (jnum && !issolved())
            fseek(in,jumps[jnum-1],SEEK_SET);
    }
*/
    else if (command == 'E' || command == '&') {
        if (faceval)
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
