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
    else if (x >= 0x24D0 && x <= 0x24E9)
        return x - 0x24D0 + 0xC0;

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
      case 0xB6:   return 0x86;
      case 0x25A0: return 0x87;
      case 0x25A6: return 0x88;
      case 0x21AC: return 0x89;
      case 0x21AB: return 0x8A;
      case 0x2192: return 0x8B;
      case 0x2190: return 0x8C;
      case 0x2194: return 0x8D;
      case 10:     return 0x20;
      case 0x1E5A: return 0xA0;
      case 0x1E36: return 0xA1;
      case 0x1EE4: return 0xA2;
      case 0x1E0C: return 0xA3;
      case 0x1E1E: return 0xA4;
      case 0x1E04: return 0xA5;
      case 0x1E42: return 0xA6;
      case 0x1EB8: return 0xA7;
      case 0x1E62: return 0xA8;
      case 0x1E5B: return 0xB0;
      case 0x1E37: return 0xB1;
      case 0x1EE5: return 0xB2;
      case 0x1E0D: return 0xB3;
      case 0x1E1F: return 0xB4;
      case 0x1E05: return 0xB5;
      case 0x1E43: return 0xB6;
      case 0x1EB9: return 0xB7;
      case 0x1E63: return 0xB8;
      case 0x1D540:return 0x9A;
   }

   return x;
}
