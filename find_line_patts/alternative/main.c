#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rastToVec.h"

#define TRUE 1
#define FALSE 0

#define PATT_SIZE 32

int composeNewPatt(char *pattA, char *pattB, char *patt)
{
   int i;
   int zeroExists = FALSE;
   
   for (i=0; i < PATT_SIZE; i++)
   {
      if (pattA[i]-'0' || pattB[i]-'0')
      patt[i] = '1';
      else
      patt[i] = '0';
      
      if (patt[i] == '0')
      zeroExists = TRUE;
   }
   
   if (zeroExists)
   return FALSE;
   
   return TRUE;
}

int isLine(char *patt)
{
   int i;
   char pattBuff[PATT_SIZE];
   
   struct pattern_variables *pv;
   pv = iniPattVars(pattBuff);
   
   for (i=0; i < PATT_SIZE; i++)
   if (checkLinePatt(patt[i]-'0', pv))
   return FALSE;
   
   return TRUE;
}

void appendLinePattToFile(char *patt, int pattSz)
{
   FILE *fp = fopen("line_patts.txt", "a");if (fp == NULL) {printf("Error opening file.\n");exit(0);}
   fwrite(patt, pattSz, 1, fp);
   fclose(fp);
}

void getLinePattFromFile(int loc, char *patt, int pattSz)
{
   FILE *fp = fopen("line_patts.txt", "r");if (fp == NULL) {printf("Error opening file.\n");exit(0);}
   fseek(fp, pattSz*loc, SEEK_SET);
   fread(patt, 1, pattSz, fp);
   fclose(fp);
}

int existsInFile(char *patt, int pattSz)
{
   int i;
   int patt_exits = FALSE;
   char pattF[PATT_SIZE+1];
   
   FILE *fp = fopen("line_patts.txt", "r");if (fp == NULL) {printf("Error opening file.\n");exit(0);}

   while (fread(pattF, 1, pattSz, fp) > 0)
   {
      for (i=0; i < pattSz; i++)
      {
         if (patt[i] != pattF[i])
         break;
         
         if (patt[i] == '\n')
         patt_exits = TRUE;
      }
      
      if (patt_exits)
      {
         fclose(fp);
         return TRUE;
      }
   }
   
   fclose(fp);
   return FALSE;
}

int main(void)
{
   int i;
   char patt[PATT_SIZE+1];
   int pattStrSz = PATT_SIZE+1;
   int lineCnt = 0;
   
   int x, y;
   char pattA[PATT_SIZE+1];
   char pattB[PATT_SIZE+1];
   int lineBlockLocA = 0;
   int lineBlockLocB = 1;
   int lineBlockEndB = PATT_SIZE+1;
   int exitMainLoop = FALSE;
   
   // create initial patterns
   for (i=0; i < PATT_SIZE; i++)
   patt[i] = '0';
   
   patt[PATT_SIZE] = '\n';
   appendLinePattToFile(patt, pattStrSz);
   lineCnt++;
   
   for (i=0; i < PATT_SIZE; i++)
   {
      patt[i] = '1';
      appendLinePattToFile(patt, pattStrSz);
      lineCnt++;
      patt[i] = '0';
   }
   
   // create patterns
   while (exitMainLoop == FALSE)
   {
      for (x=lineBlockLocA; x < lineBlockEndB; x++)
      {
         getLinePattFromFile(x, pattA, pattStrSz);
         
         for (y=lineBlockLocB; y < lineBlockEndB; y++)
         {
            getLinePattFromFile(y, pattB, pattStrSz);
            
            if (composeNewPatt(pattA, pattB, patt))
            exitMainLoop = TRUE;
            
            if (isLine(patt))
            if (!existsInFile(patt, pattStrSz)) // NOTE: add seek to skip unnecessary checks
            {
               appendLinePattToFile(patt, pattStrSz);
               lineCnt++;
               printf(">%i lines\n", lineCnt);
            }
         }
      }
      
      lineBlockLocA = lineBlockLocB;
      lineBlockLocB = lineBlockEndB;
      lineBlockEndB = lineCnt;
   }
}
