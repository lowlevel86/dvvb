#include <stdio.h>


static int getLinePattBit(int x, int y, int loc)
{
    if ((loc * x / y) != ((loc + 1) * x / y))
    return 1;

    return 0;
}

int matchLineToPatt(int rise, int run, int offset,
                    int length, char *patt, int pattSz)
{
    int i;

    if (length-1 != pattSz)
    return 1;

    for (i=0; i < pattSz; i++)
    if (getLinePattBit(run, rise, offset+i) != patt[i])
    return 1;

    return 0;
}
