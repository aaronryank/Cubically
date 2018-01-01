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
#include "codepage.h"

int32_t mem    = 0;
int32_t solved = 1;
int32_t input  = -1;
int pos;
int do_else;
int cur_depth;

extern int DEBUG, codepage;

struct {
  int pos;
  int faces[9];
} jumps[999];
int jumpnum;

int call_command(void)
{
    if (commands[pos].arg == -1) {
        int ret = execute(commands[pos].command, commands[pos].arg);
        return ret;
    }

    char arg[20] = {0};
    int i = 0;
    sprintf(arg, "%d", commands[pos].arg);

    while (arg[i]) {
        int ret = execute(commands[pos].command, arg[i++] - '0');
        if (ret)
            return ret;
    }

    if (commands[pos].command == L'(' || commands[pos].command == L')')
        execute(commands[pos].command, -1);

    return 0;
}

int interp(void)
{
    while (commands[pos].command) {
        if (DEBUG)
            printf("interpreting %c(%d)\n",commands[pos].command, commands[pos].arg);

        if (call_command())
            break;

        pos++;
    }

    fprintf(stderr, "Notepad: %d\n\n", mem);
    printcube();

    return 0;
}

int execute(wint_t command, int arg)
{
    DEBUG && printf("Command %C (%d | %d), arg %d\n",command,command,wctob(command),arg);

    if (do_else && !(command == L'!' && arg == -1))
        do_else = 0;

    if (rubiksnotation(command)+1) {
        int face  = rubiksnotation(command);
        int turns = abs(arg);
        turncube(face,turns,cur_depth);
    }
    else if (command == L'M') {
        turncube(LEFT,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == L'E') {
        turncube(DOWN,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == L'S') {
        turncube(FRONT,abs(arg),(CUBESIZE-1)/2);
    }
    else if (command == L'+') {
        mem += faceval;
    }
    else if (command == L'-') {
        mem -= faceval;
    }
    else if (command == L'/') {
        if (arg == -1)
            input && (mem /= input);
        else
            faceval && (mem /= faceval);
    }
    else if (command == L'*') {
        mem *= faceval;
    }
    else if (command == L'=') {
        if (arg == -1)
            mem = (mem == input);
        else
            mem = (mem == faceval);
    }
    else if (command == L'$') {
        int x = abs(arg);
        while (x--)
        {
            int retval = scanf("%d",&input);
            if (retval < 0)
                input = 0;
        }
    }
    else if (command == L'~') {
        int x = abs(arg);
        while (x--)
            input = getchar();
    }
    else if (command == L'%') {
        printf("%d",faceval);
        fflush(stdout);
    }
    else if (command == L'@') {
        putchar(faceval);
        fflush(stdout);
    }
    else if (command == L':') {
        if (arg == -1)
            mem = input;
        else
            mem = faceval;
    }
    else if (command == L'^') {
        mem = pow(mem,faceval);
    }
    else if (command == L'<') {
        mem = (mem < faceval);
    }
    else if (command == L'>') {
        mem = (mem > faceval);
    }
    else if (command == L'_') {
        faceval && (mem %= faceval);
    }
    else if (command == L'|') {
        mem |= faceval;
    }
    else if ((codepage == CP_UTF8 && command == 0x2295) || (codepage == CP_SBCS && command == 0x80)) {
        mem ^= faceval;
    }
    else if ((codepage == CP_UTF8 && command == 0x00AB) || (codepage == CP_SBCS && command == 0x81)) {
        mem <<= faceval;
    }
    else if ((codepage == CP_UTF8 && command == 0x00BB) || (codepage == CP_SBCS && command == 0x82)) {
        mem >>= faceval;
    }
    else if ((codepage == CP_UTF8 && command == 0x00B7) || (codepage == CP_SBCS && command == 0x83)) {
        mem &= faceval;
    }
    else if (command == L'&') {
        if (arg == -1 || faceval)
            return -1;
    }
    else if (command == L'(') {
        if (arg == -1) {
            jumps[jumpnum++].pos = pos;
            DEBUG && fprintf(stderr,"jumps[jumpnum-1].pos = %d\n", pos);
        } else {
            jumps[jumpnum].faces[arg] = 1;
        }
    }
    else if (command == L')') {
        if (arg == -1)
            do_jump();
        else
            jumps[jumpnum].faces[arg] = 1;
    }
    else if (command == L'!') {
        if (faceval) {
            do_skip();
            do_else = 0;
        }
    }
    else if (command == L'?') {
        if (!faceval) {
            do_skip();
            do_else = 1;
        } else {
            do_else = 0;
        }
    }
    else if ((codepage == CP_UTF8 && command == 0xB6) || (codepage == CP_SBCS && command == 0x86)) {
        cubically_evaluate();
    }

    return 0;
}

int rubiksnotation(wint_t x)
{
    switch (x) {
        case L'U': return 0;
        case L'L': return 1;
        case L'F': return 2;
        case L'R': return 3;
        case L'B': return 4;
        case L'D': return 5;
        default:   return -1;
    }
}

int do_jump(void)
{
    int i, count, _do_jump1, _do_jump2;
    for (i = count = _do_jump1 = 0; i < 9; i++)
    {
        if (jumps[jumpnum-1].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump1 = 1;
        }
    }
    if (!count)
        _do_jump1 = 1;
    for (i = count = _do_jump2 = 0; i < 9; i++)
    {
        if (jumps[jumpnum].faces[i]) {
            count++;
            if (_faceval(i))
                _do_jump2 = 1;
        }
    }
    if (!count)
        _do_jump2 = 1;

    DEBUG && fprintf(stderr, "Jumping if %d&&%d (pos %d)\n", _do_jump1, _do_jump2, pos);

    if (_do_jump1 && _do_jump2) {
        pos = jumps[jumpnum-1].pos;
        DEBUG && fprintf(stderr, "Jumped (pos %d)\n", pos);
        clear_jump(0);
        return 1;
    }
    else {
        clear_jump(-1);
        clear_jump(0);
        jumpnum--;
        return 0;
    }
}

int32_t _faceval(int face)
{
    face = face == 9 ? 0 : face;
    if (face == 8)
        return !issolved();
    if (face == 7)
        return input;
    else if (face == 6 || face == -1)
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

void do_skip(void)
{
    command c = commands[pos++];

    if (c.command == L'{') {
        int loop = 1, level = 0;
        while (loop) {
            c = commands[pos++];
            if (c.command == L'{')
                level++;
            else if (c.command == L'}')
                loop = level--;
        }
    }
}

void cubically_evaluate(void)
{
    char *buf = malloc(1024);
    memset(buf, 0, 1024);

    if (!fgets(buf,1024,stdin))
        return;

    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = 0;

    wint_t *str = parse_string(buf);
    DEBUG && printf("Read: <%S>\n", str);
    command *cmds = parse_commands(str);
    int l; // number of new commands
    int r; // number of old commands remaining in source
    int i; // used for final insert loop

    for (l = 0; cmds[l].command; l++) { DEBUG && printf("Read: %c%d\n", cmds[l].command, cmds[l].arg); }
    l--; // no clue

    for (r = pos; commands[r].command; r++);
    for (; r > pos; r--) {
        commands[r+l].command = commands[r].command;
        commands[r+l].arg = commands[r].arg;
    }

    for (i = 0; cmds[i].command; i++) {
        commands[pos+i].command = cmds[i].command;
        commands[pos+i].arg = cmds[i].arg;
    }

    pos--;
}
