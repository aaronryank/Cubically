#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rubiks.h"

int mem;

int rubiksnotation(char);
int execute(int,int);

int main(int argc, char **argv)
{
    initcube();

    FILE *in = argc > 2 ? fopen(argv[1],"r") : stdin;

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

#define faceval (cube[face][0][0] + cube[face][0][1] + cube[face][0][2] + cube[face][1][0] + cube[face][1][1] + cube[face][1][2] + cube[face][2][0] + cube[face][2][1] + cube[face][2][2])

int execute(int command, int argument)
{
     if (rubiksnotation(command)+1) {
        int face  = rubiksnotation(command);
        int turns = argument;
        turncube(face,turns);
    }
    else if (command == '+') {
        int face = argument;
        mem += faceval;
    }
    else if (command == '-') {
        int face = argument;
        mem -= faceval;
    }
    else if (command == '/') {
        int face = argument;
        mem /= faceval;
    }
    else if (command == '*') {
        int face = argument;
        mem *= faceval;
    }
    else if (command == '=') {
        int face = argument;
        mem = (mem == faceval);
    }
    else if (command == '%') {
        int face = argument;
        if (face == 6)
            printf("%d",mem);
        else
            printf("%d",faceval);
    }
    else if (command == '@') {
        int face = argument;
        if (face == 6)
            putchar(mem);
        else
            putchar(faceval);
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
