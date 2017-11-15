#pragma once

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

#define superscript_utf8(x) ((x == 0x2070) || \
                             (x == 0x00B9) || \
                             (x == 0x00B2) || \
                             (x == 0x00B3) || \
                             ((x >= 0x2074) && (x <= 0x2079)))

#define subscript_utf8(x) ((x >= 0x2080) && (x <= 0x2089))

#define superscript_sbcs(x) ((x >= 0x00) && (x <= 0x09))
#define subscript_sbcs(x) ((x >= 0x10) && (x <= 0x19))

#define doublestruck_utf8(x) ((x >= 0x1D7D8) && (x <= 0x1D7E1))
#define doublestruck_sbcs(x) ((x >= 0x90) && (x <= 0x99))

#define CP_UTF8 1
#define CP_SBCS 2

extern int unsuperscript(wint_t,int);
extern int unsubscript(wint_t,int);
extern int undoublestruck(wint_t,int);
