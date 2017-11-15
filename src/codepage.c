#include <wchar.h>
#include "codepage.h"

int unsuperscript(wint_t x, int flag)
{
    if (flag == CP_SBCS)
        return x;
    else if (flag == CP_UTF8) {
        switch (x) {
          case 0x2070: return 0;
          case 0x00B9: return 1;
          case 0x00B2: return 2;
          case 0x00B3: return 3;
          case 0x2074: return 4;
          case 0x2075: return 5;
          case 0x2076: return 6;
          case 0x2077: return 7;
          case 0x2078: return 8;
          case 0x2079: return 9;
          default: return 0;
       }
   }
}

int unsubscript(wint_t x, int flag)
{
    if (flag == CP_SBCS)
        return x - 0x10;
    else if (flag == CP_UTF8)
        return x - 0x2080;
}

int undoublestruck(wint_t x, int flag)
{
    if (flag == CP_SBCS)
        return x - 0x90;
    else if (flag == CP_UTF8)
        return x - 0x1D7D8;
}
