#include <stdio.h>
#include <stdlib.h>
#include "testPatt.h"
#include "getLine.h"
#include "printData.h"

#define canvasSz 100 //9

int main(void)
{
   int i, x, y, offset, remainder;
   char patt[canvasSz-1];
   int pattSz;
   int ret;
   
   for (i=0; i < 1000000; i++)
   {
      if (getTestPatt(canvasSz, patt, &pattSz))
      {
         printf("end of testing\n");
         printf(" --------------------------- %i\n", i);
         return 0;
      }
      
      if (i < 94890)
      continue;

      ret = getLine(patt, pattSz, &x, &y, &offset, &remainder);

      if (ret == -1)
      continue; // skip unwanted test lines

      //if (i == 3)
      //{x = 2;y = 5;offset = 3;remainder = 2;}

      if (matchLineToPatt(x, y, offset, remainder, patt, pattSz))
      {
         printPattVis(patt, pattSz);
         printPatt(patt, pattSz);
         printLinePattVis(x, y, offset, remainder);
         printLinePatt(x, y, offset, remainder);
         printf("line does not match pattern\n");
         printf(" --------------------------- %i\n", i);
         return 0;
      }
   }

   printf(" --------------------------- %i\n", --i);
   
   return 0;
}
