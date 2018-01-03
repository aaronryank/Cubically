#pragma once

#include <wchar.h>
#include <stdint.h>

enum { CP_UNDEF, CP_UTF8, CP_SBCS } codepages;

#define clear_jump(x)  for (i = 0; i < 9; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define special(x)  (x == L'(' || x == L')')

int execute(char, int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);

typedef struct {
  int command;
  int arg;
} command;
    
int *parse_file(FILE *);
int *parse_string(char *);
command *parse_commands(int *);

int interp(void);
void cubically_evaluate(void);
int rubiksnotation(char);

int sbcs_convert(wint_t);
int numberize(int);

extern command *commands;
extern int codepage;
