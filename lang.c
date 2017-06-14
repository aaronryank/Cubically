#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rubiks.h"

int mem;
int jumps[1000], jnum;

int rubiksnotation(char);
int execute(int,int);

FILE *in;

int main(int argc, char **argv)
{
    initcube();

    in = argc >= 2 ? fopen(argv[1],"r") : stdin;

    int loop = 1, command;
    while (loop) {
        int c = getc(in);
        if (c == EOF)
            loop = 0;
        else if (isdigit(c))
            loop = execute(command,c - '0');
        else
            command = c;
    }

    printf("\nMemory: %d\n\n",mem);
    printcube();
}

#define faceval (cube[arg][0][0] + cube[arg][0][1] + cube[arg][0][2] + cube[arg][1][0] + cube[arg][1][1] + cube[arg][1][2] + cube[arg][2][0] + cube[arg][2][1] + cube[arg][2][2])

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
    else if (command == '%') {
        if (arg == 6)
            printf("%d",mem);
        else
            printf("%d",faceval);
    }
    else if (command == '@') {
        if (arg == 6)
            putchar(mem);
        else
            putchar(faceval);
    }
    else if (command == ':') {
        mem = faceval;
    }
    else if (command == '(') {
        jumps[jnum++] = ftell(in);
    }
    else if (command == ')') {
        if (jnum) {
            fseek(in,jumps[jnum-1],SEEK_SET);
        }
    }
    else if (command == ']') {
        if (jnum && mem > 0)
            fseek(in,jumps[jnum-1],SEEK_SET);
    }
    else if (command == 'E') {
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
