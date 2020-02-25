#pragma once

#include <SDL.h>        // For types
#include <line.h>       // For types

typedef enum TOOL {
    DRAW,
    ERASE
} TOOL;

typedef struct AppState {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Event       event;
    
    int             keyEsc;
    int             mouseLeft;
    Point           mousePosition;
    int             clicked;
    
    SDL_Texture     *drawIcon, *eraseIcon, *saveIcon, *newIcon, *selectedBorder, *closeIcon,
                    *titleTexture, *size1Icon, *size2Icon, *size3Icon, *size4Icon;
    
    TOOL            selectedTool;
    
    Line            *lines;
    int             lineCount;
    Line            currentLine;
    
    int             done;
    int             error;
} AppState;

AppState    draw_drawLoop(AppState state);
AppState    draw_updateKeys(AppState state);
AppState    draw_updateMouse(AppState state);
AppState    draw_quitEvent(AppState state);
void        draw_drawState(AppState state);
AppState    draw_changeTool(AppState state);
AppState    draw_draw(AppState state);

AppState    draw_addLine(AppState state, Line line);
AppState    draw_eraseLine(AppState state, Line line);

SDL_Texture *draw_loadTexture(AppState state, char *path);
SDL_Texture *draw_textTexture(AppState state, char *fontPath, int size, char *text);
