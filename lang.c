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

        else if (isdigit(c)) {
            if (command == '(' || command == ')')
                jumps[jumpnum].faces[c - '0'] = 1;
            else
                execute(command,c - '0');
        } else {
            if (command == '(')
                jumps[jumpnum++].pos = ftell(in);
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
    int i, _do_jump;
    for (i = _do_jump = 0; i < 7; i++)
        if (jumps[jumpnum-1].faces[i])
            if (i==7?input:i==6?mem:cube[i][0][0]+cube[i][0][1]+cube[i][0][2]+cube[i][1][0]+cube[i][1][1]+cube[i][1][2]+cube[i][2][0]+cube[i][2][1]+cube[i][2][2])
                _do_jump = 1;

    if (_do_jump)
        fseek(in, jumps[jumpnum-1].pos, SEEK_SET);
    else
        jumpnum--;
}

#define faceval (arg == 6 ? mem : arg == 7 ? input : (cube[arg][0][0] + cube[arg][0][1] + cube[arg][0][2] + cube[arg][1][0] + cube[arg][1][1] + cube[arg][1][2] + cube[arg][2][0] + cube[arg][2][1] + cube[arg][2][2]))

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
