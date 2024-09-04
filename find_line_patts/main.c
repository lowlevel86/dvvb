#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define patt_size 63


static int getLinePattBit(int x, int y, int loc)
{
   if ((loc * x / y) != ((loc + 1) * x / y))
   return 1;

   return 0;
}

static void getLinePatt(char *patt, int pattSz, int x, int y, int offset)
{
   int i;

   for (i=0; i < pattSz; i++)
   patt[i] = getLinePattBit(x, y, i+offset);
}


// FIND PATTERNS USING LINE BIT DATA
int main(void)
{
   int x, y, offset, i;
   int pattSz = patt_size;
   char pattA[patt_size] = {-1};
   char pattB[patt_size] = {-1};
   char *patt = pattA;
   char *patt_prior = pattB;
   char *patt_swap;
   int new_pattern;
   
   for (x=0; x < pattSz; x++)
   for (y=1; y <= pattSz; y++)
   for (offset=0; offset < pattSz; offset++)
   {
      getLinePatt(patt, pattSz, x, y, offset);

      new_pattern = FALSE;
      for (i=0; i < pattSz; i++)
      if (patt[i] != patt_prior[i])
      {
         new_pattern = TRUE;
         break;
      }
      
      if (new_pattern)
      {
         for (i=0; i < pattSz; i++)
         printf("%i", patt[i]);
         
         printf("\n");
      }
      
      patt_swap = patt_prior;
      patt_prior = patt;
      patt = patt_swap;
   }

   return 0;
}
