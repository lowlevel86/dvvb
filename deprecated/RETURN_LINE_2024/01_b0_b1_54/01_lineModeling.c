#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "return_line.h"
#include "build_line.h"
#include "print_line.h"

#define TRUE 1
#define FALSE 0

#define BUILD_PATT_0 0b0010000
#define BUILD_PATT_1 0b0010001
#define BUILD_PATT_00 0b0100000
#define BUILD_PATT_01 0b0100001
#define BUILD_PATT_10 0b0100010
#define BUILD_PATT_11 0b0100011
#define BUILD_PATT_000 0b0110000
#define BUILD_PATT_001 0b0110001
#define BUILD_PATT_010 0b0110010
#define BUILD_PATT_011 0b0110011
#define BUILD_PATT_100 0b0110100
#define BUILD_PATT_101 0b0110101
#define BUILD_PATT_110 0b0110110
#define BUILD_PATT_111 0b0110111
#define BUILD_PATT_0000 0b1000000
#define BUILD_PATT_0001 0b1000001
#define BUILD_PATT_0010 0b1000010
#define BUILD_PATT_0011 0b1000011
#define BUILD_PATT_0100 0b1000100
#define BUILD_PATT_0101 0b1000101
#define BUILD_PATT_0110 0b1000110
#define BUILD_PATT_0111 0b1000111
#define BUILD_PATT_1000 0b1001000
#define BUILD_PATT_1001 0b1001001
#define BUILD_PATT_1010 0b1001010
#define BUILD_PATT_1011 0b1001011
#define BUILD_PATT_1100 0b1001100
#define BUILD_PATT_1101 0b1001101
#define BUILD_PATT_1110 0b1001110
#define BUILD_PATT_1111 0b1001111

static int getLinePattBit(int x, int y, int loc)
{
   if ((loc * x / y) != ((loc + 1) * x / y))
   return 1;

   return 0;
}

/*static int linesMatch(int xA, int yA, int offsetA,
                      int xB, int yB, int offsetB, int size)
{
   int i;

   for (i=0; i < size; i++)
   if (getLinePattBit(xA, yA, offsetA+i) != getLinePattBit(xB, yB, offsetB+i))
   return FALSE;

   return TRUE;
}*/

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

static void addOpsToTracker(char c, struct operation_tracker *tracker, int loopCnt) // NOTE: only needed for testing
{
   int i, opCnt;
   
   for (i=0; i < loopCnt; i++)
   {
      opCnt = strlen(tracker->operations);
      
      if (opCnt+1 >= sizeof(tracker->operations))
      {
         printf("The operation tracker is full.\n");
         exit(0);
      }
      
      tracker->operations[opCnt] = c;
   }
}

static void copyTrackerOps(struct operation_tracker *a_tracker, struct operation_tracker *b_tracker) // NOTE: only needed for testing
{
   int i;
   int a_opCnt = strlen(a_tracker->operations);
   int b_opCnt = strlen(b_tracker->operations);
   
   for (i=0; i < a_opCnt; i++)
   b_tracker->operations[i] = a_tracker->operations[i];
   
   for (; i < b_opCnt; i++)
   b_tracker->operations[i] = 0;
}

/*
static void addOffset(int count, struct line *l, struct operation_tracker *tracker)
{
   int i;
   
   truncate(l, count);
   
   for (i=0; i < count; i++)
   addOpToTracker('t', tracker);
}

static void increment_tail_segment(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('1', tracker);
   
   incPriorTailSegCnt(l);
   addOpToTracker('p', tracker);
}

static void increment_tail_segment_truncate(struct modeling *m, struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('2', tracker);
   
   incPriorTailSegCnt(l);
   addOpToTracker('p', tracker);
   
   addOffset(m->priorTailSegSz, l, tracker);//'t...'
}

static void small_first_segment_found_unknown_head(struct modeling *m, struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('3', tracker);
   
   decPriorTailSegCnt(l);
   addOpToTracker('P', tracker);
   
   addOffset(m->priorTailSegSz, l, tracker);//'t...'
}
*/
/*
static void new_medium_top_segment(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('4', tracker);
   
   newSegMd(l);
   addOpToTracker('m', tracker);
}

static void large_second_head_segment_found(struct line *l, struct operation_tracker *tracker)
{
   addOpToTracker('5', tracker);
   
   undoSegMd(l);
   addOpToTracker('u', tracker);
   
   newSegLg(l);
   addOpToTracker('l', tracker);
}*/

//case BUILD_PATT_10repeat:
//case BUILD_PATT_0repeat:
//case BUILD_PATT_0repeat1:
//case BUILD_PATT_01repeat:
//case BUILD_PATT_1repeat:
//case BUILD_PATT_1repeat0:
//case BUILD_PATT_10repeat1:
//case BUILD_PATT_01repeat0:

//case BUILD_PATT_1000repeat:
//case BUILD_PATT_101010repeat:
//case BUILD_PATT_010101repeat:
/*

    |_|---6
    |_|---5
    |_|---4
   _|_|---3
  |_|---2
  |_|---1
  |_|---0
  |_|
             _
            |_|---6
           _|_|---5
         _|_|---4
        |_|---3
       _|_|---2
     _|_|---1
   _|_|---0
  |_|
         _
       _|_|---5
      |_|---4
     _|_|---3
    |_|---2
    |_|---1
   _|_|---0
  |_|
         _
        |_|---5
       _|_|---4
      |_|---3
     _|_|---2
   _|_|---1
  |_|---0
  |_|
  
*/
int nextUpperCriticalLoc(int build_patt)
{
   switch (build_patt)
   {
      case BUILD_PATT_0:
         return 1;
      case BUILD_PATT_1:
         return 1;
      case BUILD_PATT_00:
         return 2;
      case BUILD_PATT_01:
         return 2;
      case BUILD_PATT_10:
         return 2;
      case BUILD_PATT_11:
         return 2;
      case BUILD_PATT_000:
         return 3;
      case BUILD_PATT_001:
         return 3;
      case BUILD_PATT_010:
         return 3;
      case BUILD_PATT_011:
         return 4;
      case BUILD_PATT_100:
         return 4;
      case BUILD_PATT_101:
         return 3;
      case BUILD_PATT_110:
         return 3;
      case BUILD_PATT_111:
         return 3;
      case BUILD_PATT_0000:
         return 4;
      case BUILD_PATT_0001:
         return 4;
      case BUILD_PATT_0010:
         return 4;
      case BUILD_PATT_0011:
         break;//no such thing
      case BUILD_PATT_0100:
         printf("empty at line:%i\n", __LINE__);//return 4;
         exit(0);
         break;
      case BUILD_PATT_0101:
         return 4;
      case BUILD_PATT_0110:
         return 5;
      case BUILD_PATT_0111:
         return 6;
      case BUILD_PATT_1000:
         return 6;
      case BUILD_PATT_1001:
         return 5;
      case BUILD_PATT_1010:
         return 4;
      case BUILD_PATT_1011:
         printf("empty at line:%i\n", __LINE__);//return 4;
         exit(0);
         break;
      case BUILD_PATT_1100:
         break;//no such thing
      case BUILD_PATT_1101:
         return 4;
      case BUILD_PATT_1110:
         return 4;
      case BUILD_PATT_1111:
         return 4;
   }
   
   return -1;
}

int findNextUpperCriticalLoc(struct line *bX, int pattLoc)
{
   /*|_|---4 = 5
     |_|
    _|_|---2 = 4
   |_|---1 = 2
   |_|---0 = 1
   |_|*/
   /*_
    |_|---1 = 2
   _|_|---0 = 1
  |_|*/

   // build_pattern_count + critical_loc_top
   
   int base_cnt;
   int critical_loc_top;
   int critical_loc_base = 0;
   int pxLoc = pattLoc + 1;
   
   printf("///////// %s /////////\n", __FUNCTION__);
   //printf("tailSegCntNext:%i\n", bX->tailSegCntNext);
   
   if (bX->offsetFlipFlop)
   critical_loc_top = (bX->y - bX->offset);
   else
   critical_loc_top = (bX->y - bX->offset - bX->tailSegSz);
   
   printf("critical_loc_top:%i\n", critical_loc_top);
   
   printf("bX->headSz:%i\n", bX->headSz);
   
   if (bX->headSz == 0)
   {
      base_cnt = pxLoc / (bX->tailSegSz * bX->tailSegCnt);
      
      if (base_cnt)
      critical_loc_base = (bX->tailSegSz * bX->tailSegCnt) * base_cnt - 1;
   }
   else
   {
      critical_loc_base = 0;
   }
   
   printf("critical_loc_base:%i\n", critical_loc_base);
   
   printf("pattLoc:%i critical_loc:%i\n", pattLoc, critical_loc_base + critical_loc_top);
   
   printf("///////// %s /////////\n", __FUNCTION__);
   return critical_loc_base + critical_loc_top;
   
   /*if (pattLoc == 0)
   return 0+1;
   if (pattLoc == 1)
   return 0+2;
   if (pattLoc == 2)
   return 3-1+2;//tailSz - 1 + critical_loc_top
   if (pattLoc == 3)
   {printf(">>>>>>>findNextUpperCriticalLoc Error\n");exit(0);}
   if (pattLoc == 4)
   return 5;
   
   return 0;*/
}

int pattLoc_global;
void build_b0(struct line *b0, int build_patt, struct operation_tracker *b0_tracker)
{
   switch (build_patt)
   {
      case BUILD_PATT_0:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         break;
      case BUILD_PATT_1:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_00:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         break;
      case BUILD_PATT_01:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_10:
         break;
      case BUILD_PATT_11:
         undoSegMd(b0);
         addOpToTracker('u', b0_tracker);
         newSegLg(b0);
         addOpToTracker('l', b0_tracker);
         break;
      case BUILD_PATT_000:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         break;
      case BUILD_PATT_001:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_010:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_011:
         incPriorTailSegCnt(b0);
         addOpToTracker('p', b0_tracker);
         convertLgToSm(b0);
         addOpToTracker('S', b0_tracker);
         truncate(b0, b0->tailSegSz+b0->tailSegSzPrior);
         addOpsToTracker('t', b0_tracker, b0->tailSegSz+b0->tailSegSzPrior);
         break;
      case BUILD_PATT_100:
         break;
      case BUILD_PATT_101:
         break;
      case BUILD_PATT_110:
         break;
      case BUILD_PATT_111:
         undoSegMd(b0);
         addOpToTracker('u', b0_tracker);
         newSegLg(b0);
         addOpToTracker('l', b0_tracker);
         break;
      case BUILD_PATT_0000:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         break;
      case BUILD_PATT_0001:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_0010:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, 1);
         addOpToTracker('t', b0_tracker);
         break;
      case BUILD_PATT_0011:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_0100:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_0101:
         undoSegMd(b0);
         addOpToTracker('u', b0_tracker);
         newSegLg(b0);
         addOpToTracker('l', b0_tracker);
         break;
      case BUILD_PATT_0110:
         incTailSegCnt(b0);
         addOpToTracker('i', b0_tracker);
         truncate(b0, b0->tailSegSz);
         addOpsToTracker('t', b0_tracker, b0->tailSegSz);
         break;
      case BUILD_PATT_0111:
         decTailSegCnt(b0);
         addOpToTracker('d', b0_tracker);
         newSegSm(b0);
         addOpToTracker('s', b0_tracker);
         truncate(b0, b0->tailSegSz+b0->headSz-b0->tailSegSzPrior);
         addOpsToTracker('t', b0_tracker, b0->tailSegSz+b0->headSz-b0->tailSegSzPrior);
         break;
      case BUILD_PATT_1000:
         break;
      case BUILD_PATT_1001:
         break;
      case BUILD_PATT_1010:
         break;
      case BUILD_PATT_1011:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_1100:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_1101:
         break;
      case BUILD_PATT_1110:
         decTailSegCnt(b0);
         addOpToTracker('d', b0_tracker);
         decTailSegCnt(b0);
         addOpToTracker('d', b0_tracker);
         break;
      case BUILD_PATT_1111:
         undoSegMd(b0);
         addOpToTracker('u', b0_tracker);
         newSegLg(b0);
         addOpToTracker('l', b0_tracker);
         break;
   }
   
   return;
}

void build_b1(struct line *b1, int build_patt, struct operation_tracker *b1_tracker)
{
   switch (build_patt)
   {
      case BUILD_PATT_0:
         //printf("//////////////////////////////%s\n", __FUNCTION__);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_00:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_01:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_10:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         /*decTailSegCnt(b1);
         addOpToTracker('d', b1_tracker);
         newSegLg(b1);
         addOpToTracker('l', b1_tracker);
         truncate(b1, 1);
         addOpToTracker('t', b1_tracker);*/
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_11:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_000:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_001:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_010:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_011:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_100:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_101:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_110:
         convertSmToLg(b1);
         addOpToTracker('L', b1_tracker);
         decPriorTailSegCnt(b1);
         addOpToTracker('P', b1_tracker);
         incTailSegCnt(b1);
         addOpToTracker('i', b1_tracker);
         incTailSegCnt(b1);
         addOpToTracker('i', b1_tracker);
         truncate(b1, b1->tailSegCnt);//*tailSegSz
         addOpsToTracker('t', b1_tracker, b1->tailSegCnt);
         break;
      case BUILD_PATT_111:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_0000:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_0001:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_0010:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_0011:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_0100:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_0101:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_0110:
         break;
      case BUILD_PATT_0111:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1000:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1001:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1010:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         newSegSm(b1);
         addOpToTracker('s', b1_tracker);
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1011:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_1100:
         printf("empty at line:%i\n", __LINE__);
         exit(0);
         break;
      case BUILD_PATT_1101:
         undoSegMd(b1);
         addOpToTracker('u', b1_tracker);
         convertSmToLg(b1);
         addOpToTracker('L', b1_tracker);
         decPriorTailSegCnt(b1);
         addOpToTracker('P', b1_tracker);
         break;
      case BUILD_PATT_1110:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
      case BUILD_PATT_1111:
         newSegMd(b1);
         addOpToTracker('m', b1_tracker);
         break;
   }
   
   return;
}

int lineModeling(struct line *b0, struct line *b1, struct line **l, int pattBitVal, int pattLoc, struct operation_tracker *b0_tracker, struct operation_tracker *b1_tracker)
{
   static int upper_critical_loc;
   static int build_patt;
   static int b0Matches;
   //int ucl_b0, ucl_b1;
   
   if (pattLoc == 0)// reset variables
   {
      upper_critical_loc = 0;
      build_patt = 0;
   }
   pattLoc_global = pattLoc;// TEMP for testing
   printf("-------given pattern bit:%i, loc:%i\n", pattBitVal, pattLoc);
   
   
   printf("upper_critical_loc:%i\n", upper_critical_loc);
   printf("critical bit values b0:%i b1:%i\n", getLinePattBit(b0->x, b0->y, upper_critical_loc + b0->offset), getLinePattBit(b1->x, b1->y, upper_critical_loc + b1->offset));
   
   if ((getLinePattBit(b0->x, b0->y, upper_critical_loc + b0->offset) != 0) || //TEMP
       (getLinePattBit(b1->x, b1->y, upper_critical_loc + b1->offset) != 1))
   {
      printf("INCORRECT UPPER CRITICAL LOCATION BIT VALUES\n");
      return 1;
   }
   
   if (pattLoc > 4)
   {
      printf("Error, PATTERN TOO BIG\n");
      return 1;
   }
   
   
   // match pattern bit to model bit
   if (pattBitVal == getLinePattBit(b0->x, b0->y, b0->offset + pattLoc))
   {
      b0Matches = TRUE;
      *l = b0;
      build_patt += 1 << 4;
      build_patt += pattBitVal << pattLoc;
   }
   else
   {
      b0Matches = FALSE;
      *l = b1;
      build_patt += 1 << 4;
      build_patt += pattBitVal << pattLoc;
   }
   
   int i;
   printf("build_patt:%i, 0b", pattLoc+1);
   for (i=6; i >= 0; i--)
   printf("%i", (build_patt & (int)pow(2, i)) ? 1 : 0);
   printf("\n");
   
   
   // rebuild the non-matching line
   if (upper_critical_loc == pattLoc)
   {//printf("%s %s() line:%i\n", __FILE__, __FUNCTION__, __LINE__);
      if (b0Matches)
      {
         //upper_critical_loc = findNextUpperCriticalLoc(b0, pattLoc);
         upper_critical_loc = nextUpperCriticalLoc(build_patt);
         
         // copy b0 to b1
         memcpy(b1, b0, sizeof(struct line));// b1 <--copy-- b0
         copyTrackerOps(b0_tracker, b1_tracker);
         
         build_b1(b1, build_patt, b1_tracker);
         build_b0(b0, build_patt, b0_tracker);
         
         printf("%s\n", b0_tracker->operations);
         printf("%s\n", b1_tracker->operations);
         
         /*ucl_b0 = findNextUpperCriticalLoc(b0, pattLoc);
         ucl_b1 = findNextUpperCriticalLoc(b1, pattLoc);
         if (ucl_b0 < ucl_b1)
         upper_critical_loc = ucl_b0;
         else
         upper_critical_loc = ucl_b1;*/
      }
      else
      {
         //upper_critical_loc = findNextUpperCriticalLoc(b1, pattLoc);
         upper_critical_loc = nextUpperCriticalLoc(build_patt);
         
         // copy b1 to b0
         memcpy(b0, b1, sizeof(struct line));// b0 <--copy-- b1
         copyTrackerOps(b1_tracker, b0_tracker);
         printLinePattVis6(b0->y, b0->x, b0->offset, pattLoc+2);
         build_b0(b0, build_patt, b0_tracker);
         build_b1(b1, build_patt, b1_tracker);
         
         printf("%s\n", b0_tracker->operations);
         printf("%s\n", b1_tracker->operations);
         
         /*ucl_b0 = findNextUpperCriticalLoc(b0, pattLoc);
         ucl_b1 = findNextUpperCriticalLoc(b1, pattLoc);
         if (ucl_b0 < ucl_b1)
         upper_critical_loc = ucl_b0;
         else
         upper_critical_loc = ucl_b1;*/
      }
   }
   
   return 0;
}
