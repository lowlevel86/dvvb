#include "pattBitLookAhead.h"

static int getLinePattBit(int x, int y, int loc)
{
   double d = (double)x / y;

   if ((int)(loc * d) != (int)((loc-1) * d))
   return 1;

   return 0;
}

int iniPattBitLookAhead(int iniTailSz, struct infer_line_spec *il, int loc)
{
   if (loc >= il->bodySzA)
   {
      if (loc >= il->wholeTailSzB) // if in the head section
      {
         loc -= il->wholeTailSzB;

         if (il->headSzB > il->tailSzB)
         {
            if (loc > il->headSzB - il->tailSzB)
            loc -= il->headSzB - il->tailSzB;
         }
         else
         loc += il->tailSzB - il->headSzB;
      }
      
      loc = loc % il->tailSzB;
   }

   if (loc > il->wholeTailSzA) // if in the head section
   {
      loc -= il->wholeTailSzA;

      if (loc - il->headSzA)
      return 0;
      
      return 1;
   }

   if (loc % il->tailSzA)
   return 0;

   return 1;
}

int pattBitLookAhead(struct spec *l, struct infer_line_spec *il, int loc)
{
   if (loc > il->bodySzA)
   {
      if (loc > il->wholeTailSzB) // if in the head section
      {
         loc -= il->wholeTailSzB;

         if (il->headSzB > il->tailSzB)
         {
            if (loc > il->headSzB - il->tailSzB)
            loc -= il->headSzB - il->tailSzB;
         }
         else
         loc += il->tailSzB - il->headSzB;
      }

      loc = (loc-1) % il->tailSzB + 1;
   }

   if (loc > il->wholeTailSzA) // if in the head section
   {
      loc -= il->wholeTailSzA;

      if (il->headSzA > il->tailSzA)
      {
         if (loc > il->headSzA - il->tailSzA)
         loc -= il->headSzA - il->tailSzA;
      }
      else
      loc += il->tailSzA - il->headSzA;
   }
   
   loc = (loc-1) % il->tailSzA + 1;
   
   if (loc == l->y)
   return 1;

   return getLinePattBit(l->x, l->y, l->offset+loc);
}

