void toFloatingPointLine(int x, int y, int offset, int length,
                         int xOrig, int yOrig, char xySwap, char xFlip, char yFlip,
                         float *xf1, float *yf1, float *xf2, float *yf2)
{
   float x1, y1, x2, y2;
   float yAdjust;
   float y1Adjust, y2Adjust;
   float xOffset;
   
   if ((x == 1) && (length <= y)) // if straight int line
   {
      x1 = 1.0;
      y1 = 0.5;
      x2 = 1.0;
      y2 = length - 0.5;
   }
   else
   {
      if (!(y % x)) // check if y is a multiple of x
      yAdjust = 0.0;
      else
      yAdjust = 0.5;
      
      // find the x offset
      xOffset = offset * x / y;
      
      x1 = 0.5 - xOffset;
      y1 = yAdjust - offset;
      x2 = 0.5 + (float)x * length / y - xOffset;
      y2 = yAdjust + length - offset;
      
      // account offsets
      y1Adjust = offset + 0.5 - yAdjust;
      y2Adjust = offset - (0.5 + yAdjust);
      x1 = x2 + (y1+y1Adjust - y2) / (y1 - y2) * (x1 - x2);
      y1 += y1Adjust;
      x2 = x1 + (y2+y2Adjust - y1) / (y2 - y1) * (x2 - x1);
      y2 += y2Adjust;
   }
   
   if (xySwap)
   {
      if (xFlip)
      {
         if (yFlip)
         {
            xOrig++;
            yOrig++;
            *xf1 = xOrig - y1;
            *yf1 = yOrig - x1;
            *xf2 = xOrig - y2;
            *yf2 = yOrig - x2;
         }
         else
         {
            xOrig++;
            *xf1 = xOrig - y1;
            *yf1 = yOrig + x1;
            *xf2 = xOrig - y2;
            *yf2 = yOrig + x2;
         }
      }
      else
      {
         if (yFlip)
         {
            yOrig++;
            *xf1 = xOrig + y1;
            *yf1 = yOrig - x1;
            *xf2 = xOrig + y2;
            *yf2 = yOrig - x2;
         }
         else
         {
            *xf1 = xOrig + y1;
            *yf1 = yOrig + x1;
            *xf2 = xOrig + y2;
            *yf2 = yOrig + x2;
         }
      }
   }
   else
   {
      if (xFlip)
      {
         if (yFlip)
         {
            xOrig++;
            yOrig++;
            *xf1 = xOrig - x1;
            *yf1 = yOrig - y1;
            *xf2 = xOrig - x2;
            *yf2 = yOrig - y2;
         }
         else
         {
            xOrig++;
            *xf1 = xOrig - x1;
            *yf1 = yOrig + y1;
            *xf2 = xOrig - x2;
            *yf2 = yOrig + y2;
         }
      }
      else
      {
         if (yFlip)
         {
            yOrig++;
            *xf1 = xOrig + x1;
            *yf1 = yOrig - y1;
            *xf2 = xOrig + x2;
            *yf2 = yOrig - y2;
         }
         else
         {
            *xf1 = xOrig + x1;
            *yf1 = yOrig + y1;
            *xf2 = xOrig + x2;
            *yf2 = yOrig + y2;
         }
      }
   }
}
