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

int32_t mem    = 0;
int32_t solved = 1;
int32_t input  = -1;
int pos;
int do_else;
int cur_depth;

struct {
    int pos;
    int faces[9];
} jumps[999];
int jumpnum;

#define CPC commands[pos].command
#define CPA commands[pos].arg

int call_command(void)
{
    /* if implicit, call with -1 */
    if (commands[pos].argc == 0) {
        return execute(CPC, -1);
    }

    /* functions require multiple-digit integers */
    if (CPC == 'f') {
        int i, val;
        /* so turn the array of one-digit integers into a single integer */
        for (i = val = 0; i < commands[pos].argc; i++) {
            val *= 10;
            val += CPA[i];
        }
        /* and execute `f` with that integer */
        return execute('f', val);
    }

    int i; 
    int exec; /* number of times the command has been executed.
                 this is needed for commands with superscripts/subscripts,
                 which need individual parsing via this loop but do not
                 immediately call the command. */

    for (i = exec = 0; i < commands[pos].argc; i++) {
        int arg = CPA[i];

        if (arg >= 100) {
            arg -= 100;

            if (issubscript(arg)) {
                cur_depth *= 10;
                cur_depth += numberize(arg);
                continue;
            }
            else if (issuperscript(arg)) {
                cur_depth *= 10;
                cur_depth += _faceval(numberize(arg));
                continue;
            }
            else { /* double-struck */
                arg = _faceval(numberize(arg));
            }
        }

        exec++;
        int ret = execute(CPC, arg);

        /* execute returns nonzero when it fails,
           quit early with a nonzero return value when it does. */
        if (ret)
            return ret;
    }

    /* loopers must be called implicitly,
       calling them with arguments simply cause the arguments to build off of each other */
    if (CPC == '(' || CPC == ')' || !exec)
        execute(CPC, -1);

    cur_depth = 0;
    return 0;
}

int interp(void)
{
    for (pos = 0; CPC; pos++) {
        /* print debugging information if flag set */
        if (DEBUG) {
            fprintf(stderr, "interpreting [%x] %c(", CPC, CPC);
            int i;

            /* print each argument */
            for (i = 0; i < commands[pos].argc; i++) {
                fprintf(stderr, "%d", CPA[i]);
                if (i != (commands[pos].argc - 1))
                    putc(',', stderr);
            }
            fprintf(stderr, ")\n");
        }

        /* call_command returns nonzero if it quits early;
           we need to too */
        if (call_command())
            break;
    }

    return 0;
}

int execute(int command, int arg)
{
    static int infuncdef = 0;             /* boolean; tells whether the interpreter is reading the first instance of a function */
    static int functions[1000] = {0};     /* stack of function starting positions */
    static int fc = 0;                    /* top of the functions stack */
    static int func_returns[1000] = {0};  /* stack of positions for the interpreter to return to when it exits a function */
    static int depth = 0;                 /* top of the func_returns stack */
    static int solvemode = 0;             /* tells whether the interpreter should exit when the cube becomes solved:
                                             -1 if the mode has been set but the cube was solved when it was set,
                                             1 if the cube has been unsolved since the mode was set */

    /* if the command-line flag for solvemode has been set,
        set the interpreter boolean */
    if (SOLVEMODE && !solvemode)
        solvemode = -1;

    DEBUG && fprintf(stderr, "Command %x, arg %d, depth %d\n", command, arg, cur_depth);

    /* if defining a function and the command doesn't exit a function's definition, don't interpret */
    if (infuncdef && command != 0x20) {
        DEBUG && fprintf(stderr, "Not interpreting; defining a function\n");
        return 0;
    }

    if (rubiksnotation(command) + 1) {
        int face  = rubiksnotation(command);
        int turns = abs(arg);
        turncube(face, turns % 4, cur_depth);
        PRINTMOVES && fprintf(stderr, "%c%d", command, turns);
    }
    else if (command == 'M') {
        turncube(LEFT, abs(arg), (CUBESIZE-1)/2);
    }
    else if (command == 'E') {
        turncube(DOWN, abs(arg), (CUBESIZE-1)/2);
    }
    else if (command == 'S') {
        turncube(FRONT, abs(arg), (CUBESIZE-1)/2);
    }
    else if (command == '+') {
        mem += faceval;
    }
    else if (command == '-') {
        mem -= faceval;
    }
    else if (command == '/') {
        if (arg == -1)
            input && (mem /= input);
        else
            faceval && (mem /= faceval);
    }
    else if (command == '*') {
        mem *= faceval;
    }
    else if (command == '=') {
        if (arg == -1)
            mem = (mem == input);
        else
            mem = (mem == faceval);
    }
    else if (command == '$') {
        int x = abs(arg);
        while (x--)
            if (scanf("%d", &input) < 0)
                input = 0;
    }
    else if (command == '~') {
        int x = abs(arg);
        while (x--)
            input = getchar();
    }
    else if (command == '%') {
        printf("%d", faceval);
        fflush(stdout);
    }
    else if (command == '@') {
        putchar(faceval);
        fflush(stdout);
    }
    else if (command == ':') {
        if (arg == -1)
            mem = input;
        else
            mem = faceval;
    }
    else if (command == '^') {
        mem = pow(mem, faceval);
    }
    else if (command == '<') {
        mem = (mem < faceval);
    }
    else if (command == '>') {
        mem = (mem > faceval);
    }
    else if (command == '_') {
        faceval && (mem %= faceval);
    }
    else if (command == '|') {
        mem |= faceval;
    }
    else if (command == 0x80) {
        mem ^= faceval;
    }
    else if (command == 0x81) {
        mem <<= faceval;
    }
    else if (command == 0x82) {
        mem >>= faceval;
    }
    else if (command == 0x83) {
        mem &= faceval;
    }
    else if (command == '&') {
        if (arg == -1 || faceval)
            return -1;
    }
    else if (command == '(') {
        if (arg == -1) {
            jumps[jumpnum++].pos = pos;
            DEBUG && fprintf(stderr, "jumps[jumpnum-1].pos = %d\n", pos);
        } else {
            jumps[jumpnum].faces[arg] = 1;
        }
    }
    else if (command == ')') {
        if (arg == -1)
            do_jump();
        else
            jumps[jumpnum].faces[arg] = 1;
    }
    else if (command == '!') {
        if (faceval)
            do_skip();
    }
    else if (command == '?') {
        if (!faceval)
            do_skip();
    }
    else if (command == 0x86) {
        cubically_evaluate();
    }
    else if (command == 'f') {
        if (arg == -1 || arg > fc)
            return 0;

        func_returns[depth++] = pos;
        pos = functions[arg];
        DEBUG && fprintf(stderr, "Jumped into function to %d\n", pos);
    }
    else if (command == 0x84) {
        functions[++fc] = pos;
        infuncdef = 1;
    }
    else if (command == 0x20) {
        if (infuncdef)
            infuncdef = 0;
        else if (depth) {
            pos = func_returns[--depth];
            DEBUG && fprintf(stderr, "Jumped out of function to %d\n", pos);
        }
    }
    else if (command == 's') {
        solvemode = _faceval(8) ? 1 : -1;
    }
    else if (command == 'r') {
        readcube(stdin);
    }

    if (solvemode == -1) {
        solvemode = _faceval(8) ? 1 : -1;
    } else if (solvemode == 1 && !_faceval(8)) {
        puts("Solved!");
        return 1;
    }

    return 0;
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

/* this logic is flawless but I hate this function with a burning passion */
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
                retval += (int32_t) CUBE(face, i, j);
        return retval;
    }
}

void do_skip(void)
{
    char c = commands[++pos].command;

    /* if we need to skip an entire block */
    if (c == '{') {
        int loop = 1, level = 0;
        while (loop) {
            c = commands[++pos].command;

            /* matching code blocks for nested conditionals */
            if (c == '{')
                level++;
            else if (c == '}')
                loop = level--;
        }
    }
}

void cubically_evaluate(void)
{
    char *buf = malloc(1024);
    memset(buf, 0, 1024);

    if (!fgets(buf, 1024, stdin))
        return;

    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = 0;

    int *str = parse_string(buf);
    DEBUG && fprintf(stderr, "Read: <%s>\n", (char*) str);
    command *cmds = parse_commands(str);
    int l; // number of new commands
    int r; // number of old commands remaining in source
    int i; // used for final insert loop

    for (l = 0; cmds[l].command; l++);
    l--; // no clue

    // IMPROVE copying args

    /* get number of commands remaining in source */
    for (r = pos; commands[r].command; r++);

    /* make space for the new commands */
    for (; r > pos; r--) {
        commands[r+l].command = commands[r].command;
        commands[r+l].argc = commands[r].argc;
        int _;
        for (_ = 0; _ < 100; _++)
            commands[r+l].arg[_] = commands[r].arg[_];
    }

    /* copy the new commands into the source */
    for (i = 0; cmds[i].command; i++) {
        commands[pos+i].command = cmds[i].command;
        commands[pos+i].argc = cmds[i].argc;
        int _;
        for (_ = 0; _ < 100; _++)
            commands[pos+i].arg[_] = cmds[i].arg[_];
    }

    /* we overwrite the current command; rewind a bit so that the new command gets interpreted */
    pos--;
}
