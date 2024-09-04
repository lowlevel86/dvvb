#include <stdio.h>
#include "line_patt_generator.h"

int main(void)
{
    int i, j, ret;
    int x=1, y=0, shift=0;
    char patt[LINE_PATT_BUFF_SZ];
    int pattSz;

    for (j=0; j < 100000; j++)
    {
        ret = getTestLinePatt(x, y, shift, patt, &pattSz);
        
        if (ret == 1)
        {
            x = 1;
            y++;
            continue;
        }
        else if (ret == 2)
        {
            shift = 0;
            x++;
            continue;
        }
        else if (ret == 3)
        {
            printf("Line pattern is larger than buffer.");
            break;
        }
        
        printf("x:%i y:%i, shift:%i\n", x, y, shift);
        for (i=0; i < pattSz; i++){printf("%i", patt[i]);}printf("\n");//print patt

        shift++;
    }
    return 0;
}
