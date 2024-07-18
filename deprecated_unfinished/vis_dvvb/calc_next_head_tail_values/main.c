#include <stdio.h>

// tcc main.c -run

#define more_steep 1
#define eq_steepness 0
#define less_steep -1

void calcNextPixPatSpecVals(int *headSz, int *tailSz, int tailCnt, int ternarySlopeVal)
{
   int headSize = *headSz;
   int tailSize = *tailSz;

   if (ternarySlopeVal == more_steep)// new slope is a little bit larger
   {
      *headSz = headSize + tailSize * tailCnt - tailSize;
      *tailSz = headSize + tailSize * tailCnt;
   }
   else if (ternarySlopeVal == eq_steepness)// new slope is equal
   {
      *headSz = headSize + tailSize * tailCnt;
      *tailSz = headSize + tailSize * tailCnt;
   }
   else if (ternarySlopeVal == less_steep)// new slope is a little bit smaller
   {
      *headSz = headSize + tailSize * tailCnt + tailSize;
      *tailSz = headSize + tailSize * tailCnt;
   }
}

int main(void)
{
   int i;
   int headSize;
   int tailSize;

   headSize = 1;
   tailSize = 2;
   
   printf("More Steep\n");
   for (i=0; i < 4; i++)
   {
      calcNextPixPatSpecVals(&headSize, &tailSize, 2, more_steep);
      printf("headSz:%i, tailSz:%i, tailCnt:2\n", headSize, tailSize);
   }

   headSize = 2;
   tailSize = 1;
   
   printf("\nLess Steep\n");
   for (i=0; i < 4; i++)
   {
      calcNextPixPatSpecVals(&headSize, &tailSize, 2, less_steep);
      printf("headSz:%i, tailSz:%i, tailCnt:2\n", headSize, tailSize);
   }

   return 0;
}
