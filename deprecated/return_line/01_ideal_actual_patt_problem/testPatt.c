#include <stdio.h>


static int getLinePattBit(int x, int y, int loc)
{
   double d = (double)x / y;

   if ((int)(loc * d) != (int)((loc+1) * d))
   return 1;

   return 0;
}

static int getLinePatt(char *patt, int x, int y, int loc)
{
    int i, pattSz;

    pattSz = y - loc;

    for (i=0; i < pattSz; i++)
    patt[i] = getLinePattBit(x, y, loc+i);

    return pattSz;
}

int getTestPatt(int canvasSz, char *patt, int *pattSz)
{
    static int x=0, y=0, c=1;

    while (c < canvasSz)
    {
        while (x < c)
        {
            while (y < c)
            {
                *pattSz = getLinePatt(patt, x, c, y);
                y++;
                return 0;
            }
            y = 0;
            x++;
        }
        x = 0;
        c++;
    }

    return 1;
}

int matchLineToPatt(int x, int y, int offset, int remainder, char *patt, int pattSz)
{
    int i;
    int lineSz = y - remainder;

    if (lineSz-1 != pattSz)
    return 1;

    for (i=0; i < pattSz; i++)
    if (getLinePattBit(x, y, offset+i) != patt[i])
    return 1;

    return 0;
}
