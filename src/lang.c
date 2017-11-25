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

int DEBUG = 0;

int32_t mem   = 0;
int32_t input = -1;

struct {
    long int pos;
    int faces[9];
} jumps[1000];
int parens, jumpnum, jumped;

int do_else;   /* ?7{if}!{else} */
int skip;

FILE *in;

int cur_depth;  /* depth of face turn */

struct {
    int codepage;
    int string;
} flags;

wint_t *code;
int codepos;

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
                       "<size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.\n",argv[0]);
        return -1;
    }

    int flag_arg = 0;   // 1 = file, 2 = string
    int flag_cp = 0;    // 1 = UTF-8, 2 = Cubically SBCS
    int flag_save = 0;  // 0 = discard tmpfile, 1 = save tmpfile
    int i, s = strlen(argv[1]);
    for (i = 0; i < s; i++) {
        switch (tolower(argv[1][i])) {
          case 'f': flag_arg  = 1; break;
          case 's': flag_arg  = 2; break;
          case 'u': flag_cp   = 1; break;
          case 'c': flag_cp   = 2; break;
          case 't': flag_save = 1; break;
          case 'd': DEBUG     = 1; break;
        }
    }

    flags.codepage = flag_cp;     // universal
    flags.string = flag_arg - 1;

    // if argument 3 is present, interpret as a number, set cubesize to it
    if (argv[3])
        CUBESIZE = atoi(argv[3]);

    // if invalid...
    if (CUBESIZE < 0)
        CUBESIZE = -CUBESIZE;
    if (CUBESIZE < 2)
        CUBESIZE = 3;

    initcube();

    code = malloc(1024 * sizeof(wint_t));
    memset(code,0,1024);

    if (flag_arg == 1) {           // file
        in = fopen(argv[2],"r");

        if (!in) {
            fprintf(stderr,"Error: could not read %s `%s`: %s\n",flag_arg == 2 ? "string" : "file",argv[2],strerror(errno));
            return -1;
        }

        wint_t c;
        int i = 0;
        while ((c = getwc(in)) != WEOF) {
            if (i && !(i % 1024))
                code = realloc(code,i + 1024 * sizeof(wint_t));
            code[i] = c;

            if (c != L' ' && c != L'\n' && c != L'\t')
                i++;
        }
        code[++i] = WEOF;
        fclose(in);
    }
    else if (flag_arg == 2) {      // string (broken)
        int i = 0;
        while (i < strlen(argv[2])) {
            if (i && !(i % 1024))
                code = realloc(code,i + 1024 * sizeof(wint_t));
            code[i] = btowc(argv[2][i]);

            if (argv[2][i] != ' ' || argv[2][i] != '\n' || argv[2][i] != '\t')
                i++;
        }
        code[++i] = WEOF;
    }

    if (DEBUG)
        for (i = 0; code[i]; i++)
            printf("%C",code[i]);
    DEBUG && puts("");

    int loop = 1, args = 0;
    codepos = 0;
    wint_t command, c;
    while (loop)
    {
        c = code[codepos++];
        DEBUG && fprintf(stderr,"Read %C (%d)\n",c,wctob(c));

        /* if superscript number */
        if ((flag_cp == CP_UTF8 && superscript_utf8(c)) || (flag_cp == CP_SBCS && superscript_sbcs(c))) {
            cur_depth += _faceval(unsuperscript(c,flag_cp));
        }
        /* if subscript number */
        else if ((flag_cp == CP_UTF8 && subscript_utf8(c)) || (flag_cp == CP_SBCS && subscript_sbcs(c))) {
            cur_depth *= 10;
            cur_depth += unsubscript(c,flag_cp);
        }

        /* double struck digits */
        else if ((flag_cp == CP_UTF8 && doublestruck_utf8(c)) || (flag_cp == CP_SBCS && doublestruck_sbcs(c))) {
            int x = undoublestruck(c,flag_cp);
            loop = execute(command,_faceval(x));
        }

        /* digit */
        else if (isdigit(wctob(c))) {
            args++;
            if (command == L'(' || command == L')')
                jumps[jumpnum].faces[wctob(c) - '0'] = 1;
            else
                loop = execute(command,wctob(c) - '0');
        }

        else if (c == L'\'') {
            args++;
            loop = execute(command,3);
        }

        /* new command */
        else {
            if (c == WEOF || c == 0)   /* wide-char end-of-file */
                loop = 0;

            /* if no args passed or if it is a special command */
            if (!args || special(command)) {
                int retval = execute(command,-1);
                if (retval == -1)
                    loop = 0;
                else
                    loop = loop || retval;
            }

            DEBUG && fprintf(stderr,"executed %C (%d), loop = %d, pos = %ld\n",command,wctob(command),loop,ftell(in));

            if (!jumped) {
                command = c;    /* set new command */
            } else {
                jumped = 0;
                command = ' ';
            }
            args = 0;           /* reset argument count */
            cur_depth = 0;      /* reset layer depth of face rotation */
        }
    }

    /* debugging information */
    fprintf(dbg,"\nNotepad: %d\n\n",mem);
    printcube();

#ifdef LOOP_DEBUG
    int i, j;
    for (i = 0; i < jumpnum; i++) {
        printf("Jump %d: ", i);
        for (j = 0; j < 9; j++)
            printf("%d-%d ",j,jumps[i].faces[j]);
        printf("@%d\n",jumps[i].pos);
    }
#endif

    free(cube);
    return 0;
}

int do_jump(void)
{
#ifdef LOOP_DEBUG
    int j;
    for (j = 0; j < 9; j++)
        printf("%d ",jumps[jumpnum-1].faces[j]);
    puts("");
    for (j = 0; j < 9; j++)
        printf("%d ",jumps[jumpnum].faces[j]);
    puts("");
#endif

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

    DEBUG && fprintf(stderr,"Jumping (pos %d)\n",codepos);

    if (_do_jump1 && _do_jump2) {
        codepos = jumps[jumpnum-1].pos;
        DEBUG && fprintf(stderr,"Jumped (pos %d)\n",codepos);
        clear_jump(0);
        jumped = 1;
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
                retval += (int32_t) CUBE(face,i,j);
        return retval;
    }
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
        mem && (mem /= faceval);
    }
    else if (command == L'*') {
        mem *= faceval;
    }
    else if (command == L'=') {
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
        mem %= faceval;
    }
    else if (command == L'|') {
        mem |= faceval;
    }
    else if ((flags.codepage == CP_UTF8 && command == 0x2295) || (flags.codepage == CP_SBCS && command == 0x80)) {
        mem ^= faceval;
    }
    else if ((flags.codepage == CP_UTF8 && command == 0x00AB) || (flags.codepage == CP_SBCS && command == 0x81)) {
        mem <<= faceval;
    }
    else if ((flags.codepage == CP_UTF8 && command == 0x00BB) || (flags.codepage == CP_SBCS && command == 0x82)) {
        mem >>= faceval;
    }
    else if ((flags.codepage == CP_UTF8 && command == 0x00B7) || (flags.codepage == CP_SBCS && command == 0x83)) {
        mem &= faceval;
    }
    else if (command == L'&') {
        if (arg == -1 || faceval)
            return -1;
    }
    else if (command == L'(') {
        jumps[jumpnum++].pos = codepos - 1;
        DEBUG && fprintf(stderr,"jumps[jumpnum-1].pos = %d\n",codepos);
    }
    else if (command == L')') {
        int retval = do_jump();
        return retval;
    }
    else if (command == L'!') {
        if (faceval || !(arg == -1 && do_else)) {
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
    else if (command == EOF || command == WEOF || command == 0) {
        return 0;
    }

    return 1;
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

void do_skip(void)
{
    wint_t c = code[codepos++];

    if (c == L'{') {
        int loop = 1, level = 0;
        while (loop) {
            c = code[codepos++];
            if (c == L'{')
                level++;
            else if (c == L'}')
                loop = level--;
        }
    }
    else {
        while (isdigit(wctob(c = code[codepos++])));
        codepos--;
    }
}
