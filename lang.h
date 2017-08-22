#pragma once

#include <wchar.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#define clear_jump(x)  for (i = 0; i < 9; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define special(x)  (x == L'(' || x == L')')
#define implicit(x) (x == L'(' || x == L')' || x == L'$' || x == L'~' || x == L'&' || x == L'E' || x == L'!' || (rubiksnotation(x) + 1) || x == L'M' || x == L'E' || x == L'S')

int rubiksnotation(wint_t);
int execute(wint_t,int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);
