#pragma once

#include <wchar.h>
#include <stdint.h>

enum { CP_UNDEF, CP_UTF8, CP_SBCS } codepages;

#define clear_jump(x)  for (i = 0; i < 9; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define issubscript(x)   (x >= 0x10 && x <= 0x19)
#define issuperscript(x) (x >= 0 && x <= 9)

int execute(int, int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);

typedef struct {
  int command;
  int arg[100];
  int argc;
} command;

int *parse_file(FILE *);
int *parse_string(char *);
command *parse_commands(int *);
int get_command(int *, int *, int *, int [], int *);

int interp(void);
void cubically_evaluate(void);
int rubiksnotation(char);

int sbcs_convert(wint_t);
int numberize(int);

extern int mem;
extern command *commands;
extern int DEBUG, PRINTMOVES, SOLVEMODE;
extern int codepage;
