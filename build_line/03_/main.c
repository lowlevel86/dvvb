#include <stdio.h>
#include "build_line.h"
#include "print_line.h"

#define true 1

void printVisual(struct line *l)
{
   printf("headSz:%i tailSegSz:%i tailSegCnt:%i\n", l->headSz, l->tailSegSz, l->tailSegCnt);
   printf("x:%i y:%i offset:%i\n", l->x, l->y, l->offset);
   printf("headSzPrior:%i tailSegSzPrior:%i tailSegCntPrior:%i\n", l->headSzPrior, l->tailSegSzPrior, l->tailSegCntPrior);
   printf("xPrior:%i xPriorPrior:%i\n", l->xPrior, l->xPriorPrior);
   printLinePattVis5(l->y, l->x, l->offset, l->length);
}

// Pattern initialization for testing:
// isisp i OK
// imsmi p OK
// ilmi l l OK
// ilmli l OK
// illi l OK
// ill l OK
// ilis OK
// ilis pi OK
// iili pc OK

int main()
{
   char input;
   struct line l;
   
   printf("Enter [r] to reset.\n");
   printf("Enter [q] to exit.\n\n");
   
   printf("Enter [s] to add a new smaller head segment.\n");
   printf("Enter [m] to add a new medium sized top segment.\n");
   printf("Enter [u] to undo the medium sized top segment.\n");
   printf("Enter [l] to add a new larger head segment.\n\n");
   
   printf("Enter [S] to convert the larger head to the smaller head.\n");
   printf("Enter [L] to convert the smaller head to the larger head.\n\n");

   printf("Enter [i] to increment the tail segment count.\n");
   printf("Enter [d] to decrement the tail segment count.\n");
   printf("Enter [p] to increment the prior tail segment count.\n");
   printf("Enter [P] to decrement the prior tail segment count.\n\n");

   printf("Enter [t] to truncate the line.\n");
   printf("Enter [v] to reverse truncation.\n");
   printf("Enter [x] to extend the line.\n\n");

   iniBuildLine(&l);
   printVisual(&l);

   while (true)
   {
      input = getchar();
      
      switch(input)
      {
         case 'r': //reset
            iniBuildLine(&l);
            printVisual(&l);
            break;
         
         case 's':
            newSegSm(&l);
            printVisual(&l);
            break;
         case 'm':
            newSegMd(&l);
            printVisual(&l);
            break;
         case 'u':
            undoSegMd(&l);
            printVisual(&l);
            break;
         case 'l':
            newSegLg(&l);
            printVisual(&l);
            break;
         
         case 'S':
            convertLgToSm(&l);
            printVisual(&l);
            break;
         case 'L':
            convertSmToLg(&l);
            printVisual(&l);
            break;
         
         case 'i':
            incTailSegCnt(&l);
            printVisual(&l);
            break;
         case 'd':
            decTailSegCnt(&l);
            printVisual(&l);
            break;
         case 'p':
            incPriorTailSegCnt(&l);
            printVisual(&l);
            break;
         case 'P':
            decPriorTailSegCnt(&l);
            printVisual(&l);
            break;
         
         case 't':
            truncate(&l, 1);
            printVisual(&l);
            break;
         case 'v':
            undoTruncate(&l, 1);
            printVisual(&l);
            break;
         case 'x':
            extend(&l, 1);
            printVisual(&l);
            break;
         
         case 'q':
            return 0;
      }
   }
   
   return 0;
}
