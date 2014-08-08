#include "SDL2/SDL.h"
#include "m4.h"

/*
#if defined(__IPHONEOS__) || defined(__ANDROID__)
	#include "SDL_opengles.h"
#else // Computer
	// openGL
	#include "SDL2/SDL_opengl.h"
#endif
*/

#define SCREEN_WIDTH   720
#define SCREEN_HEIGHT 1200

#define COLOUR_WALL 0x00, 0xFF, 0x00, 0xFF
#define COLOUR_BACK 0x00, 0x00, 0x00, 0xFF
#define COLOUR_UNDI 0x00, 0x10, 0x00, 0xFF
#define COLOUR_USER 0xFF, 0xFF, 0xFF, 0xFF
#define COLOUR_GOAL 0xFF, 0xFF, 0x00, 0xFF

void draw_maze (m4* maze, SDL_Renderer* renderer);

void draw_disc (m4* maze, SDL_Renderer* renderer);

void draw_disc_better (m4* maze, SDL_Renderer* renderer);
