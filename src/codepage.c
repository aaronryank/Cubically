#include <wchar.h>

int numberize(int x)
{
    if (x >= 0x90 && x <= 0x99)
        return x - 0x90;
    else if (x >= 0x10 && x <= 0x19)
        return x - 0x10;
    return x;
}

int sbcs_convert(wint_t x)
{
    if (x >= 0x2080 && x <= 0x2089)
        return x - 0x2080 + 0x10;
    else if (x >= 0x1D7D8 && x <= 0x1D7E1)
        return x - 0x1D7D8 + 0x90;

    switch (x) {
      /* superscripts */
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
      /* miscellaneous */
      case 0x2295: return 0x80;
      case 0xAB:   return 0x81;
      case 0xBB:   return 0x82;
      case 0xB7:   return 0x83;
      case 0x21D2: return 0x84;
      case 0x2192: return 0x85;
      case 0xB6:   return 0x86; 
   }

   return x; 
}
