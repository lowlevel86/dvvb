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

   void invertIF(char *patt, int pattSz)
   {
      int i;
      int invert = FALSE;
      
      for (i=1; i < pattSz; i++)
      if (patt[i])
      if (patt[i] == patt[i-1])
      invert = TRUE;
      
      if (invert)
      for (i=0; i < pattSz; i++)
      patt[i] = !patt[i];
   }
   
    x=1; y=2; shift=0;
   
//x=5; y=12; shift=10;
//x=4; y=9; shift=7;
//x=3; y=7; shift=0;
//x=5; y=12; shift=0;
//x=5; y=13; shift=0;
//x=5; y=13; shift=5;
//x=4; y=11; shift=8;
//x=5; y=13; shift=8;
//x=5; y=13; shift=9;
//x=6; y=13; shift=7;
//x=5; y=12; shift=7;
//x=5; y=14; shift=8;
    for (j=0; j < 2000; j++)
    {
        ret = getTestLinePatt(x, y, shift, patt, &pattSz);
        
        invertIF(patt, pattSz);
        
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

        if (getLine(patt, pattSz, &rise, &run, &offset, &length))
        {
            printf("\n\n--NOT A LINE--\n\n");
            printPatt(patt, pattSz);
            break;
        }

        if (matchLineToPatt(rise, run, offset, length, patt, pattSz))
        {
            printf("\n\n--DOES NOT MATCH--\n\n");
            //printPattVis(patt, pattSz);
            //printLinePattVis2(ret_x, ret_y, ret_offset, ret_remainder);
            break;
        }

        shift++;
    }

    printf("-------------------- j:%i\n", j);
    printf("-------------------- x=%i; y=%i; shift=%i;\n", x, y, shift);
    return 0;
}
