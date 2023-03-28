#include <stdio.h>
#include "build_line.h"
#include "print_line.h"

#define true 1

void printVisual(struct line *l)
{
   printf("headSz:%i tailSegSz:%i tailSegCnt:%i\n", l->headSz, l->tailSegSz, l->tailSegCnt);
   printf("x:%i y:%i offset:%i\n\n", l->x, l->y, l->offset);
   printLinePattVis3(l->xParent, l->yParent, l->offset, l->remainder);
}

int main()
{
   char input;
   struct line l;
   
   printf("Enter [h] to add a new larger head.\n");
   printf("Enter [c] to convert the larger head to a smaller head.\n");
   printf("Enter [i] to increment the tail segment count.\n");
   printf("Enter [p] to increment the prior tail segment count.\n");
   printf("Enter [q] to exit.\n\n");
   
   iniBuildLine(&l);
   printVisual(&l);
   
   while (true)
   {
      input = getchar();
      
      switch(input)
      {
         case 'h':
            newHeadLg(&l);
            printVisual(&l);
            break;
         case 'c':
            if (l.tailSegCntPrior == 1)
            printf("'tailSegCntPrior' is equal to 1\n");
            else
            {
               convertLgToSm(&l);
               printVisual(&l);
            }
            break;
         case 'i':
            incTailSegCnt(&l);
            printVisual(&l);
            break;
         case 'p':
            incPriorTailSegCnt(&l);
            printVisual(&l);
            break;
         case 'q':
            return 0;
      }
   }
   
   return 0;
}
