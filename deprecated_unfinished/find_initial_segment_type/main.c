#include <stdio.h>
#include <string.h>
#include "build_line.h"
#include "print_line.h"

#define TRUE 1
#define FALSE 0

#define SM_INI_SEG 1 // the initial segment is smaller than the first segment
#define EQ_INI_SEG 2 // the initial segment is equal to the first segment
#define LG_INI_SEG 3 // the initial segment is larger than the first segment

int ht_determinant = 0;
int calc_ht_determinant = 0;

static int getLinePattBit(int x, int y, int loc)
{
   if ((loc * x / y) != ((loc + 1) * x / y))
   return 1;

   return 0;
}

int linesMatch(int xA, int yA, int offsetA, int sizeA,
               int xB, int yB, int offsetB, int sizeB)
{
   int i, size;

   if (sizeA < sizeB)
   {
      size = sizeA;
      offsetB += sizeB - sizeA;
   }
   else
   {
      size = sizeB;
      offsetA += sizeA - sizeB;
   }
   
   for (i=0; i < size; i++)
   if (getLinePattBit(xA, yA, offsetA+i) != getLinePattBit(xB, yB, offsetB+i))
   return FALSE;

   return TRUE;
}

int findIniSegType_bruteForce(struct line *mainSeg, struct line *cmpSeg, int truncateInc)
{
   int fitsSmSeg, fitsMdSeg, fitsLgSeg;
   
   int headSz, tailSegSz, tailCnt;
   int mainSeg_offset, mainSeg_size;
   int sm_cmpSeg_offset, sm_cmpSeg_size;
   int md_cmpSeg_offset, md_cmpSeg_size;
   int lg_cmpSeg_offset, lg_cmpSeg_size;
   
   headSz = cmpSeg->extraSmHead->headSzPrior;
   tailSegSz = cmpSeg->extraSmHead->tailSegSzPrior;
   tailCnt = cmpSeg->extraSmHead->tailSegCntPrior;
   sm_cmpSeg_offset = tailSegSz * tailCnt;
   sm_cmpSeg_size = headSz + tailSegSz * (tailCnt-1) + headSz;
   
   headSz = cmpSeg->extraMdHead->headSz;
   tailSegSz = cmpSeg->extraMdHead->tailSegSz;
   tailCnt = cmpSeg->extraMdHead->tailSegCnt;
   md_cmpSeg_offset = tailSegSz * tailCnt;
   md_cmpSeg_size = headSz + tailSegSz * tailCnt + headSz;
   
   headSz = cmpSeg->extraSmHead->headSzPrior;
   tailSegSz = cmpSeg->extraSmHead->tailSegSzPrior;
   tailCnt = cmpSeg->extraSmHead->tailSegCntPrior;
   lg_cmpSeg_offset = tailSegSz * tailCnt;
   lg_cmpSeg_size = headSz + tailSegSz * (tailCnt+1) + headSz;
   
   // find the main segment offset and size
   if (mainSeg->y == cmpSeg->extraSmHead->y)
   {
      mainSeg_offset = sm_cmpSeg_offset + truncateInc;
      mainSeg_size = sm_cmpSeg_size - truncateInc;
   }
   else if (mainSeg->y == cmpSeg->extraMdHead->y)
   {
      mainSeg_offset = md_cmpSeg_offset + truncateInc;
      mainSeg_size = md_cmpSeg_size - truncateInc;
   }
   else if (mainSeg->y == cmpSeg->extraLgHead->y)
   {
      mainSeg_offset = lg_cmpSeg_offset + truncateInc;
      mainSeg_size = lg_cmpSeg_size - truncateInc;
   }
   
   // add an extra tail to the compare segments
   sm_cmpSeg_offset -= cmpSeg->extraMdHead->tailSegSz;
   sm_cmpSeg_size += cmpSeg->extraMdHead->tailSegSz;
   
   md_cmpSeg_offset -= cmpSeg->extraMdHead->tailSegSz;
   md_cmpSeg_size += cmpSeg->extraMdHead->tailSegSz;
   
   lg_cmpSeg_offset -= cmpSeg->extraMdHead->tailSegSz;
   lg_cmpSeg_size += cmpSeg->extraMdHead->tailSegSz;
   
   
   //printLinePattVis6(mainSeg->y, mainSeg->x, mainSeg->offset + mainSeg_offset, mainSeg_size);
   //printf("^main seg\n");
/*
   printLinePattVis6(cmpSeg->extraSmHead->y, cmpSeg->extraSmHead->x,
                     cmpSeg->extraSmHead->offset + sm_cmpSeg_offset, sm_cmpSeg_size);
   printf("^small seg\n");
   printLinePattVis6(cmpSeg->extraMdHead->y, cmpSeg->extraMdHead->x,
                     cmpSeg->extraMdHead->offset + md_cmpSeg_offset, md_cmpSeg_size);
   printf("^medium seg\n");
   
   printLinePattVis6(cmpSeg->extraLgHead->y, cmpSeg->extraLgHead->x,
                     cmpSeg->extraLgHead->offset + lg_cmpSeg_offset, lg_cmpSeg_size);
   printf("^large seg\n");
   */
   
   fitsSmSeg = linesMatch(mainSeg->x, mainSeg->y, mainSeg->offset + mainSeg_offset, mainSeg_size,
                          cmpSeg->extraSmHead->x, cmpSeg->extraSmHead->y,
                          cmpSeg->extraSmHead->offset + sm_cmpSeg_offset, sm_cmpSeg_size);
   
   fitsMdSeg = linesMatch(mainSeg->x, mainSeg->y, mainSeg->offset + mainSeg_offset, mainSeg_size,
                          cmpSeg->extraMdHead->x, cmpSeg->extraMdHead->y,
                          cmpSeg->extraMdHead->offset + md_cmpSeg_offset, md_cmpSeg_size);
   
   fitsLgSeg = linesMatch(mainSeg->x, mainSeg->y, mainSeg->offset + mainSeg_offset, mainSeg_size,
                          cmpSeg->extraLgHead->x, cmpSeg->extraLgHead->y,
                          cmpSeg->extraLgHead->offset + lg_cmpSeg_offset, lg_cmpSeg_size);
   
   int fitsSmSeg_next;
   fitsSmSeg_next = linesMatch(mainSeg->x, mainSeg->y, mainSeg->offset + mainSeg_offset + 1, mainSeg_size - 1,
                               cmpSeg->extraSmHead->x, cmpSeg->extraSmHead->y,
                               cmpSeg->extraSmHead->offset + sm_cmpSeg_offset, sm_cmpSeg_size);
   
   if (fitsSmSeg_next != fitsSmSeg)
   ht_determinant = mainSeg_size;
   /*
   if (fitsSmSeg)
   printf("Small seg matches!!!\n");
   
   if (fitsMdSeg)
   printf("Medium seg matches!!!\n");
   
   if (fitsLgSeg)
   printf("Large seg matches!!!\n");
   */
   if (((fitsSmSeg) && (fitsMdSeg) && (fitsLgSeg)) ||
       ((fitsSmSeg) && (fitsMdSeg) && (!fitsLgSeg)) ||
       ((fitsSmSeg) && (!fitsMdSeg) && (!fitsLgSeg)))
   return SM_INI_SEG;

   if (((!fitsSmSeg) && (fitsMdSeg) && (fitsLgSeg)) ||
      ((!fitsSmSeg) && (fitsMdSeg) && (!fitsLgSeg)))
   return EQ_INI_SEG;

   if (((!fitsSmSeg) && (!fitsMdSeg) && (fitsLgSeg)))
   return LG_INI_SEG;

   return 0;
}

int findIniSegType_discreteLogic(struct line *mainSeg, struct line *cmpSeg, int truncateInc)
{
   //int nextSegTailCnt;
   int headSz, tailSegSz;
   int tailSegCnt;//next whole segment's prior tail segment count
   //int headSzPrior, tailSegSzPrior;//, tailSegCntPrior;
   int lineSz;
   
   headSz = cmpSeg->extraMdHead->headSz;
   tailSegSz = cmpSeg->extraMdHead->tailSegSz;
   
   // used to represent the number of tail segments that is in the next larger segment
   tailSegCnt = cmpSeg->extraMdHead->tailSegCnt;
   
   //headSzPrior = cmpSeg->extraMdHead->headSzPrior;
   //tailSegSzPrior = cmpSeg->extraMdHead->tailSegSzPrior;
   //tailSegCntPrior = cmpSeg->extraMdHead->tailSegCntPrior;
   
   // find the line size
   if (mainSeg->y == cmpSeg->extraSmHead->y)
   lineSz = headSz + tailSegSz * (tailSegCnt-1) + headSz - truncateInc;
   else if (mainSeg->y == cmpSeg->extraMdHead->y)
   lineSz = headSz + tailSegSz * tailSegCnt + headSz - truncateInc;
   else if (mainSeg->y == cmpSeg->extraLgHead->y)
   lineSz = headSz + tailSegSz * (tailSegCnt+1) + headSz - truncateInc;
   
   //printf("headSz:%i\n", headSz);
   //printf("lineSz:%i\n", lineSz);
   
   // method to find the initial segment type
   //if (0)
   /*if (headSz < tailSegSz)
   //if (mainSeg->offsetFlipFlop)
   {
   printf("headSz < tailSegSz\n");
      return 0;
      if (lineSz >= headSz + tailSegSz * (tailSegCnt+1))
      return LG_INI_SEG;
      
      if (lineSz >= headSz + tailSegSz * tailSegCnt)
      return EQ_INI_SEG;
      
      return SM_INI_SEG;
   }
   
   if (headSzPrior > tailSegSzPrior)
   return 0;*/
   int y = cmpSeg->extraMdHead->y;
   int offset = cmpSeg->extraMdHead->offset;
   int flipflop = cmpSeg->extraMdHead->offsetFlipFlop;
   calc_ht_determinant = y + offset - (tailSegSz * flipflop);
   printf("calc_ht_determinant: %i\n", calc_ht_determinant);
   printf("small ht_determinant: %i\n", ht_determinant);
   printf("medium ht_determinant: %i\n", ht_determinant + tailSegSz);
   
   if (lineSz >= ht_determinant + tailSegSz)
   return LG_INI_SEG;
   
   if (lineSz >= ht_determinant)
   return EQ_INI_SEG;
   
   /*
   if (lineSz >= headSz + tailSegSz * tailSegCnt + headSz)
   return LG_INI_SEG;
   
   if (lineSz >= headSz + tailSegSz * (tailSegCnt-1) + headSz)
   return EQ_INI_SEG;
   */
   return SM_INI_SEG;
}

void matchLines(struct line *mainSeg, struct line *cmpSeg)
{
   int i;
   int truncateCnt;
   int iniSegType_BF;
   int iniSegType_DL;
   
   // find the the amount to truncate the line
   if (mainSeg->y == cmpSeg->extraSmHead->y)
   truncateCnt = cmpSeg->extraMdHead->headSz + cmpSeg->extraMdHead->tailSegSz * (cmpSeg->extraMdHead->tailSegCnt-1);
   else if (mainSeg->y == cmpSeg->extraMdHead->y)
   truncateCnt = cmpSeg->extraMdHead->headSz + cmpSeg->extraMdHead->tailSegSz * cmpSeg->extraMdHead->tailSegCnt;
   else if (mainSeg->y == cmpSeg->extraLgHead->y)
   truncateCnt = cmpSeg->extraMdHead->headSz + cmpSeg->extraMdHead->tailSegSz * (cmpSeg->extraMdHead->tailSegCnt+1);
   
   printf("truncateCnt:%i\n", truncateCnt);
   
   // get the ht_determinant first
   for (i=0; i < truncateCnt; i++)
   iniSegType_BF = findIniSegType_bruteForce(mainSeg, cmpSeg, i);
   
   for (i=0; i < truncateCnt; i++)
   {
      iniSegType_BF = findIniSegType_bruteForce(mainSeg, cmpSeg, i);
      
      printf("%i ", i);
      if (iniSegType_BF == 1)
      printf("bruteForce:    Small initial segment\n");
      else if (iniSegType_BF == 2)
      printf("bruteForce:    Equal initial segment\n");
      else if (iniSegType_BF == 3)
      printf("bruteForce:    Large initial segment\n");
      
      iniSegType_DL = findIniSegType_discreteLogic(mainSeg, cmpSeg, i);
      
      printf("%i ", i);
      if (iniSegType_DL == 1)
      printf("discreteLogic: Small initial segment\n");
      else if (iniSegType_DL == 2)
      printf("discreteLogic: Equal initial segment\n");
      else if (iniSegType_DL == 3)
      printf("discreteLogic: Large initial segment\n");
      
      if (iniSegType_BF != iniSegType_DL)
      {
         printf("ERROR: Brute force segment type does not match discrete logic segment type.\n");
         break;
      }
      else
      printf("OK\n");
   }
}

void findMatchIniSegTypes(struct line *l)
{
   static struct line mainSeg;
   
   if (l->y == 1)
   return;
   
   // the medium segment is copied to the main segment
   memcpy(&mainSeg, l->extraLgHead, sizeof(struct line));
   
   matchLines(&mainSeg, l);
   
}

void printVisual(struct line *l)
{
   //printLinePattVis6(l->extraSmHead->y, l->extraSmHead->x, l->extraSmHead->offset, l->extraSmHead->length);
   //printLinePattVis6(l->extraMdHead->y, l->extraMdHead->x, l->extraMdHead->offset, l->extraMdHead->length);
   //printLinePattVis6(l->extraLgHead->y, l->extraLgHead->x, l->extraLgHead->offset, l->extraLgHead->length);
   /*
   printf("->extraLgHead\n");
   printf("headSz:%i tailSegSz:%i tailSegCnt:%i\n", l->extraLgHead->headSz, l->extraLgHead->tailSegSz, l->extraLgHead->tailSegCnt);
   printf("x:%i y:%i offset:%i\n", l->extraLgHead->x, l->extraLgHead->y, l->extraLgHead->offset);
   printf("headSzPrior:%i tailSegSzPrior:%i tailSegCntPrior:%i\n", l->extraLgHead->headSzPrior, l->extraLgHead->tailSegSzPrior, l->extraLgHead->tailSegCntPrior);
   printf("xPrior:%i xPriorPrior:%i\n", l->extraLgHead->xPrior, l->extraLgHead->xPriorPrior);
   */
   printLinePattVis6(l->y, l->x, l->offset, l->length);
   printf("headSz:%i tailSegSz:%i tailSegCnt:%i\n", l->headSz, l->tailSegSz, l->tailSegCnt);
   printf("x:%i y:%i offset:%i\n", l->x, l->y, l->offset);
   printf("headSzPrior:%i tailSegSzPrior:%i tailSegCntPrior:%i\n", l->headSzPrior, l->tailSegSzPrior, l->tailSegCntPrior);
   printf("xPrior:%i xPriorPrior:%i\n", l->xPrior, l->xPriorPrior);
}

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

   while (TRUE)
   {
      input = getchar();
      
      if (input == '\n')
      continue; // skip the newline character '\n'
      
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
      
      findMatchIniSegTypes(&l);
   }
   
   return 0;
}
