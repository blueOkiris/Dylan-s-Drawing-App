#include <SDL.h>
#include <stdio.h>

#include <app.h>
#include <draw.h>

int main(int argc, char **args) {
	AppState initState = app_initSystem();
	
	if(initState.error)
		return initState.error;
		
	app_close(draw_drawLoop(initState));
	
	return 0;
}
