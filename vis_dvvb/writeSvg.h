#include <stdio.h>

FILE *openSvgFile(char *, int, int, double);
void closeSvgFile(FILE *);
void placeGrid(FILE *, float, float);
void placeBox(FILE *, float, float, char *);
void placeTrunLine(FILE *, int, int, int, int, int, int, char *);
