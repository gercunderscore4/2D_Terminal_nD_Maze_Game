#include <cstdio>
#include <cmath>
#include "SDL2/SDL.h"
#include "m4.h"
#include "fileio.h"
#include "display.h"
#include "control.h"

/*
#if defined(__IPHONEOS__) || defined(__ANDROID__)
	#include "SDL_opengles.h"
#else // Computer
	// openGL
	#include "SDL2/SDL_opengl.h"
#endif
*/

using namespace std;

#define MAZEX 3
#define MAZEY 3
#define MAZEZ 3
#define MAZEW 3

int main (void)
{
	char buff[FILE_BUFFER_SIZE];
	sprintf(buff, "\n");
	writeToFile(buff);
	
	sprintf(buff,"BEGIN PROGRAM");
	writeToFile(buff);
		
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sprintf(buff, "ERROR: Failed to init. %s\n", SDL_GetError());
		writeToFile(buff);
		return 1;
	}

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		sprintf(buff, "ERROR: Desktop side unknown. %s\n", SDL_GetError());
		writeToFile(buff);
		SDL_Quit();
		return 1;
	}
		
	// create window
	SDL_Window* window = NULL;
	window = SDL_CreateWindow("gc", 0, 0, dm.w, dm.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN); 
	if (window == NULL) {
		sprintf(buff, "ERROR: Window did not create. %s\n", SDL_GetError());
		writeToFile(buff);
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		sprintf(buff, "ERROR: Renderer did not create. %s\n", SDL_GetError());
		writeToFile(buff);
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	// init game stuff
	sprintf(buff,"MAKE MAZE");
	writeToFile(buff);
	m4 maze;
	maze.makeMaze(MAZEX, MAZEY, MAZEZ, MAZEW);
	
	sprintf(buff,"BEGIN LOOP");
	writeToFile(buff);

	bool quit = false;
	SDL_Event event;
	// while game not quit, continue playing game
	while (quit == false) {
		
		sprintf(buff, "LOOPING");
		writeToFile(buff);
		
		// event loop (while stuff is happening, do that stuff
		while (SDL_PollEvent(&event) != 0) {

			// parse events
			sprintf(buff, "EVENT");
			writeToFile(buff);
	
			quit = controls_k(&maze, &event);
			
			maze.discover();
			if (maze.get_flag(F_GOAL) == true ) {
				sprintf(buff, "WIN");
				writeToFile(buff);
				maze.makeMaze(MAZEX, MAZEY, MAZEZ, MAZEW);
				//quit = true;
			}
		}

		sprintf(buff, "GRAPHICS");
		writeToFile(buff);


		// GRAPHICS
		// clear screen
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
	
		draw_disc_better(&maze, renderer);

		// update screen
		SDL_RenderPresent(renderer);
	}
	
	sprintf(buff,"END LOOP");
	writeToFile(buff);

	sprintf(buff, "END PROGRAM");
	writeToFile(buff);
	
	sprintf(buff, "\n");
	writeToFile(buff);
	
	// destroy window
	// quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();

	return 0;
}
