#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "line_patt_generator.h" // uses LINE_PATT_BUFF_SZ
#include "print_line.h"
#include "return_line.h"
//#include "build_line.h"
#include "modeling_phase.h"

#define TRUE 1
#define FALSE 0

struct line l;

struct modeling_phase phase = {0};

void iniTestVars()
{
   int i;

   for (i=0; i < sizeof(tv.opTracker); i++)
   tv.opTracker[i] = 0;
}

static int getLinePattBit(int x, int y, int loc)
{
   if ((loc * x / y) != ((loc + 1) * x / y))
   return 1;

   return 0;
}

struct pattern_variables *iniPattVars(char *buff)
{
   static struct pattern_variables pv;

   pv.buff = buff;
   pv.buffInc = 0;
   pv.ptrA = 0;// 'A' occurs more often than 'B'?
   pv.ptrB = 0;// 'A' is the tail and 'B' is the head???
   pv.cntA = 0;
   pv.cntB = 0;
   pv.incA = 0;
   pv.incB = 0;
   pv.multInc = 0;
   pv.multPrior = 0;
   pv.multH1 = 0;
   pv.multH2 = 0;
   pv.multA = 0;
   pv.multB = 0;
   pv.multOffset = 0;
   pv.identicalSegInc = 0;
   pv.flipFlopSegInc = 0;
   pv.checkVal = 2; // 2 == any bit value; -1 == none
   
   return &pv;
}

void printVars(struct pattern_variables *pv)
{
   printf("buffInc:%i\n", pv->buffInc);
   printf("ptrA:%i\n", pv->ptrA);
   printf("ptrB:%i\n", pv->ptrB);
   printf("cntA:%i\n", pv->cntA);
   printf("cntB:%i\n", pv->cntB);
   printf("incA:%i\n", pv->incA);
   printf("incB:%i\n", pv->incB);

   printf("multInc:%i\n", pv->multInc);
   printf("multPrior:%i\n", pv->multPrior);
   printf("multH1:%i\n", pv->multH1);
   printf("multH2:%i\n", pv->multH2);
   printf("multA:%i\n", pv->multA);
   printf("multB:%i\n", pv->multB);
   printf("multOffset:%i\n", pv->multOffset);

   printf("identicalSegInc:%i\n", pv->identicalSegInc);
   printf("flipFlopSegInc:%i\n", pv->flipFlopSegInc);
   printf("checkVal:%i\n", pv->checkVal);
}

void recalcBitPattVars(struct pattern_variables *pv)
{
   int bitCntA, bitCntB;
   
   bitCntA = pv->multA * pv->cntA + pv->cntB;
   bitCntB = pv->multB * pv->cntA + pv->cntB;
   pv->ptrA = pv->buffInc+1 - (bitCntA + bitCntB);
   pv->ptrB = pv->buffInc+1 - bitCntB;
   pv->cntA = bitCntA;
   pv->cntB = bitCntB;
}


void accOffset(struct pattern_variables *pv) // account offset
{
   if (pv->flipFlopSegInc == 0)
   {
      pv->multOffset = pv->identicalSegInc + 1;
      pv->multInc = 0;
      pv->multH1 = 0;
      pv->multH2 = 0;
      pv->flipFlopSegInc = 0;
   }
   else if (pv->flipFlopSegInc == 1)
   {
      if (pv->multOffset)
      pv->multOffset = 0;
      else
      pv->multOffset = 1;
      
      pv->multInc = pv->identicalSegInc + 1;
      pv->multH1 = pv->multInc;
      pv->multH2 = 0;
      pv->flipFlopSegInc = 0;
   }
   else if (pv->flipFlopSegInc == 2)
   {
      if (pv->multOffset)
      pv->multOffset = 1;
      else
      pv->multOffset = 0;
      
      pv->multInc = 2;
      pv->multH1 = 1;
      pv->multH2 = 2;
      pv->flipFlopSegInc = 1;
   }
   else if (pv->flipFlopSegInc > 2)
   {
      pv->multA = 1;
      pv->multB = 2;
      recalcBitPattVars(pv);
      pv->multOffset = pv->flipFlopSegInc - 1;
      pv->multInc = 0;
      pv->multH1 = 0;
      pv->multH2 = 0;
      pv->flipFlopSegInc = 0;
   }
   
   pv->multA = 0;
   pv->multB = 0;
   pv->identicalSegInc = 0;
}


void recalcPattVars(struct pattern_variables *pv)
{
   // account offsets that represent current largest possible
   if (pv->multInc == pv->multOffset-1)
   {
      if (pv->multH1 == 0)
      {
         pv->multH1 = pv->multOffset;
         pv->multOffset = 0;
      }
      else if (pv->multH2 == 0)
      {
         pv->multH1 = pv->multOffset;
         pv->multH2 = pv->multInc;
         pv->multA = pv->multOffset;
         pv->identicalSegInc += 1;
      }
   }
   
   // account multiplier holders
   if (pv->multH1 == 0)
   pv->multH1 = pv->multInc;
   else if (pv->multH2 == 0)
   {
      if (pv->multInc != pv->multH1)
      pv->multH2 = pv->multInc;
   }
   
   // account both multA/B
   if (pv->multA == 0)
   {
      if (pv->multInc == pv->multPrior)
      pv->multA = pv->multInc;
   }
   else
   {
      if (pv->multInc != pv->multA)
      {
         pv->multB = pv->multInc;
         recalcBitPattVars(pv);
      }
   }
   
   if (pv->multB)
   accOffset(pv);
   else
   {
      // keep track of identical segments
      if (pv->multInc == pv->multPrior)
      pv->identicalSegInc += 1;
      
      // keep track of the flip-flop pattern size
      if ((pv->multInc == pv->multH2) && (pv->multInc != pv->multPrior))
      pv->flipFlopSegInc += 1;
   }
}


void growPattModel(int bitVal, struct pattern_variables *pv)
{
   // find if the pattern matches 'A' or 'B'
   if (bitVal == pv->buff[pv->ptrA+pv->incA])
   {
      if (pv->incA != -1)
      pv->incA += 1;
   }
   else
   pv->incA = -1; // does not match
   
   if (bitVal == pv->buff[pv->ptrB+pv->incB])
   {
      if (pv->incB != -1)
      pv->incB += 1;
   }
   else
   pv->incB = -1; // does not match
   
   if (pv->incA == pv->cntA) // if the pattern matches 'A'
   {
      pv->multInc += 1;
      pv->incA = 0;
      pv->incB = 0;
   }
   
   if (pv->incB == pv->cntB) // if the pattern matches 'B'
   {
      recalcPattVars(pv);
      pv->multPrior = pv->multInc;
      pv->multInc = 0;
      pv->incA = 0;
      pv->incB = 0;
   }
   
   pv->buff[pv->buffInc] = bitVal;
   pv->buffInc += 1;
}


void resolveDuoBitVal(struct pattern_variables *pv)
{
   if ((pv->incA != -1) && (pv->incB != -1))
   {
      if (pv->buff[pv->ptrA+pv->incA] == pv->buff[pv->ptrB+pv->incB])
      pv->checkVal = pv->buff[pv->ptrA+pv->incA];
      else
      pv->checkVal = 2;
   }
   else if (pv->incA != -1)
   pv->checkVal = pv->buff[pv->ptrA+pv->incA];
   else if (pv->incB != -1)
   pv->checkVal = pv->buff[pv->ptrB+pv->incB];
}

void duoPattCheck(struct pattern_variables *pv, int smMult, int lgMult)
{
   if (pv->multInc == smMult)
   resolveDuoBitVal(pv);
   else if (pv->multInc == lgMult)
   pv->checkVal = pv->buff[pv->ptrB+pv->incB];
   else if (pv->multInc < smMult)
   pv->checkVal = pv->buff[pv->ptrA+pv->incA];
}

void trioPattCheck(struct pattern_variables *pv, int smMult, int medMult, int lgMult)
{
   if (pv->multInc == smMult)
   resolveDuoBitVal(pv);
   else if (pv->multInc == medMult)
   resolveDuoBitVal(pv);
   else if (pv->multInc == lgMult)
   pv->checkVal = pv->buff[pv->ptrB+pv->incB];
   else if (pv->multInc < smMult)
   pv->checkVal = pv->buff[pv->ptrA+pv->incA];
}

void flipFlopPattCheck(struct pattern_variables *pv, int mult1, int mult2)
{
   if (pv->multA == mult1)
   {
      if (pv->multInc == mult2)
      pv->checkVal = pv->buff[pv->ptrB+pv->incB];
      else if (pv->multInc < mult2)
      pv->checkVal = pv->buff[pv->ptrA+pv->incA];
   }
   else if (pv->multA == mult2)
   {
      if (pv->multInc == mult1)
      pv->checkVal = pv->buff[pv->ptrB+pv->incB];
      else if (pv->multInc < mult1)
      pv->checkVal = pv->buff[pv->ptrA+pv->incA];
   }
}

int checkBit(int bitVal, struct pattern_variables *pv)
{
   pv->checkVal = -1;

   if ((pv->incA == -1) && (pv->incB == -1))
   return 1;
   
   if ((pv->flipFlopSegInc >= 2) && (pv->multA)) // end of flip-flop pattern
   {
      flipFlopPattCheck(pv, pv->multH1, pv->multH2);
   }
   else if (pv->multH2) // check for 2 possible patterns
   {
      if (pv->multH1 == pv->multH2-1) // multH1 < multH2
      duoPattCheck(pv, pv->multH1, pv->multH2);
      else if (pv->multH2 == pv->multH1-1) // multH2 < multH1
      duoPattCheck(pv, pv->multH2, pv->multH1);
   }
   else if ((pv->multH1) && (pv->multH2 == 0)) // check for 3 possible patterns
   {
      trioPattCheck(pv, pv->multH1-1, pv->multH1, pv->multH1+1);
   }
   else if (pv->multH1 == 0) // pattern size is unknown
   {
      if (pv->multInc >= pv->multOffset-1)
      resolveDuoBitVal(pv);
      else if (pv->multInc < pv->multOffset-1)
      pv->checkVal = pv->buff[pv->ptrA+pv->incA];
   }
   
   if (bitVal == pv->checkVal)
   return 0;
   
   if (pv->checkVal == 2)
   return 0;
   
   return 1;
}


void iniGrowPattModel(int bitVal, struct pattern_variables *pv)
{
   // find the bit value that occurs more often
   if (pv->cntA == 0)
   {
      if ((pv->buffInc) && (bitVal == pv->buff[pv->buffInc-1]))
      {
         pv->ptrA = pv->buffInc-1;
         pv->cntA = 1;
         pv->flipFlopSegInc = pv->buffInc >> 1;
      }
   }
   else if (pv->cntB == 0) // find the other bit
   {
      if (bitVal != pv->buff[pv->ptrA])
      {
         pv->ptrB = pv->buffInc;
         pv->cntB = 1;
         
         // find offset
         if (pv->flipFlopSegInc == 0) // 00...1
         {
            pv->multOffset = pv->buffInc;
         }
         else if (pv->flipFlopSegInc == 1) // 0100...1
         {
            if (pv->buff[0] == pv->buff[pv->ptrA]) // 0100...1 else 100...1
            pv->multOffset = 1;
            
            pv->multPrior = pv->ptrB - pv->ptrA;
            pv->multH1 = pv->multPrior;
            pv->flipFlopSegInc = 0;
         }
         else if (pv->flipFlopSegInc == 2) // if flip-flop Pattern 0101001
         {
            if (pv->buff[0] == pv->buff[pv->ptrA]) // 0101001 else 101001
            pv->multOffset = 1;
            
            pv->multPrior = 2;
            pv->multH1 = 1;
            pv->multH2 = 2;
            pv->flipFlopSegInc = 1;
         }
         else if (pv->flipFlopSegInc > 2) // if flip-flop Pattern 010101...001
         {
            pv->ptrA -= 2; pv->ptrB -= 2;
            pv->cntA = 2; pv->cntB = 3;
            pv->multOffset = pv->flipFlopSegInc - 1;
            pv->flipFlopSegInc = 0;
         }
      }
   }

   pv->buff[pv->buffInc] = bitVal;
   pv->buffInc += 1;
}

int iniCheckBit(int bitVal, struct pattern_variables *pv)
{
   pv->checkVal = 2;
   
   // find the possible bit values for flip-flop patterns
   if (pv->flipFlopSegInc >= 2)
   {
      if (pv->buff[pv->buffInc-2] == pv->buff[pv->ptrA])
      pv->checkVal = !pv->buff[pv->ptrA];
      
      if (pv->buff[pv->buffInc-3] == pv->buff[pv->ptrA])
      pv->checkVal = -1;
   }
   
   if (bitVal == pv->checkVal)
   return 0;
   
   if (pv->checkVal == 2)
   return 0;
   
   return 1;
}



/*static void decrementTailSegCnt(int count)
{
   int i;
   for (i=0; i < count; i++)
   {
      decTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'd';
   }
}*/

static void addMdSegs(int count)
{
   int i;
   for (i=0; i < count; i++)
   {
      newSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'm';
   }
}

static void addOffset(int count)
{
   int i;
   
   truncate(&l, count);
   
   for (i=0; i < count; i++)
   tv.opTracker[strlen(tv.opTracker)] = 't';
}

static void removeOffset(int count)
{
   int i;
   
   undoTruncate(&l, count);
   
   for (i=0; i < count; i++)
   tv.opTracker[strlen(tv.opTracker)] = 'v';
}

static void buildLine(int bitVal, struct pattern_variables *pv)
{
   int modelingPhase;
   int pattLoc, lineSz;
   
   pattLoc = pv->buffInc + 1;
   lineSz = pattLoc + 1;
   
   printf(" ----- lineSz:%i\n", lineSz);
   
   modelingPhase = getModelingPhase(&phase);
   
   findIniSeg(&phase, pattLoc, bitVal);
   
   accountModelingPhase(&phase, &l, pattLoc, bitVal);//accountLinePattern
   
   #include "lineStructure.c"
   
   if (phase.build_line_op == increment_tail_segment)
   {
      incTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'i';
   }
   
   //NOTE: just use new_medium_top_segment
   if (phase.build_line_op == initial_new_medium_top_segment)
   {
      tv.opTracker[strlen(tv.opTracker)] = '1';
      
      newSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'm';
      
      //addOffset(l.tailSegCnt);//'t...' needed for normalization
   }
   
   if (phase.build_line_op == lengthen_second_segment_size)
   {
      tv.opTracker[strlen(tv.opTracker)] = '2';
      
      incTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'i';
      
      addOffset(phase.priorTailSegSz);//'t...'
      //*2 needed for normalization
   }
   
   // assume the the larger segment is the head if unknown for normalization
   if (phase.build_line_op == small_second_segment_found_unknown_head)
   {
      tv.opTracker[strlen(tv.opTracker)] = '3';
      
      
      // remove the offset
      //removeOffset(l.offset);
      
      // there should only be one tail segment
      undoSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'u';

      decTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'd';
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
      
      // there should only be one tail segment
      //decrementTailSegCnt(1);//'d...'
      
      //addOffset(l.tailSegSz);//'t...'
      removeOffset(l.headSz);//'v...'
      
      
      /*
      int tailSegSz;
      
      if (phase.secondSegSz < phase.thirdSegSz)
      tailSegSz = phase.secondSegSz;
      else
      tailSegSz = phase.thirdSegSz;
      
      undoSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'u';

      decTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'd';
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
      
      addOffset(tailSegSz);//'t...'
      //removeOffset(phase.priorTailSegSz);//'v...'
      */
   }
   
   // assume the the larger segment is the head if unknown for normalization
   if (phase.build_line_op == large_third_segment_found_unknown_head)
   {
      tv.opTracker[strlen(tv.opTracker)] = '4';
      
      // remove the offset
      removeOffset(l.offset);
      
      // there should only be one tail segment
      if (l.tailSegCnt != 1)
      {
         undoSegMd(&l);
         tv.opTracker[strlen(tv.opTracker)] = 'u';
      }
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
      
      // there should only be one tail segment
      /*if (l.tailSegCnt == 2)
      {
               decTailSegCnt(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'd';
         decrementTailSegCnt(l.tailSegCnt - 1);
      }*/
      //decrementTailSegCnt(l.tailSegCnt - 1);
      
      // remove offset to create the initial segment size
      removeOffset(phase.iniSegSz);
      
      
      // add offset to buffer and create the initial segment size
      //addOffset(l.tailSegSz + (l.headSz - phase.iniSegSz));
      
      // add possible offset for the initial segment size
      //addOffset(l.tailSegSz - phase.iniSegSz);
      
      
      /*
      int tailSegSz;
      
      if (phase.secondSegSz < phase.thirdSegSz)
      tailSegSz = phase.secondSegSz;
      else
      tailSegSz = phase.thirdSegSz;
      */
      //newSegLg(&l);
      //tv.opTracker[strlen(tv.opTracker)] = 'l';
      
      //removeOffset(tailSegSz);//'v...'
      
      // normalization NOTE: the might be another way of doing this
      //addOffset(l.tailSegSz * (l.tailSegCnt - 1) + l.headSz);//'t...'
      //decrementTailSegCnt(l.tailSegCnt - 1);
      //addOffset(l.headSz);//'t...'
   }
   
   if (phase.build_line_op == increment_tail_segment_truncate)
   {
      tv.opTracker[strlen(tv.opTracker)] = '5';
      
      incTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'i';
      
      if (l.offset)
      addOffset(tailSegSz);//'t...'
   }
   
   if (phase.build_line_op == new_medium_top_segment)
   {
      tv.opTracker[strlen(tv.opTracker)] = '6';
      
      newSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'm';
   }
   
   if (phase.build_line_op == small_third_segment_found_unknown_head)
   {
      tv.opTracker[strlen(tv.opTracker)] = '7';
      
      //printf("l.offset:%i\n", l.offset);
      //printf("l.tailSz:%i\n", l.tailSegSz * l.tailSegCnt);
      
      // remove the offset
      removeOffset(l.offset);
      
      // there should only be one tail segment
      undoSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'u';

      decTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'd';
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
      
      //addOffset(phase.priorTailSegSz);//'t...'
      //addOffset(l.tailSegCnt);//'t...'
      
      // there should only be one tail segment
      //decrementTailSegCnt(1);
      
      // add possible offset for the initial segment size
      addOffset(l.tailSegSz - phase.iniSegSz);
      
      //removeOffset(l.tailSegSz);
      
      /*
      // normalization NOTE: the maybe another way of doing this
      int tailSegSz;
      
      if (phase.secondSegSz < phase.thirdSegSz)
      tailSegSz = phase.secondSegSz;
      else
      tailSegSz = phase.thirdSegSz;
      */
      //removeOffset(tailSegSz * (l.tailSegCnt - 1));//'v...'
      //decrementTailSegCnt(l.tailSegCnt - 1);
      
      //decrementTailSegCnt(l.tailSegCnt - 1);
      //addOffset(l.headSz);//'t...'
   }
   
   if (phase.build_line_op == small_third_head_segment_found)
   {
      tv.opTracker[strlen(tv.opTracker)] = '8';
      
      newSegSm(&l);
      tv.opTracker[strlen(tv.opTracker)] = 's';
   }
   
   /*if (phase.build_line_op == large_third_head_segment_found)
   {
      tv.opTracker[strlen(tv.opTracker)] = '9';
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
   }*/
   
   if (phase.build_line_op == convert_to_small_head)
   {
      tv.opTracker[strlen(tv.opTracker)] = '1';
      tv.opTracker[strlen(tv.opTracker)] = '0';
      
      // make sure there is enough offset for the conversion
      if (l.headSz > l.offset)
      addOffset(l.headSz + l.tailSegSz);
      
      incPriorTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'p';
      convertLgToSm(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'S';
      
      incTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'i';
      
      addOffset(phase.priorTailSegSz);//'t...'
   }
   
   if (phase.build_line_op == convert_to_small_head_add_large_head)
   {
      tv.opTracker[strlen(tv.opTracker)] = '1';
      tv.opTracker[strlen(tv.opTracker)] = '1';
      
      // make sure there is enough offset for the conversion
      if (l.headSz > l.offset)
      addOffset(l.headSz + l.tailSegSz);
      
      incPriorTailSegCnt(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'p';
      convertLgToSm(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'S';
      
      removeOffset(l.headSz);
      
      
      newSegMd(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'm';
      
      newSegLg(&l);
      tv.opTracker[strlen(tv.opTracker)] = 'l';
   }
   
   phase.build_line_op = -1; // no operation
   
   
   
   
   
   
   
   
   
   if (0)// DO NOT ENTER
   if (bitVal != getLinePattBit(l.x, l.y, pv->buffInc+l.offset))
   {
      printf(" ---------- Line Modeling Phase:%i\n", modelingPhase);//getModelingPhase(&phase));
      if (modelingPhase == 0)
      {
         incTailSegCnt(&l);
         tv.opTracker[strlen(tv.opTracker)] = 'i';
      }
      else if (modelingPhase == 1) // find the second segment
      {
         tv.opTracker[strlen(tv.opTracker)] = '1';
         
         //NOTE: iniSegType has a different meaning in this case
            if (phase.iniSegType == EQ_INI_SEG)
            {
               //newSegSm(&l);
               //tv.opTracker[strlen(tv.opTracker)] = 's';
               decTailSegCnt(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'd';
               newSegMd(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'm';
               
               tv.opTracker[strlen(tv.opTracker)] = 'v';
            }
            else if (phase.iniSegType == LG_INI_SEG)
            {
               newSegSm(&l);
               tv.opTracker[strlen(tv.opTracker)] = 's';
               /*
               if (phase.unknownSegSz)
               {
                  decPriorTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'P';
                  undoTruncate(&l, 1);
                  tv.opTracker[strlen(tv.opTracker)] = 'v';
                  undoTruncate(&l, 1);
                  tv.opTracker[strlen(tv.opTracker)] = 'v';
               }*/
            }
            else if (phase.iniSegType == SM_INI_SEG)
            {
               // back out
               undoTruncate(&l, 1);
               tv.opTracker[strlen(tv.opTracker)] = 'v';
               decTailSegCnt(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'd';
               
               // rebuild
               newSegLg(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'l';
            }
         //}
         /*else if (bitVal == 1)//if (phase.iniHeadSz)
         {tv.opTracker[strlen(tv.opTracker)] = '@';
            printf("iniHeadSz:%i iniTailSegSz:%i\n", phase.iniHeadSz, phase.iniTailSegSz);
      
            decTailSegCnt(&l);
            tv.opTracker[strlen(tv.opTracker)] = 'd';
            decTailSegCnt(&l);
            tv.opTracker[strlen(tv.opTracker)] = 'd';
            
            newSegLg(&l);
            tv.opTracker[strlen(tv.opTracker)] = 'l';
            
            addOffset(2);
         }*/
         else
         {
            incTailSegCnt(&l);
            tv.opTracker[strlen(tv.opTracker)] = 'i';
            
            addOffset(phase.priorTailSegSz);
         }
      }
      else if (modelingPhase == 2)
      {
         tv.opTracker[strlen(tv.opTracker)] = '2';
         
         if ((phase.headFound) && (phase.tailSegFound))
         {
            if (flipFlopCnt)
            {
               if (headSz > tailSegSz)
               {
                  if (phase.iniSegType == SM_INI_SEG)
                  {
                     
                  }
               }
               else
               {
                  
               }
            }
            else
            {
               if (headSz > tailSegSz)
               {
               }
               else
               {
                  //if (l.headSzPrior > l.tailSegSzPrior)
                  //   tv.opTracker[strlen(tv.opTracker)] = '?';
                  
                  if (phase.iniSegType == LG_INI_SEG)
                  //if (phase.tailSegSz == phase.secondSegSz)
                  //if (phase.priorHeadSz < phase.priorTailSegSz)
                  {
                     convertSmToLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'L';
                     decPriorTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'P';
                     
                     incTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'i';
                     
                     //printf("@@@@@@@@@@@@@@@@@@@@ priorHeadSz:%i\n", phase.priorHeadSz);
                     //printf("@@@@@@@@@@@@@@@@@@@@ priorTailSegSz:%i\n", phase.priorTailSegSz);
                     //printf("@@@@@@@@@@@@@@@@@@@@ tailSegCntA:%i\n", tailSegCntA);
                     //printf("@@@@@@@@@@@@@@@@@@@@ tailSegCntB:%i\n", tailSegCntB);
                     //addOffset(l.headSz);
                     //printf("@@@@@@@@@@@@@@@@@@@@ priorTailSegSz:%i", phase.priorTailSegSz);
                     //addOffset(phase.secondSegSz * phase.secondSegCnt);
                     addOffset(tailSegSz * tailSegCntB);
                     //addOffset(5);
                     //removeOffset(phase.priorTailSegSz);
                  }
                  
                  if (phase.iniSegType == SM_INI_SEG)
                  {
                     newSegMd(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'm';
                     
                     newSegSm(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 's';
                  }
               }
            }
            /*
            //x=3; y=5; shift=1;
            if (phase.iniSegType == EQ_INI_SEG)
            //if (phase.thirdSegSz < phase.secondSegSz)
            {tv.opTracker[strlen(tv.opTracker)] = '!';
               addMdSegs(2);
               
               newSegSm(&l);
               tv.opTracker[strlen(tv.opTracker)] = 's';
               
               addOffset(phase.tailSegSz);
            }
            
            //if (phase.thirdSegFound == FALSE) //last updated 07-31-2023
            if (phase.iniSegType == SM_INI_SEG)
            {
               decTailSegCnt(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'd';
               
               undoTruncate(&l, phase.priorTailSegSz);
               for (int i=0; i < phase.priorTailSegSz; i++)
               tv.opTracker[strlen(tv.opTracker)] = 'v';
            }*/
         }
         else
         {
            /*if (phase.headFound == FALSE)
            if (phase.thirdSegSz < phase.secondSegSz)
            {
               tv.opTracker[strlen(tv.opTracker)] = '@';
               incTailSegCnt(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'i';
               truncate(&l, l.tailSegSz);
               for (int i=0; i < l.tailSegSz; i++)
               tv.opTracker[strlen(tv.opTracker)] = 't';
            }*/
            
            
            //x=6; y=8; shift=1;
            /*if (phase.iniSegType == EQ_INI_SEG)
            if (phase.thirdSegSz < phase.secondSegSz)
            //if (phase.iniSegType == LG_INI_SEG)
            {tv.opTracker[strlen(tv.opTracker)] = '@';
               
               //undoTruncate(&l, 1);
               //tv.opTracker[strlen(tv.opTracker)] = 'v';
               //decTailSegCnt(&l);
               //tv.opTracker[strlen(tv.opTracker)] = 'd';
               
               addMdSegs(phase.secondSegCnt+1); // +1 for iniSeg
               newSegLg(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'l';
            }*/
            
            //x=2; y=3; shift=1;
            //x=2; y=5; shift=3;
            if (phase.thirdSegFound)
            if (phase.thirdSegSz > phase.iniSegSz) // stepping stone
            if (phase.thirdSegSz > phase.secondSegSz)
            {
               addMdSegs(phase.secondSegCnt);
               newSegLg(&l);
               tv.opTracker[strlen(tv.opTracker)] = 'l';
            }
            
            //x=3; y=4; shift=4;
            //x=3; y=7; shift=6;
            if (phase.thirdSegFound)
            if (phase.iniSegType == SM_INI_SEG)
            if (phase.thirdSegSz < phase.secondSegSz)
            {
               if (phase.iniHeadSz)
               {
                  addMdSegs(1);
                  newSegSm(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 's';
               }
               else // iniHeadSz == 0 // stepping stone
               {
                  // back out
                  undoTruncate(&l, 1);
                  tv.opTracker[strlen(tv.opTracker)] = 'v';
                  decTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'd';
                  
                  // rebuild
                  newSegLg(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'l';
               }
            }
            
            
            if (phase.thirdSegFound == FALSE) // last updated 07-31-2023
            if (phase.iniSegType == SM_INI_SEG)
            {
               if (phase.priorHeadSz > phase.priorTailSegSz)// NOTE: this might need to be verified
               {
                  newSegLg(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'l';
                  
                  addOffset(phase.secondSegSz + phase.priorTailSegSz);
               }
               else
               {
                  newSegSm(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 's';
                  
                  addOffset(phase.priorHeadSz + phase.priorTailSegSz);
               }
            }
         }
      }
      else if (modelingPhase == 3)
      {
         tv.opTracker[strlen(tv.opTracker)] = '3';
         
         if (phase.tailSegFound)
         {
            if (flipFlopCnt)
            {
               if (headSz > tailSegSz)
               {
                  if (phase.iniSegType == SM_INI_SEG)
                  {
                     addMdSegs(flipFlopCnt);
                     newSegLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'l';
                  }
                  
                  if (phase.iniSegType == EQ_INI_SEG)
                  {
                     addMdSegs(flipFlopCnt);
                     newSegLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'l';
                  }
                  
                  if (phase.iniSegType == LG_INI_SEG)
                  {
                     addMdSegs(flipFlopCnt);
                     newSegSm(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 's';
                  }
               }
               else
               {
                  if (phase.iniSegType == SM_INI_SEG)
                  {tv.opTracker[strlen(tv.opTracker)] = '@';
                     incPriorTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'p';
                     convertLgToSm(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'S';
                     
                     addMdSegs(flipFlopCnt);
                     
                     newSegLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'l';
                     
                     addOffset(tailSegSz);
                  }
                  
                  if (phase.iniSegType == EQ_INI_SEG)
                  {
                     incPriorTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'p';
                     convertLgToSm(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'S';
                     printf("needs to be flipFlopCnt-1\n");
                     addMdSegs(flipFlopCnt-1);
                     
                     newSegLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'l';
                     
                     removeOffset(headSz);
                  }
                  
                  if (phase.iniSegType == LG_INI_SEG)
                  {
                     addMdSegs(flipFlopCnt);
                     
                     newSegLg(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'l';
                  }
               }
            }
            else
            {
               if (headSz > tailSegSz)
               {
                  addOffset(phase.tailSegSz);
                  
                  incTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'i';
               }
               else
               {
                  if (phase.iniSegType == SM_INI_SEG)
                  {
                     if (tailSegCntA == 0)
                     {
                        convertLgToSm(&l);
                        tv.opTracker[strlen(tv.opTracker)] = 'S';
                        incPriorTailSegCnt(&l);
                        tv.opTracker[strlen(tv.opTracker)] = 'p';
                        
                        addOffset(tailSegSz);
                     }
                     
                     if (tailSegCntA)
                     {
                        incPriorTailSegCnt(&l);
                        tv.opTracker[strlen(tv.opTracker)] = 'p';
                        convertLgToSm(&l);
                        tv.opTracker[strlen(tv.opTracker)] = 'S';
                        incTailSegCnt(&l);
                        tv.opTracker[strlen(tv.opTracker)] = 'i';
                        
                        addOffset(1);//prior tail??
                     }
                  }
                  
                  if (phase.iniSegType == EQ_INI_SEG)
                  {
                     incPriorTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'p';
                     convertLgToSm(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'S';
                     
                     addOffset(tailSegSz + phase.priorTailSegSz);//addOffset(l.tailSegSz + l.tailSegSzPrior);
                  }
                  
                  //j:1911 x=7; y=18; shift=13;
                  if (phase.iniSegType == LG_INI_SEG)
                  {
                     incTailSegCnt(&l);
                     tv.opTracker[strlen(tv.opTracker)] = 'i';
                     
                     addOffset(tailSegSz);
                  }
               }
            }
         }
         else
         {
            decTailSegCnt(&l);
            tv.opTracker[strlen(tv.opTracker)] = 'd';
            
            //addOffset(l.headSz);
            removeOffset(phase.secondSegSz);
         }
      }
      else if (modelingPhase == 4) // the tail is found
      {
         tv.opTracker[strlen(tv.opTracker)] = '4';
         
         if (headCnt == 1)
         {
            if (headSz > tailSegSz)
            {
               if (tailSegCntB == 0)
               {
                  addMdSegs(tailSegCntA);//phase.secondSegCnt);
                  
                  newSegLg(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'l';
               }
            
               if (tailSegCntA < tailSegCntB)
               {
                  incTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'i';
                  
                  addOffset(tailSegSz);//phase.tailSegSz);
               }
            }
            else
            {
               if (tailSegCntB == 0)
               {
                  addMdSegs(tailSegCntA);
                  
                  newSegSm(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 's';
               }
            
               if (tailSegCntA < tailSegCntB)
               {
                  incTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'i';
                  
                  addOffset(phase.tailSegSz);
               }
            }
         }
         else if (headCnt == 2)
         {
            if (headSz > tailSegSz)
            {
               if (tailSegCntA == tailSegCntB)
               {
                  undoTruncate(&l, phase.tailSegSz);
                  for (int i=0; i < phase.tailSegSz; i++)
                  tv.opTracker[strlen(tv.opTracker)] = 'v';
                  
                  decTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'd';
               }
               
               if (tailSegCntA > tailSegCntB)
               {
                  undoTruncate(&l, phase.tailSegSz);
                  for (int i=0; i < phase.tailSegSz; i++)
                  tv.opTracker[strlen(tv.opTracker)] = 'v';
                  
                  decTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'd';
                  newSegSm(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 's';
               }
            }
            else
            {
               if (tailSegCntA == tailSegCntB)
               {
                  newSegSm(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 's';
               }
               
               if (tailSegCntA > tailSegCntB)
               {
                  tv.opTracker[strlen(tv.opTracker)] = '!';
                  
                  decTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'd';
                  
                  decTailSegCnt(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'd';
                  
                  //newSegSm(&l);
                  //tv.opTracker[strlen(tv.opTracker)] = 's';
                  
                  newSegLg(&l);
                  tv.opTracker[strlen(tv.opTracker)] = 'l';
                  
                  //if (tailSegCntB == 1)
                  //addOffset((headSz + tailSegSz) * 2);
                  addOffset(headSz);
                  
                  //addOffset(headSz*headCnt + tailSegSz);
                  //addOffset(tailSegSz*tailSegCntB);
                  
                  //addOffset(tailSegSz*(tailSegCntA+tailSegCntB) + headSz*headCnt);
                  
                  //removeOffset(tailSegSz);
               }
            }
         }
      }
   }
   
   // create new line segment
   accountNewLineSeg(&phase,
   headSz,
   headCnt,
   tailSegSz,
   tailSegCntA,
   tailSegCntB,
   flipFlopCnt,
   pattLoc, bitVal);
   
   // check if the bitVal matches
   if (0)
   if (bitVal != getLinePattBit(l.x, l.y, pv->buffInc+l.offset))
   {
      printf("/////////Bit values do not match////////////\n");
      exit(0);
   }
}



int checkLinePatt(int bitVal, struct pattern_variables *pv)
{
   if (pv->cntB)
   {
      // check if the given pattern bit value is allowed
      if (checkBit(bitVal, pv))
      return 1;
      
      buildLine(bitVal, pv);
      
      // grow the pattern model that will be used to tell if the line is straight
      growPattModel(bitVal, pv);
   }
   else
   {
      if (iniCheckBit(bitVal, pv))
      return 1;
      
      buildLine(bitVal, pv);
      
      iniGrowPattModel(bitVal, pv);
   }
   
   return 0;
}


int getLine(char *patt, int pattSz, int *rise, int *run, int *offset, int *length)
{
   int i;
   char pattBuff[LINE_PATT_BUFF_SZ];
   struct pattern_variables *pv;
   
   iniModelingPhaseVars(&phase);

   pv = iniPattVars(pattBuff);

   iniTestVars();
   iniBuildLine(&l);
   
   for (i=0; i < pattSz; i++)
   if (checkLinePatt(patt[i], pv))
   return 1;


   
   *rise = l.y;
   *run = l.x;
   *offset = l.offset;// + tv.trunOffset;
   *length = pattSz + 1;//l.length;//
   printLinePattVis5(*rise, *run, *offset, *length);
   printf("ops: %s\n", tv.opTracker);
   //printf("offset:%i\n", l.offset);
   //printVars(pv);

   //printf("LINE:%i\n",__LINE__);
   return 0;
}
