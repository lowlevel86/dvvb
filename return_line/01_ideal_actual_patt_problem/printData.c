#include <stdio.h>


static int getLinePattBit(int x, int y, int loc)
{
    double d = (double)x / y;

    if ((int)(loc * d) != (int)((loc+1) * d))
    return 1;

    return 0;
}

void printPatt(char *patt, int pattSz)
{
    int i;

    printf("test patt:");

    for (i=0; i < pattSz; i++)
    printf("%i", patt[i]);

    printf("\n");
}

void printLinePatt(int x, int y, int offset, int remainder)
{
    int i;
    int pattSz = y - remainder - 1;

    printf("line patt:");

    for (i=0; i < pattSz; i++)
    printf("%i", getLinePattBit(x, y, offset+i));

    printf("\n");
}

void printPattVis(char *patt, int pattSz)
{
    int i, j;
    int tab = 0;

    printf("test patt visual\n");

    for (i=0; i < pattSz; i++)
    if (patt[i])
    tab++;

    for (i=pattSz-1; i >= 0; i--)
    {
        for (j=0; j < tab; j++)
        printf("  ");

        if (patt[i])
        {
            printf(" _|_|\n");
            tab--;
        }
        else
        printf("  |_|\n");
    }

    printf("  |_|\n");
}

void printLinePattVis(int x, int y, int offset, int remainder)
{
    int i, j;
    int pattSz = y - remainder - 1;
    int tab = 0;

    printf("line patt visual\n");

    for (i=0; i < pattSz; i++)
    if (getLinePattBit(x, y, offset+i))
    tab++;

    for (i=pattSz-1; i >= 0; i--)
    {
        for (j=0; j < tab; j++)
        printf("  ");

        if (getLinePattBit(x, y, offset+i))
        {
            printf(" _|_|\n");
            tab--;
        }
        else
        printf("  |_|\n");
    }

    printf("  |_|\n");
}

void printLinePattVis2(int x, int y, int offset, int remainder)
{
    int i, j;
    int pattSz = y - 1;
    int tab = 0;

    printf("line patt visual\n");

    for (i=0; i < pattSz; i++)
    if (getLinePattBit(x, y, offset+i))
    tab++;

    for (i=pattSz-1; i >= 0; i--)
    {
        for (j=0; j < tab; j++)
        printf("  ");

        if (remainder > 0)
        {
            if (getLinePattBit(x, y, offset+i))
            {
                printf(" _|#|\n");
                tab--;
            }
            else
            printf("  |#|\n");
        }
        else
        {
            if (getLinePattBit(x, y, offset+i))
            {
                printf(" _|_|\n");
                tab--;
            }
            else
            printf("  |_|\n");
        }
        remainder--;
    }

    printf("  |_|\n");
}
