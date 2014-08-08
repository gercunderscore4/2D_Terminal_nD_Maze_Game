#include "display.h"

void draw_maze (m4* maze, SDL_Renderer* renderer)
{
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);

	// sizing
	int mazeDiameter = dm.h > dm.w ? dm.h : dm.w;
	//int extraDiameter = dm.h > dm.w ? dm.w : dm.h;
	int a = (maze->get_lenx()+1) * (2*maze->get_lenz()-1) + 1;
	int b = (maze->get_leny()+1) * (2*maze->get_lenw()-1) + 1;
	if (b > a) {
		a = b;
	}
	int l_line = 1;
	int l_box = floor( (mazeDiameter - (a+1)*l_line) / a );
	int l_grid = l_box + l_line;
	int l_gap = l_box/8;

	// background
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect rectCenter = {0, 0, mazeDiameter, mazeDiameter};
	SDL_RenderFillRect(renderer, &rectCenter);

	// cage
	SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
	int u1;
	int u2;
	for (int h=0; h<maze->get_lenw(); h++) {
		for (int k=0; k<maze->get_lenz(); k++) {
			for (int j=0; j<maze->get_leny(); j++) {
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					// XD
					if (maze->get_flag(i,j,k,h,XD) == true) {
						SDL_Rect rectXD = {u1, u2, l_line, l_grid+l_line};
						SDL_RenderFillRect(renderer, &rectXD);
					}
					// YD
					if (maze->get_flag(i,j,k,h,YD) == true) {
						SDL_Rect rectYD = {u1, u2, l_grid+l_line, l_line};
						SDL_RenderFillRect(renderer, &rectYD);
					}
					// goal
					if (maze->get_flag(i,j,k,h,F_GOAL) == true) {
						SDL_SetRenderDrawColor(renderer, COLOUR_GOAL);
						u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
						SDL_Rect rectGoal = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
						SDL_RenderFillRect(renderer, &rectGoal);
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
					}
				}
				// XU
				int i = maze->get_lenx()-1;
				if (maze->get_flag(i,j,k,h,XU) == true) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectXU = {u1, u2, l_line, l_line+l_box+l_line};
					SDL_RenderFillRect(renderer, &rectXU);
				}
			}
			int j = maze->get_leny()-1;
			for (int i=0; i<maze->get_lenx(); i++) {
				// YU
				if (maze->get_flag(i,j,k,h,YU) == true) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
					SDL_Rect rectYU = {u1, u2, l_line+l_box+l_line, l_line};
					SDL_RenderFillRect(renderer, &rectYU);
				}
			}
			for (int j=0; j<maze->get_leny(); j++) {
			}
			if (k<maze->get_lenz()-1) {
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						// Z level
						u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
						//
						SDL_Rect rectZXD = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_RenderFillRect(renderer, &rectZXD);
						SDL_Rect rectZYD = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_RenderFillRect(renderer, &rectZYD);
						// ZU
						if (maze->get_flag(i,j,k,h,ZU) == true) {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
					int i = maze->get_lenx()-1;
					// COL 1
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectXU = {u1, u2, l_line, l_line+l_box+l_line};
					SDL_RenderFillRect(renderer, &rectXU);
					// COL 2
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectZXU = {u1, u2, l_line, l_line+l_box+l_line};
					SDL_RenderFillRect(renderer, &rectZXU);
				}
				int j = maze->get_leny()-1;
				// ZYU
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
					SDL_Rect rectZYU = {u1, u2, l_line+l_box+l_line, l_line};
					SDL_RenderFillRect(renderer, &rectZYU);
				}
			}
		}
		if (h<maze->get_lenw()-1) {
			for (int k=0; k<maze->get_lenz(); k++) {
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						
						// W level
						u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
						//
						SDL_Rect rectZXD = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_RenderFillRect(renderer, &rectZXD);
						SDL_Rect rectZYD = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_RenderFillRect(renderer, &rectZYD);
						// WU
						if (maze->get_flag(i,j,k,h,WU) == true) {
							u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
							u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
					int i = maze->get_lenx()-1;
					// COL 1
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectXU = {u1, u2, l_line, l_grid+l_line};
					SDL_RenderFillRect(renderer, &rectXU);
					// COL 2
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectZXU = {u1, u2, l_line, l_grid+l_line};
					SDL_RenderFillRect(renderer, &rectZXU);
				}
				int j = maze->get_leny()-1;
				// ZYU
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j+1)*l_grid;
					SDL_Rect rectZYU = {u1, u2, l_grid, l_line};
					SDL_RenderFillRect(renderer, &rectZYU);
				}
			}
		}
	}
	// player
	SDL_SetRenderDrawColor(renderer, COLOUR_USER);
	u1 = (1+2*maze->get_z()*(maze->get_lenx()+1)+maze->get_x())*l_grid;
	u2 = (1+2*maze->get_w()*(maze->get_leny()+1)+maze->get_y())*l_grid;
	SDL_Rect rectPlayer = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
	SDL_RenderFillRect(renderer, &rectPlayer);
}

////////////////////////////////////////////////////////////////
////////////////                                ////////////////
////////////////////////////////////////////////////////////////

void draw_disc (m4* maze, SDL_Renderer* renderer)
{
	// sizing
	int mazeDiameter = 0;
	//int extraDiameter = 0;
	if (SCREEN_WIDTH < SCREEN_HEIGHT) {
		mazeDiameter = SCREEN_WIDTH;
		//extraDiameter = SCREEN_HEIGHT - SCREEN_WIDTH;
	} else {
		mazeDiameter = SCREEN_HEIGHT;
		//extraDiameter = SCREEN_WIDTH - SCREEN_HEIGHT;
	}
	int a = (maze->get_lenx()+1) * (2*maze->get_lenz()-1) + 1;
	int b = (maze->get_leny()+1) * (2*maze->get_lenw()-1) + 1;
	if (b > a) {
		a = b;
	}
	int l_line = 1;
	int l_box = floor( (mazeDiameter - (a+1)*l_line) / a );
	int l_grid = l_box + l_line;
	int l_gap = l_box/8;

	// background
	SDL_SetRenderDrawColor(renderer, COLOUR_BACK);
	SDL_Rect rectCenter = {0, 0, mazeDiameter, mazeDiameter};
	SDL_RenderFillRect(renderer, &rectCenter);

	// cage
	SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
	int u1;
	int u2;
	for (int h=0; h<maze->get_lenw(); h++) {
		for (int k=0; k<maze->get_lenz(); k++) {
			// XY LEVEL
			for (int j=0; j<maze->get_leny(); j++) {
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					// XD
					if (maze->get_flag(i,j,k,h,XD) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC))) {
						SDL_Rect rectXD = {u1, u2, l_line, l_grid+l_line};
						SDL_RenderFillRect(renderer, &rectXD);
					}
					// YD
					if (maze->get_flag(i,j,k,h,YD) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC))) {
						SDL_Rect rectYD = {u1, u2, l_grid+l_line, l_line};
						SDL_RenderFillRect(renderer, &rectYD);
					}
					// goal
					if (maze->get_flag(i,j,k,h,F_GOAL) && maze->get_flag(i,j,k,h,F_DISC)) {
						SDL_Rect rectGoal = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
						SDL_SetRenderDrawColor(renderer, COLOUR_GOAL);
						SDL_RenderFillRect(renderer, &rectGoal);
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
					}
				}
			}
			// XU
			int i = maze->get_lenx()-1;
			for (int j=0; j<maze->get_leny(); j++) {
				if (maze->get_flag(i,j,k,h,XU) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC))) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					SDL_Rect rectXU = {u1, u2, l_line, l_line+l_box+l_line};
					SDL_RenderFillRect(renderer, &rectXU);
				}
			}
			// YU
			int j = maze->get_leny()-1;
			for (int i=0; i<maze->get_lenx(); i++) {
				if (maze->get_flag(i,j,k,h,YU) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC))) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
					SDL_Rect rectYU = {u1, u2, l_line+l_box+l_line, l_line};
					SDL_RenderFillRect(renderer, &rectYU);
				}
			}
			if (k<maze->get_lenz()-1) {
				// Z LEVEL
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
						// ZU XD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i-1,j,k+1,h,F_DISC)) {
							SDL_Rect rectZXD = {u1, u2, l_line, l_line+l_box+l_line};
							SDL_RenderFillRect(renderer, &rectZXD);
						}
						// ZU YD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i,j-1,k+1,h,F_DISC)) {
							SDL_Rect rectZYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_RenderFillRect(renderer, &rectZYD);
						}
						// ZU
						if (maze->get_flag(i,j,k,h,ZU) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC))) {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
				}
				int i = maze->get_lenx()-1;
				for (int j=0; j<maze->get_leny(); j++) {
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					// ZU XU
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i+1,j,k+1,h,F_DISC)) {
						SDL_Rect rectZXU = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_RenderFillRect(renderer, &rectZXU);
					}
				}
				// ZU YU
				int j = maze->get_leny()-1;
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
					// ZU YU
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i,j+1,k+1,h,F_DISC)) {
						SDL_Rect rectZYU = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_RenderFillRect(renderer, &rectZYU);
					}
				}
			}
		}
		if (h<maze->get_lenw()-1) {
			// W LEVEL
			for (int k=0; k<maze->get_lenz(); k++) {
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						// W level
						u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
						// WU XD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i-1,j,k,h+1,F_DISC)) {
							SDL_Rect rectWXD = {u1, u2, l_line, l_line+l_box+l_line};
							SDL_RenderFillRect(renderer, &rectWXD);
						}
						// WU YD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i,j-1,k,h+1,F_DISC)) {
							SDL_Rect rectWYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_RenderFillRect(renderer, &rectWYD);
						}
						// WU
						if (maze->get_flag(i,j,k,h,WU) && (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC))) {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
				}
				// WU XU
				int i = maze->get_lenx()-1;
				for (int j=0; j<maze->get_leny(); j++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i+1,j,k,h+1,F_DISC)) {
						SDL_Rect rectWXU = {u1, u2, l_line, l_grid+l_line};
						SDL_RenderFillRect(renderer, &rectWXU);
					}
				}
				// WU YU
				int j = maze->get_leny()-1;
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j+1)*l_grid;
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i,j+1,k,h+1,F_DISC)) {
						SDL_Rect rectWYU = {u1, u2, l_grid, l_line};
						SDL_RenderFillRect(renderer, &rectWYU);
					}
				}
			}
		}
	}
	// player
	SDL_SetRenderDrawColor(renderer, COLOUR_USER);
	u1 = (1+2*maze->get_z()*(maze->get_lenx()+1)+maze->get_x())*l_grid;
	u2 = (1+2*maze->get_w()*(maze->get_leny()+1)+maze->get_y())*l_grid;
	SDL_Rect rectPlayer = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
	SDL_RenderFillRect(renderer, &rectPlayer);
}

////////////////////////////////////////////////////////////////
////////////////                                ////////////////
////////////////////////////////////////////////////////////////

void draw_disc_better (m4* maze, SDL_Renderer* renderer)
{
	// sizing
	int mazeDiameter = 0;
	//int extraDiameter = 0;
	if (SCREEN_WIDTH < SCREEN_HEIGHT) {
		mazeDiameter = SCREEN_WIDTH;
		//extraDiameter = SCREEN_HEIGHT - SCREEN_WIDTH;
	} else {
		mazeDiameter = SCREEN_HEIGHT;
		//extraDiameter = SCREEN_WIDTH - SCREEN_HEIGHT;
	}
	int a = (maze->get_lenx()+1) * (2*maze->get_lenz()-1) + 1;
	int b = (maze->get_leny()+1) * (2*maze->get_lenw()-1) + 1;
	if (b > a) {
		a = b;
	}
	int l_line = 1;
	int l_box = floor( (mazeDiameter - (a+1)*l_line) / a );
	int l_grid = l_box + l_line;
	int l_gap = l_box/8;

	// background
	SDL_SetRenderDrawColor(renderer, COLOUR_BACK);
	SDL_Rect rectCenter = {0, 0, mazeDiameter, mazeDiameter};
	SDL_RenderFillRect(renderer, &rectCenter);

	// cage
	SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
	int u1;
	int u2;
	for (int h=0; h<maze->get_lenw(); h++) {
		for (int k=0; k<maze->get_lenz(); k++) {
			// XY LEVEL
			for (int j=0; j<maze->get_leny(); j++) {
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					// XD
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC)) {
						if (maze->get_flag(i,j,k,h,XD)) {
							SDL_Rect rectXD = {u1, u2, l_line, l_grid+l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectXD);
						}
					} else {
						SDL_Rect rectXD = {u1, u2, l_line, l_grid+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectXD);
					}
					// YD
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC)) {
						if (maze->get_flag(i,j,k,h,YD)) {
							SDL_Rect rectYD = {u1, u2, l_grid+l_line, l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectYD);
						}
					} else {
						SDL_Rect rectYD = {u1, u2, l_grid+l_line, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectYD);
					}
					// goal
					if (maze->get_flag(i,j,k,h,F_GOAL) && maze->get_flag(i,j,k,h,F_DISC)) {
						SDL_Rect rectGoal = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
						SDL_SetRenderDrawColor(renderer, COLOUR_GOAL);
						SDL_RenderFillRect(renderer, &rectGoal);
					}
				}
			}
			// XU
			int i = maze->get_lenx()-1;
			for (int j=0; j<maze->get_leny(); j++) {
				u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
				u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
				if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC)) {
					if (maze->get_flag(i,j,k,h,XU)) {
						SDL_Rect rectXU = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectXU);
					}
				} else {
					SDL_Rect rectXU = {u1, u2, l_line, l_line+l_box+l_line};
					SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
					SDL_RenderFillRect(renderer, &rectXU);
				}
			}
			// YU
			int j = maze->get_leny()-1;
			for (int i=0; i<maze->get_lenx(); i++) {
				u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
				u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
				if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC)) {
					if (maze->get_flag(i,j,k,h,YU)) {
						SDL_Rect rectYU = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectYU);
					}
				} else {
					SDL_Rect rectYU = {u1, u2, l_line+l_box+l_line, l_line};
					SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
					SDL_RenderFillRect(renderer, &rectYU);
				}
			}
			if (k<maze->get_lenz()-1) {
				// Z LEVEL
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
						// ZU XD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i-1,j,k+1,h,F_DISC)) {
							SDL_Rect rectZXD = {u1, u2, l_line, l_line+l_box+l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectZXD);
						} else {
							SDL_Rect rectZXD = {u1, u2, l_line, l_line+l_box+l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectZXD);
						}
						// ZU YD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i,j-1,k+1,h,F_DISC)) {
							SDL_Rect rectZYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectZYD);
						} else {
							SDL_Rect rectZYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectZYD);
						}
						// ZU
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC)) {
							if (maze->get_flag(i,j,k,h,ZU)) {
								SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
								SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
								SDL_RenderFillRect(renderer, &rectZU);
							}
						} else {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
				}
				int i = maze->get_lenx()-1;
				for (int j=0; j<maze->get_leny(); j++) {
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j)*l_grid;
					// ZU XU
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i+1,j,k+1,h,F_DISC)) {
						SDL_Rect rectZXU = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectZXU);
					} else {
						SDL_Rect rectZXU = {u1, u2, l_line, l_line+l_box+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectZXU);
					}
				}
				// ZU YU
				int j = maze->get_leny()-1;
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+(2*k+1)*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+2*h*(maze->get_leny()+1)+j+1)*l_grid;
					// ZU YU
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC) || maze->get_flag(i,j,k+1,h,F_DISC) || maze->get_flag(i,j+1,k+1,h,F_DISC)) {
						SDL_Rect rectZYU = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectZYU);
					} else {
						SDL_Rect rectZYU = {u1, u2, l_line+l_box+l_line, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectZYU);
					}
				}
			}
		}
		if (h<maze->get_lenw()-1) {
			// W LEVEL
			for (int k=0; k<maze->get_lenz(); k++) {
				for (int j=0; j<maze->get_leny(); j++) {
					for (int i=0; i<maze->get_lenx(); i++) {
						// W level
						u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
						u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
						// WU XD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i-1,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i-1,j,k,h+1,F_DISC)) {
							SDL_Rect rectWXD = {u1, u2, l_line, l_line+l_box+l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectWXD);
						} else {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectZU);
						}
						// WU YD
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j-1,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i,j-1,k,h+1,F_DISC)) {
							SDL_Rect rectWYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
							SDL_RenderFillRect(renderer, &rectWYD);
						} else {
							SDL_Rect rectWYD = {u1, u2, l_line+l_box+l_line, l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectWYD);
						}
						// WU
						if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC)) {
							if (maze->get_flag(i,j,k,h,WU)) {
								SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
								SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
								SDL_RenderFillRect(renderer, &rectZU);
							}
						} else {
							SDL_Rect rectZU = {u1+2*l_line, u2+2*l_line, l_box-2*l_line, l_box-2*l_line};
							SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
							SDL_RenderFillRect(renderer, &rectZU);
						}
					}
				}
				// WU XU
				int i = maze->get_lenx()-1;
				for (int j=0; j<maze->get_leny(); j++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i+1)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j)*l_grid;
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i+1,j,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i+1,j,k,h+1,F_DISC)) {
						SDL_Rect rectWXU = {u1, u2, l_line, l_grid+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectWXU);
					} else {
						SDL_Rect rectWXU = {u1, u2, l_line, l_grid+l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectWXU);
					}
				}
				// WU YU
				int j = maze->get_leny()-1;
				for (int i=0; i<maze->get_lenx(); i++) {
					u1 = (1+2*k*(maze->get_lenx()+1)+i)*l_grid;
					u2 = (1+(2*h+1)*(maze->get_leny()+1)+j+1)*l_grid;
					if (maze->get_flag(i,j,k,h,F_DISC) || maze->get_flag(i,j+1,k,h,F_DISC) || maze->get_flag(i,j,k,h+1,F_DISC) || maze->get_flag(i,j+1,k,h+1,F_DISC)) {
						SDL_Rect rectWYU = {u1, u2, l_grid, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_WALL);
						SDL_RenderFillRect(renderer, &rectWYU);
					} else {
						SDL_Rect rectWYU = {u1, u2, l_grid, l_line};
						SDL_SetRenderDrawColor(renderer, COLOUR_UNDI);
						SDL_RenderFillRect(renderer, &rectWYU);
					}
				}
			}
		}
	}
	// player
	SDL_SetRenderDrawColor(renderer, COLOUR_USER);
	u1 = (1+2*maze->get_z()*(maze->get_lenx()+1)+maze->get_x())*l_grid;
	u2 = (1+2*maze->get_w()*(maze->get_leny()+1)+maze->get_y())*l_grid;
	SDL_Rect rectPlayer = {u1+l_line+l_gap, u2+l_line+l_gap, l_box-2*l_gap, l_box-2*l_gap};
	SDL_RenderFillRect(renderer, &rectPlayer);
}
