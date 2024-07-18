#include <stdio.h>
#include "compose.h"
#include "writeSvg.h" // for debugging

/*
static void printBin(int n) // debugging
{
   int i;
   
   for (i = sizeof(int) * 8 - 1; i >= 0; i--)
   printf("%d", (n >> i) & 1);
}

static void drawPxX(int x, int y, float offset, int color, FILE *svg_fp)
{
   float fLines[8];

   fLines[0] = x+offset;fLines[1] = y+offset;
   fLines[2] = x+1+offset;fLines[3] = y+1+offset;
   fLines[4] = x+1+offset;fLines[5] = y+offset;
   fLines[6] = x+offset;fLines[7] = y+1+offset;
   svgLines(fLines, 8, 0.1, color, svg_fp);
}*/

static void drawRightPxEdge(int x, int y, float offset, int color, FILE *svg_fp)
{
   float fLines[4];

   fLines[0] = x+1+offset;fLines[1] = y;
   fLines[2] = x+1+offset;fLines[3] = y+1;
   svgLines(fLines, 4, 0.1, color, svg_fp);
}

static void drawBottomPxEdge(int x, int y, float offset, int color, FILE *svg_fp)
{
   float fLines[4];

   fLines[0] = x;fLines[1] = y+1+offset;
   fLines[2] = x+1;fLines[3] = y+1+offset;
   svgLines(fLines, 4, 0.1, color, svg_fp);
}

static unsigned int traceInfo(unsigned int *trace, int ptr, int size)
{
   if ((ptr < 0) || (ptr >= size))
   return 0;
   
   if (!trace[ptr])
   return 0;
   
   if (trace[ptr]&using_ptr) // get trace info from trace beginning
   return trace[trace[ptr]&ptr_bits];
   else
   return trace[ptr];
}

static void traceHD(struct survey_data *sd, int ptr, int width, int sz, FILE *svg_fp)
{
   while (!(sd->HD[ptr]&trace_flag))
   {
      // HORIZONTAL DOWN DIAGONALS
      if (sd->HD[ptr]&trace_diagonal)
      if (traceInfo(sd->HD, ptr, sz)&patt_0_exists) // don't allow diagonals with no zeros
      {
         int x = ptr%width, y = ptr/width;
         drawBottomPxEdge(x, y, 0.1, 0xFF0000, svg_fp);
         drawRightPxEdge(x, y+1, 0.1, 0xFF0000, svg_fp);
         drawBottomPxEdge(x+1, y+1, 0.125, 0xFF0000, svg_fp);
         
         sd->HD[ptr] |= trace_flag;
         ptr += width+1;
         continue;
      }
      
      // HORIZONTAL STRAIGHTS
      if (sd->HD[ptr]&trace_straight)
      if (!(traceInfo(sd->HU, ptr, sz)&patt_1_exists))
      {
         int x = ptr%width, y = ptr/width;
         drawBottomPxEdge(x, y, 0.1, 0xFF0000, svg_fp);
         drawBottomPxEdge(x+1, y, 0.1, 0xFF0000, svg_fp);
         
         sd->HD[ptr] |= trace_flag;
         ptr++;
         continue;
      }
      
      break;
   }
}

static void traceHU(struct survey_data *sd, int ptr, int width, int sz, FILE *svg_fp)
{
   while (!(sd->HU[ptr]&trace_flag))
   {
      // HORIZONTAL UP DIAGONALS
      if (sd->HU[ptr]&trace_diagonal)
      if (!(traceInfo(sd->VL, ptr+width-1, sz)&patt_0_exists))
      if (!(traceInfo(sd->VL, ptr-width+1, sz)&patt_0_exists))
      {
         int x = ptr%width, y = ptr/width;
         drawBottomPxEdge(x, y, -0.2, 0xFF, svg_fp);
         drawRightPxEdge(x, y, -0.2, 0xFF, svg_fp);
         drawBottomPxEdge(x+1, y-1, -0.225, 0xFF, svg_fp);
         
         sd->HU[ptr] |= trace_flag;
         ptr -= width-1;
         continue;
      }
      
      // HORIZONTAL STRAIGHTS
      if (sd->HU[ptr]&trace_straight)
      if (traceInfo(sd->HU, ptr, sz)&patt_1_exists)
      {
         int x = ptr%width, y = ptr/width;
         drawBottomPxEdge(x, y, -0.2, 0xFF, svg_fp);
         drawBottomPxEdge(x+1, y, -0.2, 0xFF, svg_fp);
         
         sd->HU[ptr] |= trace_flag;
         ptr++;
         continue;
      }
      
      break;
   }
}

static void traceVL(struct survey_data *sd, int ptr, int width, int sz, FILE *svg_fp)
{
   while (!(sd->VL[ptr]&trace_flag))
   {
      // VERTICAL LEFT DIAGONALS
      if (sd->VL[ptr]&trace_diagonal)
      if (traceInfo(sd->VL, ptr, sz)&patt_0_exists) // don't allow diagonals with no zeros
      {
         int x = ptr%width, y = ptr/width;
         drawRightPxEdge(x, y, -0.1, 0xAA00, svg_fp);
         drawBottomPxEdge(x, y, -0.1, 0xAA00, svg_fp);
         drawRightPxEdge(x-1, y+1, -0.125, 0xAA00, svg_fp);
         
         sd->VL[ptr] |= trace_flag;
         ptr += width-1;
         continue;
      }
      
      // VERTICAL STRAIGHTS
      if (sd->VL[ptr]&trace_straight)
      if (!(traceInfo(sd->VR, ptr, sz)&patt_1_exists))
      {
         int x = ptr%width, y = ptr/width;
         drawRightPxEdge(x, y, -0.1, 0xAA00, svg_fp);
         drawRightPxEdge(x, y+1, -0.1, 0xAA00, svg_fp);
         
         sd->VL[ptr] |= trace_flag;
         ptr += width;
         continue;
      }
      
      break;
   }
}

static void traceVR(struct survey_data *sd, int ptr, int width, int sz, FILE *svg_fp)
{
   while (!(sd->VR[ptr]&trace_flag))
   {
      // VERTICAL RIGHT DIAGONALS
      if (sd->VR[ptr]&trace_diagonal)
      if (!(traceInfo(sd->HD, ptr+1, sz)&patt_0_exists))
      if (!(traceInfo(sd->HD, ptr-width, sz)&patt_0_exists))
      {
         int x = ptr%width, y = ptr/width;
         drawRightPxEdge(x, y, 0.2, 0xFFFF, svg_fp);
         drawBottomPxEdge(x+1, y, 0.2, 0xFFFF, svg_fp);
         drawRightPxEdge(x+1, y+1, 0.25, 0xFFFF, svg_fp);
         
         sd->VR[ptr] |= trace_flag;
         ptr += width+1;
         continue;
      }
      
      // VERTICAL STRAIGHTS
      if (sd->VR[ptr]&trace_straight)
      if (traceInfo(sd->VR, ptr, sz)&patt_1_exists)
      {
         int x = ptr%width, y = ptr/width;
         drawRightPxEdge(x, y, 0.2, 0xFFFF, svg_fp);
         drawRightPxEdge(x, y+1, 0.2, 0xFFFF, svg_fp);
         
         sd->VR[ptr] |= trace_flag;
         ptr += width;
         continue;
      }
      
      break;
   }
}

void compose(struct survey_data *sd, int width, int height, FILE *svg_fp)
{
   int x, y, ptr;
   int sz = width * height;

   for (x=0; x < width-1; x++)
   for (y=0; y < height-1; y++)
   {
      ptr = y*width + x;
      
      if (sd->HD[ptr])
      traceHD(sd, ptr, width, sz, svg_fp);
   }
   
   for (x=0; x < width-1; x++)
   for (y=height-2; y >= 0; y--)
   {
      ptr = y*width + x;
      
      if (sd->HU[ptr])
      traceHU(sd, ptr, width, sz, svg_fp);
   }
   
   for (y=0; y < height-1; y++)
   for (x=width-2; x >= 0; x--)
   {
      ptr = y*width + x;
      
      if (sd->VL[ptr])
      traceVL(sd, ptr, width, sz, svg_fp);
   }
   
   for (y=0; y < height-1; y++)
   for (x=0; x < width-1; x++)
   {
      ptr = y*width + x;
      
      if (sd->VR[ptr])
      traceVR(sd, ptr, width, sz, svg_fp);
   }
}
