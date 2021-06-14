//format strings for writing SVGs

// example: printf(svgHead, 640, 480, 640, 480);
char svgHead[]={"<svg width=\"%i\" height=\"%i\" viewBox=\"0 0 %i %i\" "
"xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"};

// example: printf(svgGroup, 0.0, 0.0, 480.0, 1.0, -1.0);
char svgGroup[]={"<g transform=\"rotate(%f) translate(%f %f) scale(%f %f)\">\n"};

// example: printf(svgGroupMatrix, 1.0, 0.0, 0.0, -1.0, 0.0, 200.0);
char svgGroupMatrix[]={"<g transform=\"matrix(%f %f %f %f %f %f)\">\n"};


// example: printf(svgLineBgn, 1.0, "00FF00", 3.5);
char svgLineBgn[]={"<path style=\"opacity:%.3f;stroke:#%s;stroke-width:%.3fpx;fill:none\" d=\""};

// example: printf(svgLine, -120.0, -200.0, 180.0, 60.0);
char svgLine[]={"M%f %fL%f %f"};

char svgLineEnd[]={"\"/>\n"};


// example: printf(svgPolyBgn, 1.0, "00FF00", 3.5, 3.0, 1.0, "888888");
char svgPolyBgn[]={"<path style=\"stroke-opacity:%.3f;stroke:#%s;stroke-width:%.3fpx;fill-opacity:%.3f;fill:#%s\" d=\"M"};

// example: printf(svgPoly, 50.0, 150.0);
// example: printf(svgPoly, 180.0, 60.0);
// example: printf(svgPoly, 80.0, 40.0);
char svgPoly[]={"%f %f "};

char svgPolyEnd[]={"Z\"/>\n"};


// example: printf(svgEllipse, 1.0, "FF0000", 3.0, 1.0, "888888", 77.4, 130.6, 41.0, 43.0);
char svgEllipse[]={"<ellipse style=\"stroke-opacity:%.3f;stroke:#%s;"
"stroke-width:%.3f;fill-opacity:%.3f;fill:#%s\" cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" />\n"};


char svgGroupEnd[]={"</g>\n"};
char svgEnd[]={"</svg>\n"};
