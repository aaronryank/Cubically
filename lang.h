#pragma once

#pragma GCC diagnostic ignored "-Wunused-result"

#ifndef DEBUG
#define DEBUG 0
#endif

#define clear_jump(x)  for (i = 0; i < 8; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define special(x)  (x == '(' || x == ')')
#define implicit(x) (x == '(' || x == ')' || x == '$' || x == '~' || x == '&' || x == 'E' || x == '!')

int rubiksnotation(char);
int execute(int,int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);
