#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <time.h>

#include <draw.h>
#include <line.h>

AppState draw_quitEvent(AppState state) {
    if(SDL_PollEvent(&state.event) != 0) {
        if(state.event.type == SDL_QUIT)
            return
                (AppState) { 
                    state.window, state.renderer, 
                    state.event, 
                    state.keyEsc,
                    state.mouseLeft, state.mousePosition, state.clicked,
                    state.drawIcon, state.eraseIcon, state.saveIcon, state.newIcon, state.selectedBorder, state.titleTexture, state.closeIcon,
                    state.size1Icon, state.size2Icon, state.size3Icon, state.size4Icon,
                    state.selectedTool,
                    state.lines, state.lineCount, state.currentLine,
                    1, state.error };
        else
            return draw_quitEvent(state);
    } else if(state.keyEsc)
        return 
            (AppState) { 
                state.window, state.renderer, 
                state.event, 
                state.keyEsc, 
                state.mouseLeft, state.mousePosition, state.clicked,
                state.drawIcon, state.eraseIcon, state.saveIcon, state.newIcon, state.selectedBorder, state.titleTexture, state.closeIcon,
                state.size1Icon, state.size2Icon, state.size3Icon, state.size4Icon,
                state.selectedTool,
                state.lines, state.lineCount, state.currentLine,
                1, state.error };
    else
        return state;
}

AppState draw_updateMouse(AppState state) {
    int x, y;
    uint32_t mouseState = SDL_GetMouseState(&x, &y);
    char mouseDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) ? 1 : 0;
    //printf("Mouse Pos: (%d, %d)\r\n", x, y);
        
    return
        (AppState) { 
            state.window, state.renderer, state.event,
            state.keyEsc,
            mouseDown, { (unsigned short) x, (unsigned short) y }, !mouseDown ? 0 : state.clicked,
            state.drawIcon, state.eraseIcon, state.saveIcon, state.newIcon, state.selectedBorder, state.titleTexture, state.closeIcon,
            state.size1Icon, state.size2Icon, state.size3Icon, state.size4Icon,
            state.selectedTool,
            state.lines, state.lineCount, state.currentLine,
            state.done, state.error };
}

AppState draw_updateKeys(AppState state) {
    const uint8_t *currentKeyStates = SDL_GetKeyboardState(NULL);
        
    return
        (AppState) { 
            state.window, state.renderer, state.event,
            currentKeyStates[SDL_SCANCODE_ESCAPE] != 0,
            state.mouseLeft, state.mousePosition, state.clicked,
            state.drawIcon, state.eraseIcon, state.saveIcon, state.newIcon, state.selectedBorder, state.titleTexture, state.closeIcon,
            state.size1Icon, state.size2Icon, state.size3Icon, state.size4Icon,
            state.selectedTool,
            state.lines, state.lineCount, state.currentLine,
            state.done, state.error };
}

void draw_drawState(AppState state) {
    // Get size of window
    int w, h;
    SDL_GetRendererOutputSize(state.renderer, &w, &h);
    
    // Draw the tool bar rect
    const SDL_Rect toolBar = (SDL_Rect) { 0, 0, w, 48 };
    SDL_SetRenderDrawColor(state.renderer, 0xA5, 0xA9, 0xB0, 0xFF);
    SDL_RenderFillRect(state.renderer, &toolBar);
    
    // Draw tool bar icons
    const SDL_Rect drawIconRect = (SDL_Rect) { 4, 4, 40, 40 };
    const SDL_Rect eraseIconRect = (SDL_Rect) { 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect saveIconRect = (SDL_Rect) { 4 + 40 + 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect newIconRect = (SDL_Rect) { 4 + 40 + 4 + 40 + 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect closeIconRect = (SDL_Rect) { w - 40 - 4, 4, 40, 40 };
    SDL_RenderCopy(state.renderer, state.drawIcon, NULL, &drawIconRect);
    SDL_RenderCopy(state.renderer, state.eraseIcon, NULL, &eraseIconRect);
    SDL_RenderCopy(state.renderer, state.saveIcon, NULL, &saveIconRect);
    SDL_RenderCopy(state.renderer, state.newIcon, NULL, &newIconRect);
    SDL_RenderCopy(state.renderer, state.closeIcon, NULL, &closeIconRect);
    
    // Draw size options
    const SDL_Rect sizeOpt1Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt2Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt3Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt4Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4, 4, 40, 40 };
    SDL_RenderCopy(state.renderer, state.size1Icon, NULL, &sizeOpt1Rect);
    SDL_RenderCopy(state.renderer, state.size2Icon, NULL, &sizeOpt2Rect);
    SDL_RenderCopy(state.renderer, state.size3Icon, NULL, &sizeOpt3Rect);
    SDL_RenderCopy(state.renderer, state.size4Icon, NULL, &sizeOpt4Rect);
    
    // Draw border around selected tool
    switch(state.selectedTool) {
        case DRAW:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &drawIconRect);
            break;
        
        case ERASE:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &eraseIconRect);
            break;
        
        default:
            // DRAW
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &drawIconRect);
            break;
    }
    
    // Draw border around selected size
    switch(state.currentLine.radius) {
        case 0:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &sizeOpt1Rect);
            break;
            
        case 1:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &sizeOpt2Rect);
            break;
            
        case 2:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &sizeOpt3Rect);
            break;
            
        case 3:
        default:
            SDL_RenderCopy(state.renderer, state.selectedBorder, NULL, &sizeOpt4Rect);
            break;
    }
    
    // Draw title 
    const SDL_Rect titleRect = (SDL_Rect) { w / 2 - 100, 10, 200, 28 };
    SDL_RenderCopy(state.renderer, state.titleTexture, NULL, &titleRect);
    
    // Draw current line
    SDL_SetRenderDrawColor(state.renderer, 0x00, 0x00, 0x00, 0xFF);
    if(state.currentLine.length > 1) {
        for(int i = 0; i < state.currentLine.length - 1; i++) {
            for(int k = -(state.currentLine.radius + 1); k <= (state.currentLine.radius + 1); k++) {
                SDL_RenderDrawLine(state.renderer,
                    state.currentLine.m_points[i].x, state.currentLine.m_points[i].y + k,
                        state.currentLine.m_points[i + 1].x, state.currentLine.m_points[i + 1].y + k);
            }
        }
    }
    
    // Draw other lines
    for(int j = 0; j < state.lineCount; j++) {
        if(state.lines[j].length > 1) {
            for(int i = 0; i < state.lines[j].length - 1; i++) {
                for(int k = -(state.lines[j].radius + 1); k <= (state.lines[j].radius + 1); k++) {
                    SDL_RenderDrawLine(state.renderer,
                        state.lines[j].m_points[i].x, state.lines[j].m_points[i].y + k,
                            state.lines[j].m_points[i + 1].x, state.lines[j].m_points[i + 1].y + k);
                }
            }
        }
    }
}

AppState draw_changeTool(AppState state) {
    // Get size of window
    int w, h;
    SDL_GetRendererOutputSize(state.renderer, &w, &h);
    
    const SDL_Rect drawIconRect = (SDL_Rect) { 4, 4, 40, 40 };
    const SDL_Rect eraseIconRect = (SDL_Rect) { 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect saveIconRect = (SDL_Rect) { 4 + 40 + 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect newIconRect = (SDL_Rect) { 4 + 40 + 4 + 40 + 4 + 40 + 4, 4, 40, 40 };
    const SDL_Rect sizeOpt1Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt2Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt3Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect sizeOpt4Rect = (SDL_Rect) { w - 40 - 8 - 40 - 4 - 40 - 4, 4, 40, 40 };
    const SDL_Rect closeIconRect = (SDL_Rect) { w - 40 - 4, 4, 40, 40 };
    
    AppState stateCopy = state;
    
    if(stateCopy.mouseLeft && !stateCopy.clicked) { // Just clicked down
        //printf("Clicked!\r\n");
        //printf("Mouse position: (%d, %d)\r\n", stateCopy.mousePosition.x, stateCopy.mousePosition.y);
        if(line_pointInRect(stateCopy.mousePosition, drawIconRect)) {
            //printf("Draw\r\n");
            stateCopy.selectedTool = DRAW;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, eraseIconRect)) {
            //printf("Erase\r\n");
            stateCopy.selectedTool = ERASE;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, saveIconRect)) {
            // Get the current date in DD/MM/YY - II:MM:SS.png
            time_t currentTime;
            struct tm *timeInfo;
            char timeString[50];
            time(&currentTime);
            timeInfo = localtime(&currentTime);
            strftime(timeString, sizeof(timeString), "/home/pi/Desktop/notes/%m|%d|%Y-%r.bmp", timeInfo);
            printf("Saving note as file: %s\r\n", timeString);
            
            SDL_Surface *screenSurface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
            SDL_Rect ignoreTopBar = { 0, 48, w, h };
            SDL_RenderReadPixels(stateCopy.renderer, &ignoreTopBar, SDL_PIXELFORMAT_ARGB8888, screenSurface->pixels, screenSurface->pitch);
            
            if(SDL_SaveBMP(screenSurface, timeString) < 0) {
                stateCopy.done = 1;
                stateCopy.error = fprintf(stderr, "Failed to save '%s'! SDL Error: %s\r\n", timeString, SDL_GetError());
                SDL_FreeSurface(screenSurface);
                return stateCopy;
            }
            
            SDL_FreeSurface(screenSurface);
            stateCopy.clicked = 1;
        } else if(line_pointInRect(stateCopy.mousePosition, newIconRect)) {
            while(stateCopy.lineCount > 0) {
                stateCopy = draw_eraseLine(stateCopy, stateCopy.lines[stateCopy.lineCount - 1]);
                stateCopy.lineCount--;
            }
            stateCopy.clicked = 1;
        } else if(line_pointInRect(stateCopy.mousePosition, sizeOpt1Rect)) {
            stateCopy.currentLine.radius = 0;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, sizeOpt2Rect)) {
            stateCopy.currentLine.radius = 1;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, sizeOpt3Rect)) {
            stateCopy.currentLine.radius = 2;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, sizeOpt4Rect)) {
            stateCopy.currentLine.radius = 3;
            stateCopy.clicked = 1;
            return stateCopy;
        } else if(line_pointInRect(stateCopy.mousePosition, closeIconRect)) {
            stateCopy.done = 1;
            return stateCopy;
        }
    }
    
    return stateCopy;
}

AppState draw_draw(AppState state) {    
    // Get size of window
    int w, h;
    SDL_GetRendererOutputSize(state.renderer, &w, &h);
    
    AppState stateCopy = state;
    
    if(stateCopy.selectedTool == DRAW) {
        if(stateCopy.mouseLeft && line_pointInRect(stateCopy.mousePosition, (SDL_Rect) { 0, 48, w, h - 48 })) {
            // Add points to current line
            if(stateCopy.currentLine.m_points == NULL ||
                (stateCopy.currentLine.m_points[stateCopy.currentLine.length - 1].x != stateCopy.mousePosition.x
                    || stateCopy.currentLine.m_points[stateCopy.currentLine.length - 1].y != stateCopy.mousePosition.y))
                stateCopy.currentLine = line_appendPointToLine(stateCopy.currentLine, stateCopy.mousePosition);
        } else if(stateCopy.currentLine.m_points != NULL) {
            // Finished line. Add to lines
            stateCopy = draw_addLine(stateCopy, stateCopy.currentLine);
            
            stateCopy.currentLine.m_points = NULL;
            stateCopy.currentLine.length = 0;
        }
    } else if(stateCopy.selectedTool == ERASE && stateCopy.mouseLeft) {
        for(int i = 0; i < stateCopy.lineCount; i++) {
            if(line_isPointOn(stateCopy.lines[i], stateCopy.mousePosition))
                stateCopy = draw_eraseLine(stateCopy, stateCopy.lines[i]);
        }
    }
    
    return stateCopy;
}

AppState draw_drawLoop(AppState state) {
    if(state.done)
        return state;
    else {
        SDL_SetRenderDrawColor(state.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(state.renderer);        
        draw_drawState(state);
        SDL_RenderPresent(state.renderer);
        
        state = draw_draw(
                    draw_changeTool(
                        draw_quitEvent(
                            draw_updateMouse(
                                draw_updateKeys(state)))));
        
        return draw_drawLoop(state);
    }
}

SDL_Texture *draw_textTexture(AppState state, char *fontPath, int size, char *text) {
    // The final Texture
    SDL_Texture *newTexture = NULL;
    
    // Load image at specified path
    SDL_Surface *loadedSurface = NULL;
    
    TTF_Font *font = TTF_OpenFont(fontPath, size);
    
    if(font == NULL)
        fprintf(stderr, "Unable to load font %s! SDL_ttf Error: %s\r\n", fontPath, TTF_GetError());
    else {
        loadedSurface = TTF_RenderText_Solid(font, text, (SDL_Color) { 0, 0, 0 });
                    
        if(loadedSurface == NULL)
            fprintf(stderr, "Unable to create text %s! SDL_ttf Error: %s\r\n", text, TTF_GetError());
        else {
            // Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(state.renderer, loadedSurface);
            
            if(newTexture == NULL)
                fprintf(stderr, "Unable to create texture from font %s! SDL Error: %s\r\n", fontPath, SDL_GetError());
            
            // Clean up
            SDL_FreeSurface(loadedSurface);
        }
    }
    
    return newTexture;
}

SDL_Texture *draw_loadTexture(AppState state, char *path) {
    // The final Texture
    SDL_Texture *newTexture = NULL;
    
    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path);
    
    if(loadedSurface == NULL)
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\r\n", path, IMG_GetError());
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(state.renderer, loadedSurface);
        
        if(newTexture == NULL)
            fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\r\n", path, SDL_GetError());
        
        // Clean up
        SDL_FreeSurface(loadedSurface);
    }
    
    return newTexture;
}

AppState draw_addLine(AppState state, Line line) {
    AppState stateCopy = state;
    
    if(stateCopy.lines == NULL) {
        stateCopy.lineCount = 1;
        stateCopy.lines = malloc(sizeof(Line));
    } else {
        stateCopy.lineCount++;
        stateCopy.lines = realloc(stateCopy.lines, sizeof(Line) * stateCopy.lineCount);
    }
    
    stateCopy.lines[stateCopy.lineCount - 1] = line;
    return stateCopy;
}

AppState draw_eraseLine(AppState state, Line line) {
    AppState stateCopy = state;
    
    for(int i = 0; i < stateCopy.lineCount; i++) {
        // Find the line with the carried point memory (so it's the right line to erase)
        if(line.m_points == stateCopy.lines[i].m_points) {
            // Get rid of line's memory and remove it from list
            stateCopy.lines[i] = line_cleanLine(stateCopy.lines[i]);
            memcpy(stateCopy.lines + i, stateCopy.lines + i + 1, sizeof(Line) * (stateCopy.lineCount - i - 1));
            
            stateCopy.lineCount--;
            stateCopy.lines = realloc(stateCopy.lines, sizeof(Line) * stateCopy.lineCount);
            
            // break to return new appstate
            return stateCopy;
        }
    }
    
    return stateCopy;
}
