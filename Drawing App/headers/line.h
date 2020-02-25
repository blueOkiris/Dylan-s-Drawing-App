#pragma once

#include <SDL.h>                                    // For types

typedef struct Point {
    unsigned short x;
    unsigned short y;
} Point;

typedef struct Line {
    char radius;
    
    Point *m_points;
    int length;
} Line;

Line    line_appendPointToLine(Line line, Point p); // use malloc/realloc to add points to the line
Line    line_cleanLine(Line line);                  // free the point memory
char    line_isPointOn(Line line, Point p);         // Check if a (clicked) point is inside of the line when thickness is taken into account
int     line_pointDistance(Point a, Point b);       // Distance between two points
int     line_pointInRect(Point p, SDL_Rect rect);
