#include <stdio.h>
#include "line_patt_generator.h"

#define TRUE 1
#define FALSE 0

static int getLinePattBit(int x, int y, int loc)
{
   double d = (double)x / y;

   if ((int)(loc * d) != (int)((loc+1) * d))
   return 1;

   return 0;
}

static int getLinePatt(char *patt, int x, int y, int shift)
{
    int i;

    for (i=0; i < y; i++)
    patt[i] = getLinePattBit(x, y, i+shift);

    return i;
}

int getTestLinePatt(int x, int y, int shift, char *patt, int *pattSz)
{
    if (x >= y)
    return 1;

    if (shift >= y)
    return 2;

    if (y >= LINE_PATT_BUFF_SZ)
    return 3;

    *pattSz = getLinePatt(patt, x, y, shift);
    return 0;
}
