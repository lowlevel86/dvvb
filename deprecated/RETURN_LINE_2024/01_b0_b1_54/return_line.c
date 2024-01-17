#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "print_line.h"
#include "return_line.h"
#include "build_line.h"
#include "01_lineModeling.h"


static void iniOpTracker(struct operation_tracker *tracker) // NOTE: only needed for testing
{
   int i;

   for (i=0; i < sizeof(tracker->operations); i++)
   tracker->operations[i] = 0;
}

static void addOpToTracker(char c, struct operation_tracker *tracker) // NOTE: only needed for testing
{
   int opCnt = strlen(tracker->operations);
   
   if (opCnt+1 >= sizeof(tracker->operations))
   {
      printf("The operation tracker is full.\n");
      exit(0);
   }
   
   tracker->operations[opCnt] = c;
}

/*
static void addOffset(int count, struct line *l, struct operation_tracker *tracker)
{
   int i;
   
   truncate(l, count);
   
   for (i=0; i < count; i++)
   addOpToTracker('t', tracker);
}


increment_tail_segment 0
new_medium_top_segment 1

increment_tail_segment_truncate 2

//small_first_segment_found_unknown_head 3
//large_second_segment_found_unknown_head 4

//small_second_segment_found_unknown_head 5
//small_second_head_segment_found 6

//convert_to_small_head 7
//convert_to_small_head_add_large_head 8
*/
/*
static void increment_tail_segment(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('0', tracker);
   
   incTailSegCnt(l);
   addOpToTracker('i', tracker);
}

static void new_medium_top_segment(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('1', tracker);
   
   newSegMd(l);
   addOpToTracker('m', tracker);
}

static void increment_tail_segment_truncate(struct modeling *m, struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('2', tracker);
   
   incTailSegCnt(l);
   addOpToTracker('i', tracker);
   
   addOffset(m->tailSegSz, l, tracker);//'t...'
}

static void small_first_segment_found_unknown_head(struct modeling *m, struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('3', tracker);
   
   decTailSegCnt(l);
   addOpToTracker('d', tracker);
   
   newSegLg(l);
   addOpToTracker('l', tracker);
   
   addOffset(m->priorTailSegSz, l, tracker);//'t...'
}*/

//tail_found_add_large_head

/*static void small_first_segment_found_add_large_head(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('4', tracker);
   
   newSegLg(l);
   addOpToTracker('l', tracker);
}*/
/*
int iniLineModeling(struct modeling *m, int pattBitVal, int pattLoc, struct line *l, struct operation_tracker *tracker)
{
   int currSegSz;
   
   if (m->firstSegSz)
   return 1;
   
   if (!m->iniSegSz) // if iniSegSz is not found
   {
      if (pattBitVal)
      m->iniSegSz = pattLoc;
      else
      increment_tail_segment(l, tracker);//1
   }
   else
   {
      if (pattBitVal)
      {
         m->firstSegSz = pattLoc - m->iniSegSz;
         m->firstSegCnt++;
         m->mdSegPtr = m->iniSegSz;
         
         if (m->firstSegSz+1 == m->iniSegSz)
         small_first_segment_found_unknown_head(m, l, tracker);//3
         
         if (m->firstSegSz == m->iniSegSz)
         new_medium_top_segment(l, tracker);//0
         
         //if (m->firstSegSz > m->iniSegSz)
         //printf("m->firstSegSz > m->iniSegSz\n");
      }
      else
      {
         currSegSz = pattLoc+1 - m->iniSegSz;
         
         if (currSegSz > m->iniSegSz)
         increment_tail_segment_truncate(m, l, tracker);//2
      }
   }
   
   return 0;
}*/

int returnLine(char *patt, int pattSz, int *rise, int *run, int *offset, int *length)
{
   int i;
   static struct line b0, b1;
   struct line *l;
   struct operation_tracker b0_tracker, b1_tracker; // TEMP
   
   // print binary pattern defines
   /*int j, k, build_patt;
   for (i=1; i <= 4; i++)// place values
   {
      for (j=0; j < (int)pow(2, i); j++) // place value count for each power
      {
         printf("#define BUILD_PATT_");
         for (k=i-1; k >= 0; k--)// bin count
         printf("%i", (j & (int)pow(2, k)) ? 1 : 0);
         
         build_patt = i << 4;
         build_patt += j;
         
         printf(" 0b");
         for (k=6; k >= 0; k--)
         printf("%i", (build_patt & (int)pow(2, k)) ? 1 : 0);
         printf("\n");
      }
   }
   
   // print binary pattern cases
   for (i=1; i <= 4; i++)// place values
   {
      for (j=0; j < (int)pow(2, i); j++) // place value count for each power
      {
         printf("case BUILD_PATT_");
         for (k=i-1; k >= 0; k--)// bin count
         printf("%i", (j & (int)pow(2, k)) ? 1 : 0);
         
         printf(":\n");
         printf("   printf(\"empty at line:%%i\\n\", __LINE__);\n");
         printf("   exit(0);\n");
         printf("   break;\n");
      }
   }
   exit(0);*/
   
   
   iniBuildLine(&b0);
   iniBuildLine(&b1);
   
   iniOpTracker(&b0_tracker);
   iniOpTracker(&b1_tracker);

   incTailSegCnt(&b0);
   addOpToTracker('i', &b0_tracker);
   
   newSegMd(&b1);
   addOpToTracker('m', &b1_tracker);
   
   // line modeling
   for (i=0; i < pattSz; i++)
   if (lineModeling(&b0, &b1, &l, patt[i], i, &b0_tracker, &b1_tracker))
   {
      printf("b0: headSz:%i tailSegSz:%i tailSegCnt:%i ", b0.headSz, b0.tailSegSz, b0.tailSegCnt);
      printf("tailSegCntNext:%i ", b0.tailSegCntNext);
      printf("x:%i y:%i offset:%i\n", b0.x, b0.y, b0.offset);
      printf("b0: headSzPrior:%i tailSegSzPrior:%i tailSegCntPrior:%i ", b0.headSzPrior, b0.tailSegSzPrior, b0.tailSegCntPrior);
      printf("xPrior:%i xPriorPrior:%i\n", b0.xPrior, b0.xPriorPrior);
      
      printf("b1: headSz:%i tailSegSz:%i tailSegCnt:%i ", b1.headSz, b1.tailSegSz, b1.tailSegCnt);
      printf("tailSegCntNext:%i ", b1.tailSegCntNext);
      printf("x:%i y:%i offset:%i\n", b1.x, b1.y, b1.offset);
      printf("b1: headSzPrior:%i tailSegSzPrior:%i tailSegCntPrior:%i ", b1.headSzPrior, b1.tailSegSzPrior, b1.tailSegCntPrior);
      printf("xPrior:%i xPriorPrior:%i\n", b1.xPrior, b1.xPriorPrior);
      return 1;
   }
   
   *rise = l->y;
   *run = l->x;
   *offset = l->offset;
   *length = pattSz + 1;
   printLinePattVis6(*rise, *run, *offset, *length);
   printf("b0: %s\n", b0_tracker.operations);
   printf("b1: %s\n", b1_tracker.operations);
   
   return 0;
}
