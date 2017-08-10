int unsuperscript(int x, int flag)
{
    if (flag == 2)   // SBCS
        return x;

    switch (x) {
      case L'\u2070': return 0;
      case L'\u00B9': return 1;
      case L'\u00B2': return 2;
      case L'\u00B3': return 3;
      case L'\u2074': return 4;
      case L'\u2075': return 5;
      case L'\u2076': return 6;
      case L'\u2077': return 7;
      case L'\u2078': return 8;
      case L'\u2079': return 9;
      default: return 0;
    }
}
