#include <stdio.h>
#include <string.h>
#include <stdlib.h> //exit()
#include "modeling_phase.h"

char test_iniSegType[4][3] = {"NA\0", "SM\0", "EQ\0", "LG\0"}; //temporary

void findIniSeg(struct modeling_phase *phase, int pattLoc, int pattBitVal)
{
   int lineSz = pattLoc + 1;
   static int oneBitValCnt = 0;//NOTE: temporary
   
   //printf("pattBitVal:%i\n", pattBitVal);
   
   //printf("lineSz-phase->iniSegSz:%i\n", lineSz-phase->iniSegSz);
   //printf("phase->iniSegSz:%i\n", phase->iniSegSz);
   
   // find the Initial Segment
   if (phase->iniSegFound == FALSE)
   {
      if (pattBitVal)
      {
         phase->iniSegFound = TRUE;
         phase->iniSegSz = pattLoc;
         phase->build_line_op = initial_new_medium_top_segment;
         
         oneBitValCnt = 0;//reset
      }
      else
      {
         phase->build_line_op = increment_tail_segment;
      }
   }
   else
   {
      if (oneBitValCnt == 1)
      if (!pattBitVal)
      if (lineSz-phase->iniSegSz > phase->iniSegSz)
      {
         phase->build_line_op = lengthen_second_segment_size;
      }
   }
   
   if (pattBitVal)
   oneBitValCnt++;
}

void accountNewLineSeg(struct modeling_phase *phase,
int headSz,
int headCnt,
int tailSegSz,
int tailSegCntA,
int tailSegCntB,
int flipFlopCnt,
int pattLoc, int pattBitVal)
{
   int lineSz;
   
   // return if tail is not found
   if ((phase->thirdSegFound == FALSE) ||
       (phase->tailSegFound == FALSE))
   return;
   
   
   // return if second head is not found
   if (headCnt < 2)
   return;
   
   // define new line segment
   printf("Creating New Line Segment ----------\n");

   // find the line size
   if (pattBitVal)
   lineSz = pattLoc;
   else
   lineSz = pattLoc + 1;
   
   
   if (flipFlopCnt)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      
      phase->priorHeadSz = headSz;
      phase->priorTailSegSz = tailSegSz;
      phase->priorTailSegCnt = tailSegCntB;
      
      phase->iniSegSz = lineSz - (tailSegSz+headSz)*flipFlopCnt - tailSegSz*2;
      
      phase->secondSegSz = tailSegSz + headSz;
      phase->secondSegCnt = flipFlopCnt;
      phase->secondSegFound = TRUE;
      
      phase->lastSegSz = phase->secondSegSz;//headSz;
      
      // find the initial segment type
      if (tailSegCntA == tailSegCntB)
      phase->iniSegType = EQ_INI_SEG;
      else if (tailSegCntA > tailSegCntB)
      phase->iniSegType = LG_INI_SEG;
      else
      phase->iniSegType = SM_INI_SEG;
      
      // reset variables
      phase->thirdSegFound = FALSE;
      phase->thirdSegSz = 0;
      phase->thirdSegCnt = 0;
      
      phase->tailSegFound = FALSE;
      phase->tailSegSz = 0;
      
      phase->tailSegInc = 0;
      
      phase->headFound = FALSE;
      phase->headSz = 0;
   }
   else
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      
      phase->priorHeadSz = headSz;
      phase->priorTailSegSz = tailSegSz;
      phase->priorTailSegCnt = tailSegCntB;
      
      phase->iniSegSz = lineSz - (tailSegSz*tailSegCntB + headSz);
      
      phase->secondSegSz = tailSegSz*tailSegCntB + headSz;
      phase->secondSegCnt = 1;
      phase->secondSegFound = TRUE;
      
      phase->lastSegSz = phase->secondSegSz;//headSz;
      
      // find the initial segment type
      if (tailSegCntA == tailSegCntB)
      phase->iniSegType = EQ_INI_SEG;
      else if (tailSegCntA > tailSegCntB)
      phase->iniSegType = LG_INI_SEG;
      else
      phase->iniSegType = SM_INI_SEG;
      
      // reset variables
      phase->thirdSegFound = FALSE;
      phase->thirdSegSz = 0;
      phase->thirdSegCnt = 0;
      
      phase->tailSegFound = FALSE;
      phase->tailSegSz = 0;
      
      phase->tailSegInc = 0;
      
      phase->headFound = FALSE;
      phase->headSz = 0;
   }
   
   printf("updated iniSegSz:%i\n", phase->iniSegSz);
   printf("updated secondSegSz:%i\n", phase->secondSegSz);
   printf("updated secondSegCnt:%i\n", phase->secondSegCnt);
   printf("updated thirdSegSz:%i\n", phase->thirdSegSz);
   printf("updated thirdSegCnt:%i\n", phase->thirdSegCnt);
   
   printf("updated priorHeadSz:%i\n", phase->priorHeadSz);
   printf("updated priorTailSegSz:%i\n", phase->priorTailSegSz);
   printf("updated priorTailSegCnt:%i\n", phase->priorTailSegCnt);
   
   printf("updated lastSegSz:%i\n", phase->lastSegSz);
   printf("initial segment type:%s\n", test_iniSegType[phase->iniSegType]);
}


////////////////////////////////////////////////////////////
//               BEGINNING OF MATCH SEGMENT
////////////////////////////////////////////////////////////

static void matchSeg_small(struct modeling_phase *phase, int lineSzPriorBit)
{
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_sm = lineSzPriorBit - segments;
   
   // count the number of segments matching the Second Segment
   if (phase->secondSegFound)
   if (currSegSz_sm == phase->secondSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->secondSegCnt++;
      printf("second segment, count:%i\n", phase->secondSegCnt);
      
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->headFound)
      if (phase->tailSegFound)
      {
         phase->tailSegInc++;
         printf("tail increment:%i\n", phase->tailSegInc);
      }
      
      if (phase->thirdSegFound)
      if (phase->tailSegFound)
      //if (phase->lastSegSz == phase->thirdSegSz)
      if (phase->tailSegSz == phase->thirdSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      if (phase->tailSegFound == FALSE)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->secondSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
         
         if (phase->thirdSegFound) // updated 08-01-2023
         {
            phase->headFound = TRUE;
            phase->headSz = phase->thirdSegSz;
            printf("head found, size:%i\n", phase->headSz);
            
            phase->tailSegInc = 2;
            printf("tail increment:%i\n", phase->tailSegInc);
         }
         else if (phase->iniSegType == LG_INI_SEG) // updated 08-01-2023
         {
            phase->headFound = TRUE; // updated 08-01-2023
            phase->headSz = phase->iniSegSz;
            printf("head found, size:%i\n", phase->headSz);
            
            phase->tailSegInc = 2; // updated 08-01-2023
            printf("tail increment:%i\n", phase->tailSegInc);
         }
      }
      
      if (phase->thirdSegFound == FALSE)
      phase->build_line_op = new_medium_top_segment;
      
      if (phase->tailSegInc)
      {
         int headSz, tailSegSz;
         int tailSegCntA, tailSegCntB;
         
         if (phase->tailSegSz == phase->secondSegSz)
         {
            headSz = phase->thirdSegSz;
            tailSegSz = phase->secondSegSz;
            tailSegCntA = phase->secondSegCnt - phase->tailSegInc;
         }
         else
         {
            headSz = phase->secondSegSz;
            tailSegSz = phase->thirdSegSz;
            tailSegCntA = phase->thirdSegCnt - phase->tailSegInc;
         }
         
         tailSegCntB = phase->tailSegInc;
         
         if (phase->flipFlopCnt == 2)
         if (headSz > tailSegSz)
         if (tailSegCntB > tailSegCntA)
         phase->build_line_op = increment_tail_segment_truncate;
         
         printf("tailSegCntA:%i\n", tailSegCntA);
         printf("tailSegCntB:%i\n", tailSegCntB);
         //printf("flipFlopCnt:%i\n", phase->flipFlopCnt);
         if (phase->flipFlopCnt == 3)
         if (tailSegCntA == tailSegCntB+1)
         phase->build_line_op = small_third_head_segment_found;
      }
      
      phase->lastSegSz = phase->secondSegSz;
   }
   
   // count the number of segments matching the Third Segment
   if (phase->thirdSegFound)
   if (currSegSz_sm == phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->thirdSegCnt++;
      printf("third segment, count:%i\n", phase->thirdSegCnt);
      
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->headFound)
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->thirdSegSz) // updated 08-01-2023
      {
         phase->tailSegInc++;
         printf("tail increment:%i\n", phase->tailSegInc);
      }
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      if (phase->tailSegFound == FALSE)
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->thirdSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
         
         phase->headFound = TRUE; // updated 08-01-2023
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
         
         phase->tailSegInc = 2; // updated 08-01-2023
         printf("tail increment:%i\n", phase->tailSegInc);
      }
      
      int tailSegCntA, tailSegCntB;
      
      if (phase->tailSegSz == phase->secondSegSz)
      tailSegCntA = phase->secondSegCnt - phase->tailSegInc;
      else
      tailSegCntA = phase->thirdSegCnt - phase->tailSegInc;
      
      tailSegCntB = phase->tailSegInc;
      
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->build_line_op = increment_tail_segment_truncate;
         
         if (phase->flipFlopCnt == 3)
         if (tailSegCntB > tailSegCntA)
         phase->build_line_op = large_third_segment_found_unknown_head;
      }
      
      if (phase->lastSegSz == phase->secondSegSz)
      {
         //if (phase->flipFlopCnt == 3)
         if (phase->tailSegFound)
         {
            if (tailSegCntB+1 == tailSegCntA)
            phase->build_line_op = small_third_head_segment_found;
            
            //if (tailSegCntB == tailSegCntA+1)
            //phase->build_line_op = large_third_segment_found_unknown_head;
         }
      }
      
      phase->lastSegSz = phase->thirdSegSz;
   }
}

static void matchSeg_large(struct modeling_phase *phase, int lineSz)
{//printf("here\n");
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_lg = lineSz - segments;
   /*
      printf("currSegSz_lg:%i\n", currSegSz_lg);
   printf("phase->secondSegSz:%i\n", phase->secondSegSz);
   printf("phase->iniSegSz - phase->priorTailSegSz:%i\n", phase->iniSegSz - phase->priorTailSegSz);
   printf("phase->iniHeadSz:%i\n", phase->iniHeadSz);
         //printf("phase->lastSegSz:%i\n", phase->lastSegSz);
   printf("phase->priorTailSegSz:%i\n", phase->priorTailSegSz);
   printf("phase->secondSegCnt:%i\n", phase->secondSegCnt);
   */
   //if (phase->secondSegCnt > 1)
   //if (phase->iniHeadSz)
   //printf("currSegSz_lg:%i\n", currSegSz_lg);
   //if (0)
   
   if (phase->iniSegFound)
   if (phase->secondSegFound)
   if ((phase->thirdSegFound) || (phase->iniHeadSz > phase->iniTailSegSz))
   //if ((phase->thirdSegFound) && (phase->thirdSegSz < phase->secondSegSz))
   //if (phase->iniSegFound < phase->secondSegSz)// || ((phase->thirdSegFound) && (phase->thirdSegSz > )
   if (phase->secondSegSz > phase->thirdSegSz)
   if (currSegSz_lg == phase->secondSegSz)
      
   //if ((phase->iniHeadSz > phase->iniTailSegSz) || ((phase->thirdSegFound) && (currSegSz_lg > phase->thirdSegSz)))
   //if (phase->iniHeadSz > phase->iniTailSegSz)
   //if ((phase->iniHeadSz) || ((phase->thirdSegFound) && (currSegSz_lg > phase->thirdSegSz)))
   //if (phase->thirdSegFound)
   //if (currSegSz_lg > phase->thirdSegSz)
   //if ((!phase->iniHeadSz) || (phase->iniHeadSz > phase->iniTailSegSz))
   //if (currSegSz_lg == phase->secondSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->secondSegCnt++;
      printf("second segment, count:%i\n", phase->secondSegCnt);
      
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->headFound)
      if (phase->tailSegFound)
      if (currSegSz_lg == phase->tailSegSz)
      {
         phase->tailSegInc++;
         printf("tail increment:%i\n", phase->tailSegInc);
      }
      
      if (phase->thirdSegFound)
      if (phase->tailSegFound)
      //if (phase->lastSegSz == phase->thirdSegSz)
      if (phase->tailSegSz == phase->thirdSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      if (phase->tailSegFound == FALSE)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->secondSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
         
         phase->headFound = TRUE; // updated 08-01-2023
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
         
         phase->tailSegInc = 2; // updated 08-01-2023
         printf("tail increment:%i\n", phase->tailSegInc);
         
         phase->build_line_op = convert_to_small_head;
      }
      
      if (phase->tailSegInc)
      {
         int headSz, tailSegSz;
         int tailSegCntA, tailSegCntB;
         
         if (phase->tailSegSz == phase->secondSegSz)
         {
            headSz = phase->thirdSegSz;
            tailSegSz = phase->secondSegSz;
            tailSegCntA = phase->secondSegCnt - phase->tailSegInc;
         }
         else
         {
            headSz = phase->secondSegSz;
            tailSegSz = phase->thirdSegSz;
            tailSegCntA = phase->thirdSegCnt - phase->tailSegInc;
         }
         
         tailSegCntB = phase->tailSegInc;
         
         printf("tailSegCntA:%i\n", tailSegCntA);
         printf("tailSegCntB:%i\n", tailSegCntB);
         
         if (phase->flipFlopCnt == 2)
         if (tailSegCntB > tailSegCntA+1)
         if (headSz < tailSegSz)
         phase->build_line_op = increment_tail_segment_truncate;
         
         /*if (phase->tailSegInc == 2)
         if (phase->lastSegSz == phase->secondSegSz)
         if (tailSegCntB == tailSegCntA+1)
         phase->build_line_op = convert_to_small_head;*/
      }
      
      phase->lastSegSz = phase->secondSegSz;
   }
   
   if (phase->secondSegFound)
   if (phase->thirdSegFound)
   if (currSegSz_lg > phase->secondSegSz)
   if (currSegSz_lg == phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->thirdSegCnt++;
      printf("third segment, count:%i\n", phase->thirdSegCnt);
      
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->headFound)
      if (phase->tailSegFound)
      if (phase->tailSegSz == phase->thirdSegSz) // updated 08-01-2023
      {
         phase->tailSegInc++;
         printf("tail increment:%i\n", phase->tailSegInc);
         
         phase->build_line_op = increment_tail_segment_truncate;
      }
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      if (phase->tailSegFound == FALSE)
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->thirdSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
         
         phase->headFound = TRUE; // updated 08-01-2023
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
         
         phase->tailSegInc = 2; // updated 08-01-2023
         printf("tail increment:%i\n", phase->tailSegInc);
      }
      
      int tailSegCntA, tailSegCntB;
      
      if (phase->tailSegSz == phase->secondSegSz)
      tailSegCntA = phase->secondSegCnt - phase->tailSegInc;
      else
      tailSegCntA = phase->thirdSegCnt - phase->tailSegInc;
      
      tailSegCntB = phase->tailSegInc;
      
      if (phase->tailSegFound)
      if (tailSegCntA == tailSegCntB+1)
      phase->build_line_op = small_third_head_segment_found;
      
      if (phase->tailSegInc == 2)//tailSegFound
      if (phase->lastSegSz == phase->thirdSegSz)
      {
         phase->build_line_op = convert_to_small_head;
         
         if (phase->flipFlopCnt == 3)
         if (tailSegCntB == tailSegCntA+1)
         phase->build_line_op = convert_to_small_head_add_large_head;
      }
      
      phase->lastSegSz = phase->thirdSegSz;
   }
}

static void matchSecondSeg_small(struct modeling_phase *phase, int lineSzPriorBit)
{
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_sm = lineSzPriorBit - segments;
   
   //printf("............priorHeadSz:%i\n", phase->priorHeadSz);
   //printf("............priorTailSegSz:%i\n", phase->priorTailSegSz);
   
   //printf("............lastPriorSegInc:%i\n", phase->lastPriorSegInc);
   //if ((phase->priorHeadSz) && (currSegSz_sm == phase->priorHeadSz + phase->priorTailSegSz)
   
   //if ((!phase->iniHeadSz) || (phase->lastPriorSegInc == phase->priorHeadSz))
   //if ((phase->iniHeadSz < phase->iniTailSegSz) || (phase->lastPriorSegInc == phase->priorHeadSz))
   
   //if (phase->lastPriorSegInc == phase->priorHeadSz)
   //if (phase->lastPriorSegInc != phase->priorTailSegSz)
   //if ((!phase->iniHeadSz) || (phase->lastPriorSegInc != phase->priorTailSegSz))
   if (phase->lastPriorSegInc != phase->priorTailSegSz)
   if (phase->iniSegFound)
   if (phase->secondSegFound == FALSE)
   if (phase->iniHeadSz < phase->iniTailSegSz)
   if (currSegSz_sm != 0)
   if ((currSegSz_sm >= phase->iniSegSz - phase->priorTailSegSz) ||
       (currSegSz_sm >= phase->iniSegSz + phase->priorTailSegSz))
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->secondSegFound = TRUE;
      phase->secondSegSz = currSegSz_sm;
      phase->secondSegCnt++;
      printf("second segment found, size:%i\n", phase->secondSegSz);
      
      // find the initial segment type
      if (phase->iniSegSz == phase->secondSegSz)
      phase->iniSegType = EQ_INI_SEG;
      else
      {
         if (phase->iniSegSz > phase->secondSegSz)
         phase->iniSegType = LG_INI_SEG;
         else
         phase->iniSegType = SM_INI_SEG;
      }
      printf("initial segment type:%s\n", test_iniSegType[phase->iniSegType]);
      
      phase->lastSegSz = phase->secondSegSz;
      
      if (phase->iniSegType == LG_INI_SEG)
      {
         // redefine the initial and second segment (the initial segment must be larger than the second)
         int iniSegSz = phase->iniSegSz;
         int secondSegSz = phase->secondSegSz;
         
         phase->iniSegSz -= phase->secondSegSz + phase->priorTailSegSz;
         phase->iniSegType = SM_INI_SEG;
         printf("initial segment redefined: %i\n", phase->iniSegSz);
         
         phase->secondSegSz = iniSegSz;
         printf("second segment redefined: %i\n", phase->secondSegSz);
         
         // define the third segment
         phase->thirdSegFound = TRUE;
         phase->thirdSegSz = secondSegSz;
         phase->thirdSegCnt++;
         printf("third segment found, size:%i\n", phase->thirdSegSz);
         
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
         
         phase->build_line_op = small_second_segment_found_unknown_head;
         
         phase->lastSegSz = phase->thirdSegSz;
      }
   }
}

static void matchSecondSeg_large(struct modeling_phase *phase, int lineSz)
{
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_lg = lineSz - segments;
   //printf("%i >= %i - %i\n", currSegSz_lg, phase->iniSegSz, phase->priorTailSegSz);
   if (phase->iniSegFound)
   if (phase->secondSegFound == FALSE)
   //if (currSegSz_lg != 0)
   // GREATER THAN OR EQUAL TO
   if (currSegSz_lg >= phase->iniSegSz - phase->priorTailSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->secondSegFound = TRUE;
      phase->secondSegSz = currSegSz_lg;
      phase->secondSegCnt++;
      printf("second segment found, size:%i\n", phase->secondSegSz);
      
      // find the initial segment type
      if (phase->iniSegSz == phase->secondSegSz)
      phase->iniSegType = EQ_INI_SEG;
      else
      {
         if (phase->iniSegSz > phase->secondSegSz)
         phase->iniSegType = LG_INI_SEG;
         else
         phase->iniSegType = SM_INI_SEG;
      }
      printf("initial segment type:%s\n", test_iniSegType[phase->iniSegType]);
      
      phase->lastSegSz = phase->secondSegSz;
   }
}

static void matchThirdSeg_small(struct modeling_phase *phase, int lineSzPriorBit)
{
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_sm = lineSzPriorBit - segments;

   if (phase->thirdSegFound == FALSE)
   if (currSegSz_sm != 0)
   if (phase->secondSegSz != 0)
   if ((currSegSz_sm == phase->secondSegSz - phase->priorTailSegSz) ||
       (currSegSz_sm == phase->secondSegSz + phase->priorTailSegSz))
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->thirdSegFound = TRUE;
      phase->thirdSegSz = currSegSz_sm;
      phase->thirdSegCnt++;
      printf("third segment found, size:%i\n", phase->thirdSegSz);
      
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->iniSegType == EQ_INI_SEG)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->secondSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
         
         // redefine the initial segment (the initial segment must be larger than the third)
         phase->iniSegSz -= phase->secondSegSz;
         phase->iniSegType = SM_INI_SEG;
         printf("initial segment redefined: %i\n", phase->iniSegSz);
         phase->secondSegCnt++;
         printf("second segment incremented: %i\n", phase->secondSegCnt);
      }
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      if (phase->tailSegFound)
      phase->build_line_op = small_third_head_segment_found;
      else
      phase->build_line_op = small_third_segment_found_unknown_head;
      
      phase->lastSegSz = phase->thirdSegSz;
   }
}

static void matchThirdSeg_large(struct modeling_phase *phase, int lineSz)
{
   int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   int currSegSz_lg = lineSz - segments;
   
   if (phase->thirdSegFound == FALSE)
   if (phase->secondSegFound)
   if ((currSegSz_lg == phase->secondSegSz + phase->priorTailSegSz) ||
      ((phase->iniHeadSz) && (currSegSz_lg == phase->secondSegSz - phase->priorTailSegSz)))
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->thirdSegFound = TRUE;
      phase->thirdSegSz = currSegSz_lg;
      phase->thirdSegCnt++;
      printf("third segment found, size:%i\n", phase->thirdSegSz);
      
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->flipFlopCnt++;
         printf("flipFlopCnt: %i\n", phase->flipFlopCnt);
      }
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
      //if (phase->headFound)
      phase->build_line_op = large_third_segment_found_unknown_head;//large_third_head_segment_found;
      //else
      //phase->build_line_op = large_third_segment_found_unknown_head;
      
      phase->lastSegSz = phase->thirdSegSz;
   }
}

////////////////////////////////////////////////////////////
//                  END OF MATCH SEGMENT
////////////////////////////////////////////////////////////


void accountModelingPhase(struct modeling_phase *phase, struct line *l, int pattLoc, int pattBitVal)
{
   int lineSzPriorBit = pattLoc;
   int lineSz = pattLoc + 1;
   
   if (pattBitVal) // small initial head size as compared to the initial tail segment
   {
      matchSeg_small(phase, lineSzPriorBit); // find a recurring segment
      
      if (phase->iniHeadSz < phase->iniTailSegSz)
      {
         matchSecondSeg_small(phase, lineSzPriorBit); // find the Second Segment
         matchThirdSeg_small(phase, lineSzPriorBit); // find the Third Segment
      }
   }
   else
   {
      matchSeg_large(phase, lineSz);
      
      if (phase->iniHeadSz > phase->iniTailSegSz)
      matchSecondSeg_large(phase, lineSz);
      
      if ((!phase->iniHeadSz) || (phase->iniHeadSz > phase->iniTailSegSz))
      matchThirdSeg_large(phase, lineSz);
   }
/*
   if (phase->tailSegFound)
   if (phase->iniSegType == LG_INI_SEG)
   {
      //phase->thirdSegFound = TRUE;
      //phase->thirdSegSz = phase->iniSegSz;
      phase->headFound = TRUE;
      phase->headSz = phase->iniSegSz;
   }
   */
   // get the initial head and tail segment sizes
   if (!phase->iniHeadSz)
   if (phase->thirdSegFound)
   if (phase->tailSegFound)
   {
      if (phase->secondSegSz == phase->tailSegSz)
      {
         phase->iniHeadSz = phase->thirdSegSz;
         phase->iniTailSegSz = phase->secondSegSz;
      }
      else
      {
         phase->iniHeadSz = phase->secondSegSz;
         phase->iniTailSegSz = phase->thirdSegSz;
      }
      printf("  Initial Head and Tail Segment Size, iniHeadSz:%i iniTailSegSz:%i line:%i\n", phase->iniHeadSz, phase->iniTailSegSz, __LINE__);
   }
   
   // increment variable until the end of the last prior segment
   if (pattBitVal) // for matchSecondSeg_small()
   {
      if (phase->lastPriorSegInc >= phase->priorTailSegSz)
      phase->lastPriorSegInc = 0;
      
      if (phase->lastPriorSegInc >= phase->priorHeadSz)
      phase->lastPriorSegInc = 0;
      
      //printf("--------------------------------------  %i\n", phase->tailSegFound);
      //printf("--------------------------------------  %i\n", phase->lastPriorSegInc);
      if (phase->headFound)
      phase->lastPriorSegInc = 0;
    
      if (phase->tailSegFound)
      phase->lastPriorSegInc = 0;
   }
   
   //if (phase->iniHeadSz)
   //if (phase->priorTailSegCnt > 1)
   phase->lastPriorSegInc++;
}

int getModelingPhase(struct modeling_phase *phase)
{
   if (phase->tailSegFound)
   return 4;
   
   if (phase->thirdSegFound)
   return 3;
   
   if (phase->secondSegFound)
   return 2;
   
   if (phase->iniSegFound)
   return 1;
   
   return 0; // NOTE:iniSeg is always found after initialization
   
   /*
   phase -- if flipFlopSeg is found ?????????
   
   phase 1 -- initialization
   phase 2 -- get the second segment size
   phase 3 -- get the third segment size
   phase 4 -- get a tail size
   As soon as a new large segment can be created it loops back around.
   */
   
   /*
   --deprecated--
   phase 1 -- initial segment is found
   phase 2 -- second segment is found
   phase 3 -- third segment is found
   phase 4 -- tail segment is found
   As soon as the head is found it loops back around.
   */
}

//NOTE: make variables nonstatic rather than use this function
void iniModelingPhaseVars(struct modeling_phase *phase)
{
   phase->iniSegFound = 0;
   phase->iniSegSz = 0;
   phase->iniSegType = 0;
   
   phase->secondSegFound = 0;
   phase->secondSegSz = 0;
   
   phase->thirdSegFound = 0;
   phase->thirdSegSz = 0;
   
   phase->secondSegCnt = 0;
   phase->thirdSegCnt = 0;
   
   phase->lastSegSz = 0;
   phase->lastPriorSegInc = 0;
   
   phase->tailSegFound = 0;
   phase->tailSegSz = 0;
   
   phase->tailSegInc = 0;
   
   phase->headFound = 0;
   phase->headSz = 0;
   
   phase->priorHeadSz = 0;
   phase->priorTailSegSz = 1;
   phase->priorTailSegCnt = 1;
   
   phase->iniHeadSz = 0;
   phase->iniTailSegSz = 1;
   
   phase->build_line_op = 0;
   
   phase->flipFlopCnt = 0;
}