#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "writeSvg.h"

#define TRUE 1
#define FALSE 0

static void placePixPatGraph(FILE *filePtr, int *headSz, int *tailSz, int *tailCnt, int magnCnt)
{
   #define HEAD 1
   #define TAIL 2
   #define OFFSET 3
   int i, j, k, l;
   int xTailSeg;
   int xLineSz, yLineSz;
   int *xBodySz, *yBodySz;
   int *xHeadSz, *yHeadSz;
   int *xTailSz, *yTailSz;
   int *xOffset, *yOffset;
   char ***pxBodyType;
   int patternFlip;
   
   
   xBodySz = (int *)malloc(magnCnt * sizeof(int));
   yBodySz = (int *)malloc(magnCnt * sizeof(int));
   
   xHeadSz = (int *)malloc(magnCnt * sizeof(int));
   yHeadSz = (int *)malloc(magnCnt * sizeof(int));
   
   xTailSz = (int *)malloc(magnCnt * sizeof(int));
   yTailSz = (int *)malloc(magnCnt * sizeof(int));
   
   xOffset = (int *)malloc(magnCnt * sizeof(int));
   yOffset = (int *)malloc(magnCnt * sizeof(int));
   
   
   xBodySz[0] = tailCnt[0] + 1;
   yBodySz[0] = headSz[0] + tailSz[0] * tailCnt[0];
   
   xHeadSz[0] = (headSz[0] != 0);
   yHeadSz[0] = headSz[0];
   
   xTailSz[0] = xBodySz[0] - xHeadSz[0];
   yTailSz[0] = yBodySz[0] - yHeadSz[0];
   
   if (tailSz[0] > headSz[0])
   {
      xOffset[0] = 0;
      yOffset[0] = 0;
      patternFlip = FALSE;
   }
   else
   {
      xOffset[0] = 1;
      yOffset[0] = headSz[0];
      patternFlip = TRUE;
   }
   
   xTailSeg = 1;
   for (i=1; i < magnCnt; i++)
   {
      if (tailSz[i] > headSz[i])
      {
         xBodySz[i] = xBodySz[i-1] * (tailCnt[i] + 1) - xTailSeg;
         yBodySz[i] = yBodySz[i-1] * (tailCnt[i] + 1) - tailSz[i-1];
      }
      else
      {
         xBodySz[i] = xBodySz[i-1] * (tailCnt[i] + 1) + xTailSeg;
         yBodySz[i] = yBodySz[i-1] * (tailCnt[i] + 1) + tailSz[i-1];
         patternFlip = !patternFlip;
      }
      
      xOffset[i] = 0;
      yOffset[i] = 0;
      
      if (patternFlip) // NOTE: reduce^
      {
         if (tailSz[i] > headSz[i])
         {
            xOffset[i] = xBodySz[i-1] - xTailSeg;
            yOffset[i] = yBodySz[i-1] - tailSz[i-1];
         }
         else
         {
            xOffset[i] = xBodySz[i-1] + xTailSeg;
            yOffset[i] = yBodySz[i-1] + tailSz[i-1];
         }
      }
      
      xHeadSz[i] = xBodySz[i-1] - xTailSeg;
      yHeadSz[i] = headSz[i];
      
      xTailSz[i] = xBodySz[i] - xHeadSz[i];
      yTailSz[i] = yBodySz[i] - yHeadSz[i];
      
      xTailSeg = xBodySz[i-1];
   }
   
   
   for (i=0; i < magnCnt; i++)
   {
      printf("xBodySz[i]:%i yBodySz[i]:%i  ", xBodySz[i], yBodySz[i]);
      printf("xHeadSz[i]:%i yHeadSz[i]:%i  ", xHeadSz[i], yHeadSz[i]);
      printf("xTailSz[i]:%i yTailSz[i]:%i\n", xTailSz[i], yTailSz[i]);
   }
   
   // create buffer for the pixel pattern color data
   pxBodyType = (char ***)malloc((yBodySz[magnCnt-1]*2) * sizeof(char **));
   
   for (i=0; i < yBodySz[magnCnt-1]*2; i++)
   pxBodyType[i] = (char **)malloc(magnCnt * sizeof(char *));
   
   for (i=0; i < yBodySz[magnCnt-1]*2; i++)
   for (j=0; j < magnCnt; j++)
   pxBodyType[i][j] = (char *)malloc(magnCnt);
   
   for (i=0; i < yBodySz[magnCnt-1]*2; i++)
   for (j=0; j < magnCnt; j++)
   pxBodyType[i][j][magnCnt-1] = 0; //set to zero
   
   
   int offset_sum = 0; //offset
   // sum offsets
   for (i=0; i < magnCnt; i++)
   offset_sum += yOffset[i];
   
   printf("offset_sum:%i\n\n", offset_sum);
   
   
   // double line length if there is an offset
   if (offset_sum == 0)
   {
      xLineSz = xBodySz[magnCnt-1];
      yLineSz = yBodySz[magnCnt-1];
   }
   else
   {
      xLineSz = xBodySz[magnCnt-1] * 2;
      yLineSz = yBodySz[magnCnt-1] * 2;
   }
   
   
   // find pixel pattern color data
   
   // ini offset
   for (i=0; i < offset_sum; i++)
   pxBodyType[i][magnCnt-1][magnCnt-1] = OFFSET;
   
   for (i=1; i <= yBodySz[magnCnt-1]-offset_sum; i++)
   pxBodyType[yLineSz-i][magnCnt-1][magnCnt-1] = OFFSET;
   
   
   pxBodyType = &pxBodyType[offset_sum];
   

   int head_found;
   // find pixel pattern color data
   for (l=0; l < magnCnt; l++)
   {
      // ini tail and head
      for (i=0; i < yTailSz[l]; i++)
      pxBodyType[i][l][l] = TAIL;
      
      for (i=yTailSz[l]; i < yBodySz[l]; i++)
      pxBodyType[i][l][l] = HEAD;
      
      head_found = FALSE;
      for (k=l+1; k < magnCnt; k++)
      {
         // find next tail
         for (j=0; j < tailCnt[k]; j++)
         for (i=0; i < yBodySz[k-1]; i++)
         pxBodyType[yBodySz[k-1]*j+i][l][k] = pxBodyType[i][l][k-1];
         
         // find next head
         if ((l < magnCnt-1) && (head_found == FALSE))
         //if (l < magnCnt-1)
         {
            if (tailSz[l+1] > headSz[l+1])
            {
               for (i=0; i < yHeadSz[k]; i++)
               pxBodyType[yTailSz[k]+i][l][k] = pxBodyType[i+tailSz[k-1]][l][k-1];
            }
            else
            {
               // draw larger head section
               for (i=0; i < yTailSz[k-1]; i++)
               pxBodyType[yTailSz[k]+i][l][k] = pxBodyType[i][l][k-1];
               
               for (i=0; i < yHeadSz[k]; i++)
               pxBodyType[yTailSz[k]+tailSz[k-1]*tailCnt[k-1]+i][l][k] = pxBodyType[i+tailSz[k-1]][l][k-1];
            }
            
            head_found = TRUE;
         }
      }
   }
   
   pxBodyType = &pxBodyType[-offset_sum];
   
   // draw graphics
   placeGrid(filePtr, xLineSz, yLineSz);
   placeTrunLine(filePtr, 0, 0, xLineSz, yLineSz, 0, 0, "000000");
   
   for (i=0; i < yLineSz; i++)
   for (j=magnCnt-1; j >= 0; j--) //for (j=0; j < magnCnt; j++)
   if (pxBodyType[i][j][magnCnt-1] == HEAD)
   placeTrunLine(filePtr, 0, 0, xLineSz, yLineSz, i, yLineSz-(i+1), "FF0000");
   else if (pxBodyType[i][j][magnCnt-1] == TAIL)
   placeTrunLine(filePtr, 0, 0, xLineSz, yLineSz, i, yLineSz-(i+1), "0000FF");
   else if (pxBodyType[i][j][magnCnt-1] == OFFSET)
   placeTrunLine(filePtr, 0, 0, xLineSz, yLineSz, i, yLineSz-(i+1), "00FF00");
   
   // free buffers
   for (i=0; i < yBodySz[magnCnt-1]; i++)
   for (j=0; j < magnCnt; j++)
   free(pxBodyType[i][j]);
   
   for (i=0; i < yBodySz[magnCnt-1]; i++)
   free(pxBodyType[i]);
   
   free(pxBodyType);
   
   free(xBodySz);
   free(yBodySz);
   free(xHeadSz);
   free(yHeadSz);
   free(xTailSz);
   free(yTailSz);
   free(xOffset);
   free(yOffset);
}


void drawPixPatSvg(char *filename, int *headSz, int *tailSz, int *tailCnt, int magnCnt)
{
   #define HEIGHT 2000
   int i;
   FILE *filePtr;
   int x, y, xPrior, yPrior;
   int xTailSeg;
   int unit_size;
   int offset;
   
   
   x = tailCnt[0] + 1;
   y = headSz[0] + tailSz[0] * tailCnt[0];
   
   if (tailSz[0] > headSz[0])
   offset = FALSE;
   else
   offset = TRUE;
   
   xTailSeg = 1;
   xPrior = x;
   yPrior = y;
   for (i=1; i < magnCnt; i++)
   {
      if (tailSz[i] > headSz[i])
      {
         x = xPrior * (tailCnt[i] + 1) - xTailSeg;
         y = yPrior * (tailCnt[i] + 1) - tailSz[i-1];
      }
      else
      {
         x = xPrior * (tailCnt[i] + 1) + xTailSeg;
         y = yPrior * (tailCnt[i] + 1) + tailSz[i-1];
         offset = TRUE;
      }
      
      xTailSeg = xPrior;
      xPrior = x;
      yPrior = y;
   }
   
   
   // double length for offset
   if (offset)
   {
      x *= 2; 
      y *= 2;
   }
   
   printf("size x:%i  y:%i\n", x, y);
   
   unit_size = round((float)HEIGHT / y);
   
   filePtr = openSvgFile(filename, x, y, unit_size);
   
   placePixPatGraph(filePtr, headSz, tailSz, tailCnt, magnCnt);
   
   closeSvgFile(filePtr);
}