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
  wint_t command;
  int arg;
} command;
    
char *parse_file(FILE *);
char *parse_string(char *);
command *parse_commands(char *);

int interp(void);
void cubically_evaluate(void);
int rubiksnotation(char);

char sbcs_convert(wint_t);

extern command *commands;
extern int codepage;
