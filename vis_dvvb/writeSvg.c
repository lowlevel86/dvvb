#include <stdio.h>
#include "svg.h"

static double UNIT;

FILE *openSvgFile(char *filename, int w, int h, double unit)
{
   FILE *filePtr;
   
   UNIT = unit;
   
   // open file for write
   filePtr = fopen(filename,"w");
   if (!filePtr)
   {
      printf("Can not write SVG.\n");
      return filePtr;
   }
   
   // write header
   fprintf(filePtr, svgHead, w*(int)UNIT, h*(int)UNIT, w*(int)UNIT, h*(int)UNIT);
   fprintf(filePtr, svgGroup, 0.0, 0.0, h*UNIT, 1.0, -1.0);
   
   return filePtr;
}

void closeSvgFile(FILE *filePtr)
{
   if (!filePtr)
   {
      printf("Can not write SVG.\n");
      return;
   }
   
   // write footer
   fprintf(filePtr, svgGroupEnd);
   fprintf(filePtr, svgEnd);
   
   fclose(filePtr);
}

void placeGrid(FILE *filePtr, float w, float h)
{
   int i;
   
   if (!filePtr)
   {
      printf("Can not write SVG.\n");
      return;
   }
   
   fprintf(filePtr, svgPolyBgn, 1.0, "FFFFFF", 1.0, 1.0, "FFFFFF");
   fprintf(filePtr, svgPoly, 0.0, 0.0);
   fprintf(filePtr, svgPoly, w*UNIT, 0.0);
   fprintf(filePtr, svgPoly, w*UNIT, h*UNIT);
   fprintf(filePtr, svgPoly, 0.0, h*UNIT);
   fprintf(filePtr, svgPolyEnd);
   
   // make grid
   fprintf(filePtr, svgLineBgn, 1.0, "555555", 0.5);
   
   for (i=1; i < w; i++)
   fprintf(filePtr, svgLine, UNIT*i, h*UNIT, UNIT*i, 0.0);
   
   for (i=1; i < h; i++)
   fprintf(filePtr, svgLine, 0.0, UNIT*i, w*UNIT, UNIT*i);
   
   fprintf(filePtr, svgLineEnd);
}

void placeBox(FILE *filePtr, float x, float y, char *color)
{
   float size;
   
   x = x*UNIT+UNIT/2.0;
   y = y*UNIT+UNIT/2.0;
   size = UNIT/2.0-2;
   
   fprintf(filePtr, svgPolyBgn, 0.0, color, 1.0, 0.3, color);
   fprintf(filePtr, svgPoly, x-size, y+size);
   fprintf(filePtr, svgPoly, x+size, y+size);
   fprintf(filePtr, svgPoly, x+size, y-size);
   fprintf(filePtr, svgPoly, x-size, y-size);
   fprintf(filePtr, svgPolyEnd);
}

void placeTrunLine(FILE *filePtr, int xA, int yA, int xB, int yB, int trunA, int trunB, char *color)
{
   int i, length;
   double unit;
   
   unit = (double)(xB - xA) / (yB - yA);
   length = (yB - yA) - trunB;
   
   for (i=trunA; i < length; i++)
   placeBox(filePtr, (int)(xA+i*unit), yA+i, color);
}
