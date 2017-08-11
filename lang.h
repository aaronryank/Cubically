#pragma once

#pragma GCC diagnostic ignored "-Wunused-result"

#ifndef DEBUG
#define DEBUG 0
#endif

#define clear_jump(x)  for (i = 0; i < 8; i++)            \
                           jumps[jumpnum+x].faces[i] = 0; \
                       jumps[jumpnum+x].pos = 0;

#define faceval _faceval(arg)

#define special(x)  (x == L'(' || x == L')')
#define implicit(x) (x == L'(' || x == L')' || x == L'$' || x == L'~' || x == L'&' || x == L'E' || x == L'!')

// U+2070 SUPERSCRIPT 0
// U+00B9 SUPERSCRIPT 1
// U+00B2 SUPERSCRIPT 2
// U+00B3 SUPERSCRIPT 3
// U+2074 SUPERSCRIPT 4
// U+2075 SUPERSCRIPT 5
// U+2076 SUPERSCRIPT 6
// U+2077 SUPERSCRIPT 7
// U+2078 SUPERSCRIPT 8
// U+2079 SUPERSCRIPT 9

#define superscript_utf8(x) ((x == L'\u2070') || \
                             (x == L'\u00B9') || \
                             (x == L'\u00B2') || \
                             (x == L'\u00B3') || \
                             ((x >= L'\u2074') && (x <= L'\u2079')))

#define superscript_sbcs(x) ((x >= 0) && (x <= 9))

#define CP_UTF8 1
#define CP_SBCS 2

extern int unsuperscript(int,int);

int rubiksnotation(char);
int execute(int,int);
int do_jump(void);
int32_t _faceval(int);
void do_skip(void);
