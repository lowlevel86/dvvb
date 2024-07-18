#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

// HORIZONTAL DOWN
int HD_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, f, g, h;
   int diffA, diffB, diff, median;
   //[e][f]
   //[a][b] pixal locations
   //[c][d]
   //[g][h]
   
   if (y-1 < 0)
   return FALSE;
   
   if (y+2 >= H)
   return FALSE;
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   e = (unsigned char)rast[y_loc-W + x];
   f = (unsigned char)rast[y_loc-W + x+1];
   g = (unsigned char)rast[y_loc+W*2 + x];
   h = (unsigned char)rast[y_loc+W*2 + x+1];
   
   diffA = abs((a + b) - (g + h)) / 2;
   diffB = abs((e + f) - (c + d)) / 2;
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;
   
   return FALSE;
}

int HD_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char f, g;
   int diffA, diffB, diff, median;
   //[e]<--not used
   //[a][f] pixal locations
   //[c][b]
   //[g][d]
   //   [h]<--not used
   
   if (y+2 >= H)
   return FALSE;
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc+W*2 + x+1];
   
   //e = not used
   f = (unsigned char)rast[y_loc + x+1];
   g = (unsigned char)rast[y_loc+W*2 + x];
   //h = not used
   
   diffA = abs(a - g);
   diffB = abs(f - d);
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;
   
   return FALSE;
}

// HORIZONTAL UP
int HU_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, f, g, h;
   int diffA, diffB, diff, median;
   //[e][f]
   //[a][b] pixal locations
   //[c][d]
   //[g][h]
   
   if (y-1 < 0)
   return FALSE;
   
   if (y+2 >= H)
   return FALSE;
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   e = (unsigned char)rast[y_loc-W + x];
   f = (unsigned char)rast[y_loc-W + x+1];
   g = (unsigned char)rast[y_loc+W*2 + x];
   h = (unsigned char)rast[y_loc+W*2 + x+1];
   
   diffA = abs((a + b) - (g + h)) / 2;
   diffB = abs((e + f) - (c + d)) / 2;
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;
   
   return FALSE;
}

int HU_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, h;
   int diffA, diffB, diff, median;
   //   [f]<--not used
   //[e][b] pixal locations
   //[a][d]
   //[c][h]
   //[g]<--not used
   
   if (y-1 < 0)
   return FALSE;

   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc-W + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc + x+1];
   
   e = (unsigned char)rast[y_loc-W + x];
   //f = not used
   //g = not used
   h = (unsigned char)rast[y_loc+W + x+1];
   
   diffA = abs(e - c);
   diffB = abs(b - h);
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;

   return FALSE;
}

// VERTICAL LEFT
int VL_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, f, g, h;
   int diffA, diffB, diff, median;
   //[e][a][c][g] pixal locations
   //[f][b][d][h]
   
   if (x-1 < 0)
   return FALSE;
   
   if (x+2 >= W)
   return FALSE;
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   e = (unsigned char)rast[y_loc + x-1];
   f = (unsigned char)rast[y_loc+W + x-1];
   g = (unsigned char)rast[y_loc + x+2];
   h = (unsigned char)rast[y_loc+W + x+2];
   
   diffA = abs((a + b) - (g + h)) / 2;
   diffB = abs((e + f) - (c + d)) / 2;
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;
   
   return FALSE;
}

int VL_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, h;
   int diffA, diffB, diff, median;
   //   [e][a][c][g]<--not used
   //[f][b][d][h] pixal locations
   // ^not used
   
   if (x-1 < 0)
   return FALSE;

   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x-1];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x];
   
   e = (unsigned char)rast[y_loc + x-1];
   //f = not used
   //g = not used
   h = (unsigned char)rast[y_loc+W + x+1];
   
   diffA = abs(e - c);
   diffB = abs(b - h);
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;

   return FALSE;
}

// VERTICAL RIGHT
int VR_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char e, f, g, h;
   int diffA, diffB, diff, median;
   //[e][a][c][g] pixal locations
   //[f][b][d][h]
   
   if (x-1 < 0)
   return FALSE;
   
   if (x+2 >= W)
   return FALSE;
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   e = (unsigned char)rast[y_loc + x-1];
   f = (unsigned char)rast[y_loc+W + x-1];
   g = (unsigned char)rast[y_loc + x+2];
   h = (unsigned char)rast[y_loc+W + x+2];
   
   diffA = abs((a + b) - (g + h)) / 2;
   diffB = abs((e + f) - (c + d)) / 2;
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;
   
   return FALSE;
}

int VR_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   unsigned char f, g;
   int diffA, diffB, diff, median;
   //[e][a][c][g] pixal locations
   // | [f][b][d][h]
   // ^not used   ^not used
   
   if (x+2 >= W)
   return FALSE;

   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x+1];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x+2];
   
   //e = not used
   f = (unsigned char)rast[y_loc+W + x];
   g = (unsigned char)rast[y_loc + x+2];
   //h = not used
   
   diffA = abs(a - g);
   diffB = abs(f - d);
   diff = (diffA > diffB) ? diffA : diffB;
   
   if (diff < threshold)
   return FALSE;
   
   median = diff / 2;
   
   if ((a < c - median) || (a > c + median)) // different
   if ((b < d - median) || (b > d + median)) // different
   if (((a >= b - median) && (a <= b + median)) || // the same
       ((c >= d - median) && (c <= d + median)))   // the same
   return TRUE;

   return FALSE;
}
