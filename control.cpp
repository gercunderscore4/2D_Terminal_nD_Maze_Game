#include "control.h"

// returns quit
bool controls_k (m4* maze, SDL_Event* event)
{
	bool quit = false;
	// KEYBOARD CONTROLS
	if (event->type == SDL_QUIT) {
		quit = true;
	} else if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
			case SDLK_BACKQUOTE:
			{
				quit = true;
				break;
			}
			case SDLK_a:
			{
				maze->move(XD);
				break;
			}
			case SDLK_d:
			{
				maze->move(XU);
				break;
			}
			case SDLK_w:
			{
				maze->move(YD);
				break;
			}
			case SDLK_s:
			{
				maze->move(YU);
				break;
			}
			case SDLK_q:
			{
				maze->move(ZD);
				break;
			}
			case SDLK_e:
			{
				maze->move(ZU);
				break;
			}
			case SDLK_z:
			{
				maze->move(WD);
				break;
			}
			case SDLK_c:
			{
				maze->move(WU);
				break;
			}
		}
	}
	return false;
}

bool controls_t (m4* maze, SDL_Event* event)
{
	bool quit = false;
	if (event->tfinger.type == SDL_FINGERUP) {
		if (event->tfinger.y > 0.67) {
			// top row
			if (event->tfinger.x < 0.33) {
				// left
				// WD
				maze->move(WD);
			} else if  (event->tfinger.x < 0.67) {
				// middle
				// YU
				maze->move(YU);
			} else if  (event->tfinger.x > 1.00) {
				// right
				// ZU
				maze->move(ZU);
			}
		} else if  (event->tfinger.y > 0.33) {
			// middle row
			if (event->tfinger.x < 0.33) {
				// left
				// XD
				maze->move(XD);
			} else if  (event->tfinger.x < 0.67) {
				// middle
				// ??
				
			} else if  (event->tfinger.x > 1.00) {
				// right
				// XU
				maze->move(XU);
			}
		} else if  (event->tfinger.y > 0.00) {
			// bottom row
			if (event->tfinger.x < 0.33) {
				// left
				// ZD
				maze->move(ZD);
			} else if  (event->tfinger.x < 0.67) {
				// middle
				// YD
				maze->move(YD);
			} else if  (event->tfinger.x > 1.00) {
				// right
				// WU
				maze->move(WU);
			}
		}
		
	}
	return quit;
}
