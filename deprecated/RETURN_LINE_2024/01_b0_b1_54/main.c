#include <stdio.h>
#include "line_patt_generator.h"
#include "print_line.h"
#include "return_line.h"
#include "test_line.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
    int j, ret;
    int x, y, shift;
    char patt[LINE_PATT_BUFF_SZ];
    int pattSz;
    int rise, run, offset, length; 
    x=1; y=2; shift=0;
   
   
   //x=1; y=3; shift=1;
   //x=4; y=5; shift=0;
   //x=2; y=5; shift=4;
    for (j=0; j < 54; j++)
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
        
        printf("-------------------- x=%i; y=%i; shift=%i;\n", x, y, shift);
        printPattVis(patt, pattSz);

        if (returnLine(patt, pattSz, &rise, &run, &offset, &length))
        {
            printf("\n\n--ERROR--\n\n");
            break;
        }

        if (matchLineToPatt(rise, run, offset, length, patt, pattSz))
        {
            printf("\n\n--DOES NOT MATCH--\n\n");
            break;
        }

        shift++;
    }

    printf("-------------------- j:%i\n", j);
    printf("-------------------- x=%i; y=%i; shift=%i;\n", x, y, shift);
    return 0;
}
