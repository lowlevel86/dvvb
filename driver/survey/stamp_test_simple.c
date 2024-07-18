#include <stdio.h>

#define TRUE 1
#define FALSE 0

// HORIZONTAL DOWN
int HD_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a][b] pixal locations
   //[c][d]
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   if ((a < c - threshold) || (a > c + threshold)) // different
   if ((b < d - threshold) || (b > d + threshold)) // different
   if (((a >= b - threshold) && (a <= b + threshold)) || // the same
       ((c >= d - threshold) && (c <= d + threshold)))   // the same
   return TRUE;
   
   return FALSE;
}

int HD_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a]    pixal locations
   //[c][b]
   //   [d]
   
   if (y+2 < H)
   {
      y_loc = y * W;
      
      a = (unsigned char)rast[y_loc + x];
      b = (unsigned char)rast[y_loc+W + x+1];
      c = (unsigned char)rast[y_loc+W + x];
      d = (unsigned char)rast[y_loc+W*2 + x+1];
      
      if ((a < c - threshold) || (a > c + threshold)) // different
      if ((b < d - threshold) || (b > d + threshold)) // different
      if (((a >= b - threshold) && (a <= b + threshold)) || // the same
          ((c >= d - threshold) && (c <= d + threshold)))   // the same
      return TRUE;
   }
   
   return FALSE;
}

// HORIZONTAL UP
int HU_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a][b] pixal locations
   //[c][d]
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc + x+1];
   c = (unsigned char)rast[y_loc+W + x];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   if ((a < c - threshold) || (a > c + threshold)) // different
   if ((b < d - threshold) || (b > d + threshold)) // different
   if (((a >= b - threshold) && (a <= b + threshold)) || // the same
       ((c >= d - threshold) && (c <= d + threshold)))   // the same
   return TRUE;
   
   return FALSE;
}

int HU_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //   [b] pixal locations
   //[a][d]
   //[c]
   
   //printf("x:%i y:%i W:%i H:%i\n", x+1, y-1, W, H);
   if (y-1 >= 0)
   {
      y_loc = y * W;
      
      a = (unsigned char)rast[y_loc + x];
      b = (unsigned char)rast[y_loc-W + x+1];
      c = (unsigned char)rast[y_loc+W + x];
      d = (unsigned char)rast[y_loc + x+1];
      
      if ((a < c - threshold) || (a > c + threshold)) // different
      if ((b < d - threshold) || (b > d + threshold)) // different
      if (((a >= b - threshold) && (a <= b + threshold)) || // the same
          ((c >= d - threshold) && (c <= d + threshold)))   // the same
      return TRUE;
   }
   
   return FALSE;
}

// VERTICAL LEFT
int VL_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a][c] pixal locations
   //[b][d]
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   if ((a < c - threshold) || (a > c + threshold)) // different
   if ((b < d - threshold) || (b > d + threshold)) // different
   if (((a >= b - threshold) && (a <= b + threshold)) || // the same
       ((c >= d - threshold) && (c <= d + threshold)))   // the same
   return TRUE;
   
   return FALSE;
}

int VL_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //   [a][c] pixal locations
   //[b][d]
   
   //printf("x:%i y:%i W:%i H:%i\n", x-1, y+1, W, H);
   if (x-1 >= 0)
   {
      y_loc = y * W;
      
      a = (unsigned char)rast[y_loc + x];
      b = (unsigned char)rast[y_loc+W + x-1];
      c = (unsigned char)rast[y_loc + x+1];
      d = (unsigned char)rast[y_loc+W + x];
      
      if ((a < c - threshold) || (a > c + threshold)) // different
      if ((b < d - threshold) || (b > d + threshold)) // different
      if (((a >= b - threshold) && (a <= b + threshold)) || // the same
          ((c >= d - threshold) && (c <= d + threshold)))   // the same
      return TRUE;
   }
   
   return FALSE;
}

// VERTICAL RIGHT
int VR_straight_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a][c] pixal locations
   //[b][d]
   
   y_loc = y * W;
   
   a = (unsigned char)rast[y_loc + x];
   b = (unsigned char)rast[y_loc+W + x];
   c = (unsigned char)rast[y_loc + x+1];
   d = (unsigned char)rast[y_loc+W + x+1];
   
   if ((a < c - threshold) || (a > c + threshold)) // different
   if ((b < d - threshold) || (b > d + threshold)) // different
   if (((a >= b - threshold) && (a <= b + threshold)) || // the same
       ((c >= d - threshold) && (c <= d + threshold)))   // the same
   return TRUE;
   
   return FALSE;
}

int VR_diagonal_exists(char *rast, unsigned int *traceBuff, int W, int H, int x, int y, int threshold)
{
   int y_loc;
   unsigned char a, b, c, d;
   //[a][c]    pixal locations
   //   [b][d]
   
   //printf("x:%i y:%i W:%i H:%i\n", x+2, y+1, W, H);
   if (x+2 < W)
   {
      y_loc = y * W;
      
      a = (unsigned char)rast[y_loc + x];
      b = (unsigned char)rast[y_loc+W + x+1];
      c = (unsigned char)rast[y_loc + x+1];
      d = (unsigned char)rast[y_loc+W + x+2];
      
      if ((a < c - threshold) || (a > c + threshold)) // different
      if ((b < d - threshold) || (b > d + threshold)) // different
      if (((a >= b - threshold) && (a <= b + threshold)) || // the same
          ((c >= d - threshold) && (c <= d + threshold)))   // the same
      return TRUE;
   }
   
   return FALSE;
}
