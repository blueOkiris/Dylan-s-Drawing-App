#include <stdlib.h>
#include <math.h>
#include <SDL.h>

#include <line.h>

Line line_appendPointToLine(Line line, Point p) {
    Line lineCopy = line;
    if(lineCopy.m_points == NULL) {
        // Add first point
        lineCopy.length = 1;
        lineCopy.m_points = malloc(sizeof(Point));
    } else {
        lineCopy.m_points = realloc(lineCopy.m_points, sizeof(Point) * (lineCopy.length + 1));
        lineCopy.length++;
    }
    
    lineCopy.m_points[lineCopy.length - 1] = p;
    
    return lineCopy;    // It updates the pointer, but since pointers are just numbers, we can pass line by value!
}

Line line_cleanLine(Line line) {
    Line lineCopy = line;
    
    free(lineCopy.m_points);
    
    lineCopy.length = 0;
    lineCopy.m_points = NULL;
    
    return lineCopy;
}

int line_pointInRect(Point p, SDL_Rect rect) {
    return (p.x > rect.x) && (p.y > rect.y) && (p.x < rect.x + rect.w) && (p.y < rect.y + rect.h);
}

char line_isPointOn(Line line, Point p) {
    for(int i = 0; i < line.length - 1; i++) {
        for(int k = -(line.radius + 1); k < line.radius + 1; k++) {
            double slope = ((double) (line.m_points[i].y - line.m_points[i + 1].y)) / (line.m_points[i].x - line.m_points[i + 1].y);
            double offset = (line.m_points[i].y + k) - (line.m_points[i].x * slope);
            //printf("y = %f * x + %f\r\n", slope, offset);
            
            if(slope * p.x + offset == p.y)
                return 1;
        }
    }
    
    return 0;
}

int line_pointDistance(Point a, Point b) {
    int dx = a.x - b.x, dy = a.y - b.y;
    return (int) sqrt((double) (dx * dx + dy * dy));
}
