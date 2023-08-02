#include <stdio.h>
#include <string.h>
#include <stdlib.h> //exit()
#include "modeling_phase.h"

char test_iniSegType[4][3] = {"NA\0", "SM\0", "EQ\0", "LG\0"}; //temporary


static void findIniSegTypeResetVars(struct modeling_phase *phase)
{
   if (phase->secondSegSz)
   {
      phase->secondSegFound = TRUE;
      
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
   }
   else
   {
      phase->secondSegFound = FALSE;
      phase->iniSegType = 0;
   }
   
   // reset variables
   phase->thirdSegFound = FALSE;
   phase->thirdSegSz = 0;
   phase->thirdSegCnt = 0;
   
   phase->tailSegFound = FALSE;
   phase->tailSegSz = 0;
   
   phase->headFound = FALSE;
   phase->headSz = 0;
}

static void tempFuncResetVars(struct modeling_phase *phase)
{
   phase->lastSegSz = 0;//phase->secondSegSz;
   
   phase->thirdSegFound = FALSE;
   phase->thirdSegSz = 0;
   phase->thirdSegCnt = 0;
   
   phase->tailSegFound = FALSE;
   phase->tailSegSz = 0;
   
   phase->headFound = FALSE;
   phase->headSz = 0;
}

void accountNewLineSeg(struct modeling_phase *phase, int pattLoc, int pattBitVal)
{
   int lineSzPriorBit = pattLoc;
   int lineSz = pattLoc + 1;
   //int segments = phase->iniSegSz + (phase->secondSegSz * phase->secondSegCnt) + (phase->thirdSegSz * phase->thirdSegCnt);
   
   // find the Initial Segment
   if (phase->iniSegFound == FALSE)
   if (pattBitVal)
   {
      phase->iniSegFound = TRUE;
      phase->iniSegSz = pattLoc;
   }
   
   // find new segment
   if ((phase->thirdSegFound == FALSE) ||
       (phase->tailSegFound == FALSE))
   return;
   
   
   // define new line segment
   printf("Creating New Line Segment ----------\n");
   
   // 4
   if (phase->headFound == FALSE)
   if (phase->secondSegSz == phase->tailSegSz)
   if (phase->secondSegSz < phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      printf("phase->thirdSegCnt:%i\n", phase->thirdSegCnt);
      if (phase->iniSegType == LG_INI_SEG)
      {
         // determine if the initial and second segments alternate
         if (phase->secondSegCnt-2 > 1) // subtract the 2 tail segments
         if (phase->thirdSegCnt > 1)
         {
            phase->secondSegSz += phase->thirdSegSz;
            phase->iniSegSz = phase->secondSegSz - 1;
            
            phase->secondSegCnt -= 2; // subtract the 2 tail segments
            
            printf("phase->iniSegSz:%i\n", phase->iniSegSz);
            printf("phase->secondSegSz:%i\n", phase->secondSegSz);
            printf("phase->secondSegCnt:%i\n", phase->secondSegCnt);
            
            findIniSegTypeResetVars(phase);
         }
      }
      else if (phase->iniSegType == EQ_INI_SEG)
      {
         printf("secondSegCnt:%i thirdSegCnt:%i\n", phase->secondSegCnt, phase->thirdSegCnt);
         // determine if the initial and second segments alternate
         //if ((phase->secondSegCnt > 1) && (phase->thirdSegCnt-2 > 0)) // subtract the 2 tail segments
         
         // determine if the initial and second segments alternate
         //if (phase->secondSegCnt-2 > 1) // subtract the 2 tail segments
         //if (phase->thirdSegCnt > 1)
         if ((phase->secondSegCnt-2 > 1) && (phase->thirdSegCnt > 1))
         {
            phase->secondSegSz += phase->thirdSegSz;
            
            phase->secondSegCnt -= 2; // subtract the 2 tail segments
            
            printf("phase->iniSegSz:%i\n", phase->iniSegSz);
            printf("phase->secondSegSz:%i\n", phase->secondSegSz);
            printf("phase->secondSegCnt:%i\n", phase->secondSegCnt);
            
            findIniSegTypeResetVars(phase);
         }
         else
         {printf("There is no reason to create new segments\n");//last updated 07-31-2023
            /*
            phase->iniSegSz = phase->secondSegSz * 2 + phase->thirdSegSz;
         
            printf("\nphase->iniSegSz:%i\n", phase->iniSegSz);
            
            phase->iniSegType = 0;
            
            phase->secondSegFound = FALSE;
            phase->secondSegSz = 0;
            
            tempFuncResetVars(phase);
            */
         }
      }
      else
      {
         // return if the tail count is not found
         //if (phase->tailSegCnt == 0) // last updated 08-01-2023
         if (phase->thirdSegCnt <= 1)
         {
            printf("There is no reason to create new segments\n");
            return;
         }
         
         // last updated 08-01-2023
         phase->priorHeadSz = phase->headSz;
         phase->priorTailSegSz = phase->tailSegSz;
         phase->priorTailSegCnt = 1;
         //printf("line size:%i\n", lineSzPriorBit);
         //phase->iniSegSz = lineSzPriorBit - (phase->secondSegSz + phase->thirdSegSz + phase->tailSegSz*2);
         phase->iniSegSz += phase->secondSegSz + phase->thirdSegSz;
         
         phase->secondSegSz = phase->secondSegSz + phase->thirdSegSz;
         phase->secondSegCnt = 1; 
         phase->lastSegSz = phase->secondSegSz;
         
         phase->secondSegFound = TRUE;
         phase->iniSegType = EQ_INI_SEG; // NOTE: create function for this?
         
         // reset variables
         phase->thirdSegFound = FALSE;
         phase->thirdSegSz = 0;
         phase->thirdSegCnt = 0;
         
         phase->tailSegFound = FALSE;
         phase->tailSegSz = 0;
         
         phase->headFound = FALSE;
         phase->headSz = 0;
         
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
      
      return;
   }
   
   // 3
   if (phase->headFound == FALSE)
   if (phase->secondSegSz == phase->tailSegSz)
   if (phase->secondSegSz > phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__); // last updated:08-01-2023
      printf("There is no reason to create new segments\n");//last updated 07-31-2023
      return;
      /*
      phase->iniSegSz = phase->secondSegSz * 2 + phase->thirdSegSz;
      
      printf("updated phase->iniSegSz:%i\n", phase->iniSegSz);
      
      phase->iniSegType = 0;
      
      phase->secondSegFound = FALSE;
      phase->secondSegSz = 0;
      
      tempFuncResetVars(phase);
      return;*/
   }
   
   // 2
   if (phase->headFound == FALSE)
   if (phase->thirdSegSz == phase->tailSegSz)
   if (phase->secondSegSz < phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__); // last updated:07-31-2023
      printf("There is no reason to create new segments\n");//last updated 07-31-2023
      return;
      //int secondSegSz;
      if (phase->secondSegCnt > 1) // flipflop pattern
      {
         phase->secondSegSz = phase->thirdSegSz + phase->secondSegSz;
         phase->secondSegCnt--; 
         phase->iniSegSz = lineSz - phase->secondSegSz - phase->thirdSegSz * 2;
         
         phase->lastSegSz = phase->secondSegSz;
         phase->priorTailSegSz = phase->tailSegSz;
         
         findIniSegTypeResetVars(phase);
      }
      else
      {
         // it does not need updated segments
         
         
         /*
         // find the second segment
         printf("lineSz:%i\n", lineSz);
         int segSz = lineSz - phase->iniSegSz; // remove the initial segment
         printf("segSz:%i\n", segSz);
         segSz -= phase->thirdSegSz * phase->thirdSegCnt; // remove the identified tail
         //printf("seg:%i\n", segSz);
         segSz -= phase->secondSegSz * phase->secondSegCnt; // remove the head
         //int alternateCnt = phase->thirdSegCnt - 2;// the number of times secondSegSz and thirdSegSz have been alternating
         
         printf("segSz:%i\n", segSz);
         phase->iniSegSz += phase->thirdSegSz * 2 + phase->secondSegSz;
         
         phase->secondSegSz = segSz;
         
         phase->secondSegCnt = (phase->secondSegCnt - 1);//1;//temp
         phase->lastSegSz = phase->secondSegSz;
         */
         
      }
      
      
      printf("updated iniSegSz:%i\n", phase->iniSegSz);
      printf("updated secondSegSz:%i\n", phase->secondSegSz);
      printf("updated secondSegCnt:%i\n", phase->secondSegCnt);
      printf("initial segment type:%s\n", test_iniSegType[phase->iniSegType]);
      
      return;
   }

   // 1
   if (phase->headFound == FALSE)
   if (phase->thirdSegSz == phase->tailSegSz)
   if (phase->secondSegSz > phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      if (phase->iniSegType == SM_INI_SEG)
      {
         // return if the tail count is not found
         //if (phase->tailSegCnt == 0) // last updated 08-01-2023
         if (phase->secondSegCnt <= 1)
         {
            printf("There is no reason to create new segments\n");
            return;
         }
         
         // last updated 08-01-2023
         phase->priorHeadSz = phase->headSz;
         phase->priorTailSegSz = phase->tailSegSz;
         phase->priorTailSegCnt = 1;
         
         phase->iniSegSz = lineSzPriorBit - (phase->secondSegSz + phase->thirdSegSz + phase->tailSegSz*2);
         //phase->iniSegSz += phase->secondSegSz + phase->thirdSegSz;
         
         phase->secondSegSz = phase->secondSegSz + phase->thirdSegSz;
         phase->secondSegCnt = 1; 
         phase->lastSegSz = phase->secondSegSz;
         
         phase->secondSegFound = TRUE;
         phase->iniSegType = SM_INI_SEG; // NOTE: create function for this?
         
         // reset variables
         phase->thirdSegFound = FALSE;
         phase->thirdSegSz = 0;
         phase->thirdSegCnt = 0;
         
         phase->tailSegFound = FALSE;
         phase->tailSegSz = 0;
         
         phase->headFound = FALSE;
         phase->headSz = 0;
         
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
      else
      {
         printf("tailSegSz:%i tailSegCnt:%i\n", phase->thirdSegSz, phase->thirdSegCnt);
         phase->iniSegSz = phase->thirdSegSz * phase->thirdSegCnt + phase->secondSegSz;
         
         printf("updated phase->iniSegSz:%i\n", phase->iniSegSz);
         
         phase->iniSegType = 0;
         
         phase->secondSegFound = FALSE;
         phase->secondSegSz = 0;
         
         tempFuncResetVars(phase);
      }
      return;
   }
   
   if (phase->headFound)
   if (phase->thirdSegSz == phase->tailSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
         
      // return if the tail count is not found
      //if (phase->tailSegCnt == 0) // last updated 08-01-2023
      if (phase->secondSegCnt <= 1)
      {
         printf("There is no reason to create new segments\n");
         return;
      }
      
         printf("line size:%i\n", lineSzPriorBit);
         phase->iniSegSz += phase->secondSegSz;
      
         phase->secondSegSz = lineSzPriorBit - phase->iniSegSz;
         phase->secondSegCnt = 1; 
         
         phase->lastSegSz = phase->secondSegSz;
         phase->priorTailSegSz = phase->tailSegSz;
         
         findIniSegTypeResetVars(phase);
         
         printf("updated iniSegSz:%i\n", phase->iniSegSz);
         printf("updated secondSegSz:%i\n", phase->secondSegSz);
         printf("updated secondSegCnt:%i\n", phase->secondSegCnt);
         printf("initial segment type:%s\n", test_iniSegType[phase->iniSegType]);
         
         return;
   }
   
   if (phase->headFound)
   if (phase->secondSegSz == phase->tailSegSz)
   {
      // return if the tail count is not found
      //if (phase->tailSegCnt == 0) // last updated 08-01-2023
      if (phase->thirdSegCnt <= 1)
      {
         printf("There is no reason to create new segments\n");
         return;
      }
      
      if (phase->iniSegType == EQ_INI_SEG)
      {printf("%s():%i\n",__FUNCTION__, __LINE__);
         
         if (phase->headSz > phase->tailSegSz)
         {//printf("here\n");
            phase->iniSegSz += phase->secondSegSz * 2 + phase->thirdSegSz;
            
            printf("\nphase->iniSegSz:%i\n", phase->iniSegSz);
            
            phase->priorTailSegSz = phase->tailSegSz;
            
            phase->iniSegType = 0;
            
            phase->secondSegFound = FALSE;
            phase->secondSegSz = 0;
            
            tempFuncResetVars(phase);
         }
         else
         {printf("%s():%i\n",__FUNCTION__, __LINE__);
            
            // last updated 08-01-2023
            phase->priorHeadSz = phase->headSz;
            phase->priorTailSegSz = phase->tailSegSz;
            phase->priorTailSegCnt = 1;
            
            phase->iniSegSz = phase->secondSegSz*2 + phase->thirdSegSz; // NOTE: create function for this?
            
            phase->secondSegSz = phase->secondSegSz + phase->thirdSegSz;
            phase->secondSegCnt = 1; 
            phase->lastSegSz = phase->secondSegSz;
            
            phase->secondSegFound = TRUE;
            phase->iniSegType = LG_INI_SEG; // NOTE: create function for this?
            
            // reset variables
            phase->thirdSegFound = FALSE;
            phase->thirdSegSz = 0;
            phase->thirdSegCnt = 0;
            
            phase->tailSegFound = FALSE;
            phase->tailSegSz = 0;
            
            phase->headFound = FALSE;
            phase->headSz = 0;
            
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
            
            /*
            phase->iniSegSz = phase->secondSegSz * 2 + phase->thirdSegSz;
            
            phase->priorHeadSz = phase->headSz;
            phase->priorTailSegSz = phase->tailSegSz;
            
            phase->secondSegFound = FALSE;
            phase->secondSegSz = 0;
            phase->iniSegType = 0;
            
            tempFuncResetVars(phase);
            
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
            */
         }
      }
      else if (phase->iniSegType == LG_INI_SEG) // last updated 07-31-2023
      {printf("%s():%i\n",__FUNCTION__, __LINE__);
         phase->priorHeadSz = phase->headSz;
         phase->priorTailSegSz = phase->tailSegSz;
         phase->priorTailSegCnt = phase->secondSegCnt;
         
         phase->secondSegSz = lineSz - phase->iniSegSz;
         phase->secondSegCnt = 1; 
         phase->lastSegSz = phase->secondSegSz;
         
         findIniSegTypeResetVars(phase);
         
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
      else if (phase->iniSegType == SM_INI_SEG) // last updated 07-31-2023
      {printf("%s():%i\n",__FUNCTION__, __LINE__);
         
         // return if the tail count is not found
         //if (phase->tailSegCnt == 0) // last updated 08-01-2023
         if (phase->thirdSegCnt <= 1)
         {
            printf("There is no reason to create new segments\n");
            return;
         }
         
         // last updated 08-01-2023
         phase->priorHeadSz = phase->headSz;
         phase->priorTailSegSz = phase->tailSegSz;
         phase->priorTailSegCnt = 1;
         
         phase->iniSegSz += phase->secondSegSz*2 + phase->thirdSegSz; // NOTE: create function for this?
         
         phase->secondSegSz = phase->secondSegSz*phase->tailSegInc + phase->thirdSegSz;
         phase->secondSegCnt = 1; 
         phase->lastSegSz = phase->secondSegSz;
         
         phase->secondSegFound = TRUE;
         phase->iniSegType = EQ_INI_SEG; // NOTE: create function for this?
         
         // reset variables
         phase->thirdSegFound = FALSE;
         phase->thirdSegSz = 0;
         phase->thirdSegCnt = 0;
         
         phase->tailSegFound = FALSE;
         phase->tailSegSz = 0;
         
         phase->headFound = FALSE;
         phase->headSz = 0;
         
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
         
         /*
         phase->priorHeadSz = phase->headSz;
         phase->priorTailSegSz = phase->tailSegSz;
         phase->priorTailSegCnt = phase->secondSegCnt;
         
         phase->secondSegSz = lineSz - phase->iniSegSz;
         phase->secondSegCnt = 1; 
         phase->lastSegSz = phase->secondSegSz;
         
         findIniSegTypeResetVars(phase);
         
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
         */
      }
      
      return;
   }
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
      
      phase->lastSegSz = phase->secondSegSz;
   }
   
   // count the number of segments matching the Third Segment
   if (phase->thirdSegFound)
   if (currSegSz_sm == phase->thirdSegSz)
   {printf("%s():%i\n",__FUNCTION__, __LINE__);
      phase->thirdSegCnt++;
      printf("third segment, count:%i\n", phase->thirdSegCnt);
      
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
   if (phase->thirdSegFound)
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
         
         phase->headFound = TRUE; // updated 08-01-2023
         phase->headSz = phase->secondSegSz;
         printf("head found, size:%i\n", phase->headSz);
         
         phase->tailSegInc = 2; // updated 08-01-2023
         printf("tail increment:%i\n", phase->tailSegInc);
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
      
      if (phase->headFound)
      if (phase->tailSegFound)
      if (phase->tailSegSz == phase->thirdSegSz) // updated 08-01-2023
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
      
      if (phase->iniSegType == EQ_INI_SEG)
      {
         phase->tailSegFound = TRUE;
         phase->tailSegSz = phase->secondSegSz;
         printf("tail segment found, size:%i\n", phase->tailSegSz);
      }
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
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
      
      if (phase->tailSegFound)
      if (phase->lastSegSz == phase->secondSegSz)
      {
         phase->headFound = TRUE;
         phase->headSz = phase->thirdSegSz;
         printf("head found, size:%i\n", phase->headSz);
      }
      
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
   
   return 0;
   
   /*
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
}