#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <app.h>
#include <draw.h>

void app_close(AppState finalState) {
	// Clean up everything
	free(finalState.lines);
	SDL_DestroyWindow(finalState.window);
	SDL_DestroyRenderer(finalState.renderer);
	SDL_DestroyTexture(finalState.drawIcon);
	SDL_DestroyTexture(finalState.eraseIcon);
	SDL_DestroyTexture(finalState.saveIcon);
	SDL_DestroyTexture(finalState.newIcon);
	SDL_DestroyTexture(finalState.titleTexture);
	SDL_DestroyTexture(finalState.closeIcon);
	SDL_DestroyTexture(finalState.size1Icon);
	SDL_DestroyTexture(finalState.size2Icon);
	SDL_DestroyTexture(finalState.size3Icon);
	SDL_DestroyTexture(finalState.size4Icon);
	SDL_Quit();
}

AppState app_initSystem() {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *drawIcon = NULL, *eraseIcon = NULL, *saveIcon = NULL, *newIcon = NULL, *selectedBorder = NULL, *titleTexture = NULL, *closeIcon = NULL;
	SDL_Texture *size1Icon = NULL, *size2Icon = NULL, *size3Icon = NULL, *size4Icon = NULL;
	SDL_Event event;
	SDL_PollEvent(&event);
    
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
	return
	    (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
		fprintf(stderr, "SDL could not initialize! SDL Error: %s\r\n", SDL_GetError()) };
    else if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	return
	    (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
		fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\r\n", IMG_GetError()) };
    else if(TTF_Init() == -1)
	return 
	    (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
		fprintf(stderr, "Could not init SDL_ttf! SDL_ttf Error: %s\r\n", TTF_GetError()) };
    else if((window = SDL_CreateWindow(
			"Note Application",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP)) == NULL)
	return
	    (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
		fprintf(stderr, "Window could not be created! SDL Error: %s\r\n", SDL_GetError()) };
    else if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
	return 
	    (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
		fprintf(stderr, "Renderer could not be created! SDL Error: %s\r\n", SDL_GetError()) };
    else if((drawIcon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/draw_icon.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load draw icon!\r\n") };
    else if((eraseIcon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/erase_icon.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load erase icon!\r\n") };
    else if((saveIcon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/save_icon.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load save icon!\r\n") };
    else if((newIcon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/new_icon.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load new icon!\r\n") };
    else if((size1Icon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/size_opt_1.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load size icon 1!\r\n") };
    else if((size2Icon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/size_opt_2.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load size icon 2!\r\n") };
    else if((size3Icon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/size_opt_3.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load size icon 3!\r\n") };
    else if((size4Icon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/size_opt_4.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load size icon 4!\r\n") };
    else if((selectedBorder = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/selected_border.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load border icon!\r\n") };
    else if((closeIcon = draw_loadTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "images/close_icon.png")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to load close icon!\r\n") };
    else if((titleTexture = draw_textTexture(
                (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 },
                "fonts/AlexandriaFLF.ttf", 24, "Dylan's Note Drawing Application!")) == NULL)
        return
            (AppState) { NULL, NULL, event, 0, 0, { 0, 0 }, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DRAW, NULL, 0, { 0, NULL, 0 }, 0,
                fprintf(stderr, "Failed to init title texture!\r\n") };
    else
	return 
	    (AppState) { window, renderer, event, 0, 0, { 0, 0 }, 0, 
		drawIcon, eraseIcon, saveIcon, newIcon, selectedBorder, closeIcon, titleTexture,
		size1Icon, size2Icon, size3Icon, size4Icon,
		DRAW, NULL, 0, { 0, NULL, 0 }, 0, 0 };
}
