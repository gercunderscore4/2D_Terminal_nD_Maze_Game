/*
 * FILE:    m4.cpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-19
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

#include "mn.hpp"
#include "m4.hpp"

////////////////////////////////////////////////////////////////
////////////////             META               ////////////////
////////////////////////////////////////////////////////////////

/*
 * For basic functionality testing, modify at will.
 */
void m4::do_stuff (void)
{
	lenx=5,leny=5,lenz=1,lenw=1;
	a0=0, a1=1, a2=2, a3=3;
	gen();
	
	//box();
	//cage();
	//random_build();
	//depth_build();
	//breadth_build();
	hunt_and_kill_build();
	
	print_data();
	print_all();
		
	degen();
}

/*
 * Play, manual size selection.
 */
void m4::play (void)
{
	print_title_screen();
	
	while (true) {
		// clear screen
		print_clr();
		
		// size
		get_size();
		
		// allocate
		gen();
		
		// build
		build_maze(alg);

		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		// pause
		//getchar();
		
		// play
		control();

		// deallocate
		degen();
	}
}

/*
 * Play, automatic size selection.
 */
void m4::play (int xs, int ys, int zs, int ws, int algs, int sights)
{
	while (true) {
		// clear screen
		print_clr();
		
		// size
		lenx=xs, leny=ys, lenz=zs, lenw=ws; // for testing
		
		// allocate
		gen();
		
		// build
		build_maze(algs);
		
		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		//getchar();
		
		// play
		sight = sights;
		control();
		
		// deallocate
		degen();
	}
}

/*
 * Testing for benchmarking.
 */
void m4::test (int n, int xs, int ys, int zs, int ws)
{
	int temp; // will literally be related to time
	int* build_time = new int[n];
	float build_avg = 0;
	int* solve_time = new int[n];
	float solve_avg = 0;
	
	// size
	lenx=xs, leny=ys, lenz=zs, lenw=ws; // for testing
		
	// allocate
	gen();
		
	printf("num bld slv (sec)\n");
	for (int i = 0; i < n; i++) {
		temp = (int) time(NULL);
		// build, choose one
		//random_build();
		//depth_build();
		//breadth_build();
		hunt_and_kill_build();
		//
		build_time[i] = (int) time(NULL) - temp;
		
		temp = (int) time(NULL);
		// solve, choose one
		rec_depth_solve();
		//breadth_solve();
		//
		solve_time[i] = (int) time(NULL) - temp;
		
		printf("%03i %03i %03i\n", i, build_time[i], solve_time[i]);
		build_avg += build_time[i];
		solve_avg += solve_time[i];
	}
	build_avg/=n;
	solve_avg/=n;
	printf("--- --- ---\n");
	printf("--- %7.3f\n", build_avg);
	printf("--- --- %7.3f\n", solve_avg);
	delete[] build_time;
	delete[] solve_time;
}

////////////////////////////////////////////////////////////////
////////////////            CLASS               ////////////////
////////////////////////////////////////////////////////////////

/*
 * Class constructor.
 */
m4::m4 (void)
{
	// all blank
	lenx=0,leny=0,lenz=0,lenw=0;	
	arry=NULL;
	x=0,y=0,z=0,w=0;
	gx=0,gy=0,gz=0,gw=0;
	a0=0,a1=1,a2=2,a3=3;
	
	// seed rand
	srand((int) time(NULL));
}

m4::~m4 (void)
{
	degen();
}

////////////////////////////////////////////////////////////////
////////////////          ALLOCATION            ////////////////
////////////////////////////////////////////////////////////////

/*
 * Class constructor.
 */
void m4::gen (void)
{
	degen();
	// keep size within bounds
	if (lenx < LEN_MIN) lenx = LEN_MIN;
	if (lenx > LEN_MAX) lenx = LEN_MAX;
	if (leny < LEN_MIN) leny = LEN_MIN;
	if (leny > LEN_MAX) leny = LEN_MAX;
	if (lenz < LEN_MIN) lenz = LEN_MIN;
	if (lenz > LEN_MAX) lenz = LEN_MAX;
	if (lenw < LEN_MIN) lenw = LEN_MIN;
	if (lenw > LEN_MAX) lenw = LEN_MAX;

	// allocate
	arry = (node_t****) calloc(lenx+1,sizeof(node_t***));
	for (int i = 0; i < lenx+1; i++) {
		arry[i] = (node_t***) calloc(leny+1,sizeof(node_t**));
		for (int j = 0; j < leny+1; j++) {
			arry[i][j] = (node_t**) calloc(lenz+1,sizeof(node_t*));
			for (int k = 0; k < lenz+1; k++) {
				arry[i][j][k] = (node_t*) calloc(lenw+1,sizeof(node_t));
			}
		}
	}
}

/*
 * Class destructor.
 */
void m4::degen (void)
{
	if (arry != NULL) {
		for (int i = 0; i < lenx+1; i++) {
			for (int j = 0; j < leny+1; j++) {
				for (int k = 0; k < lenz+1; k++) {
					free(arry[i][j][k]);
				}
				free(arry[i][j]);
			}
			free(arry[i]);
		}
		free(arry);
		arry = NULL;
	
		lenx=0;
		leny=0;
		lenz=0;
		lenw=0;
		
		a0=0;
		a1=1;
		a2=2;
		a3=3;
	}	
}

////////////////////////////////////////////////////////////////
////////////////             BOXES              ////////////////
////////////////////////////////////////////////////////////////

/*
+---------+
|?????????|
|?????????|
|?????????|
|?????????|
+---------+
*/
void m4::frame (void)
{
	// make outer walls
	/*
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			for (k = 1; k < lenz; k++) {
				for (h = 1; h < lenw; h++) {
	*/
	// XD & XU
	for (int j = 0; j < leny; j++) {
		for (int k = 0; k < lenz; k++) {
			for (int h = 0; h < lenw; h++) {
				// XD
				arry[0][j][k][h] |= XD;
				// XU
				arry[lenx-1][j][k][h] |= XU;
			}
		}
	}
	// YD & YU
	for (int i = 0; i < lenx; i++) {
		for (int k = 0; k < lenz; k++) {
			for (int h = 0; h < lenw; h++) {
				// YD
				arry[i][0][k][h] |= YD;
				// YU
				arry[i][leny-1][k][h] |= YU;
			}
		}
	}
	// ZD & ZU
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int h = 0; h < lenw; h++) {
				// ZD
				arry[i][j][0][h] |= ZD;
				// ZU
				arry[i][j][lenz-1][h] |= ZU;
			}
		}
	}
	// WD & WU
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				// WD
				arry[i][j][k][0] |= WD;
				// WU
				arry[i][j][k][lenw-1] |= WU;
			}
		}
	}
}

/*
+---------+
|         |
|         |
|         |
|         |
+---------+
*/
void m4::box (void)
{
	empty();
	frame();
}

/*
+++++++++++
+++++++++++
+++++++++++
+++++++++++
+++++++++++
+++++++++++
*/
void m4::cage (void)
{
	// build walls
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					arry[i][j][k][h] = XD|YD|ZD|WD | XU|YU|ZU|WU;
				}
			}
		}
	}
}

/*
...........
...........
...........
...........
...........
...........
*/
void m4::empty (void)
{
	// make outer walls
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					arry[i][j][k][h] = 0;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////
////////////////           GRAPHICS             ////////////////
////////////////////////////////////////////////////////////////

/*
 * Print based on sight algorithm.
 */
void m4::print_sight (int sights)
{
	switch (sights) {
		case S_FULL:
			//set_flag_all(F_DISC);
			//print_disc();
			print_all();
			break;
		case S_RANGE:
			clear_flag_all(F_DISC);
			disc_ranged(RANGE);
			print_disc();
			break;
		case S_RANGE_DISC:
			disc_ranged(RANGE);
			print_disc();
			break;
		case S_LOS:
			clear_flag_all(F_DISC);
			disc_line_of_sight();
			print_disc();
			break;
		case S_LOS_DISC:
			disc_line_of_sight();
			print_disc();
			break;
		case S_LOS_RANGE:
			clear_flag_all(F_DISC);
			disc_line_of_sight_ranged(RANGE);
			print_disc();
			break;
		case S_LOS_RANGE_DISC:
			disc_line_of_sight_ranged(RANGE);
			print_disc();
			break;
		default:
			print_all();
			break;
	}
}

/*
 * Clear screen.
 */
void m4::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

/*
 * Title screen.
 */
void m4::print_title_screen (void)
{
	// clear screen
	print_clr();
	
	// title screen
	printf(
		"2D Terminal nD Maze Game\n"
		"                        \n"
		"   By Geoffrey Card     \n"
		"                        \n"
		"      press enter       \n"
		"                        \n"
	);
	// press enter
	char c = 0;
	do {
		c = getchar();
	} while (c != '\n');
}

/*
 * Print manual.
 */
void m4::print_man (void)
{
	char alet[] = {'X', 'Y', 'Z', 'W'};
	printf(
		"  axes: \n"
		"  +---------------------%c \n"
		"  |                       \n"
		"  |  +----%c     +----%c    \n"
		"  |  |          |         \n"
		"  |  |          |         \n"
		"  |  %c          %c         \n"
		"  |                       \n"
		"  |  +----%c     +----%c    \n"
		"  |  |          |         \n"
		"  |  |          |         \n"
		"  |  %c          %c         \n"
		"  |                       \n"
		"  %c                       \n"
		"  \n"
		"  controls: \n"
		"  X+   %c \n"
		"  X-   %c \n"
		"  Y+   %c \n"
		"  Y-   %c \n"
		"  Z+   %c \n"
		"  Z-   %c \n"
		"  W+   %c \n"
		"  W-   %c \n"
		"  new  %c \n"
		"  d-swap: (all swap with %c) \n"
		"  X    %c \n"
		"  Y    %c \n"
		"  Z    %c \n"
		"  W    %c \n"
		"  type it then hit enter \n"
		"  \n"
		"  USER: %s \n"
		"  GOAL: %s \n"
		"  \n",
		alet[a2],
		alet[a0], alet[a0], 
		alet[a1], alet[a1], 
		alet[a0], alet[a0], 
		alet[a1], alet[a1], 
		alet[a3], 
		XP, XM, YP, YM, ZP, ZM, WP, WM, RESET, 
		DSWAPX, DSWAPX, DSWAPY, DSWAPZ, DSWAPW, 
		USER, GOAL);
}

/*
    ::YD::  ::::::  
    XD  XU  ::ZU::  
    ::YU::  ::::::  
                    
    ::::::  ......  
    ::WU::  ......  
    ::::::  ......  
                    
*/ 
void m4::print_all (void)
{
	// debug
	//printf("address %0X\nlenx %i\nleny %i\nlenz %i\nlenw %i\n", arry, lenx, leny, lenz, lenw);
	//printf("a1 = %i, a2 = %i, a3 = %i, a4 = %i\n", a1, a2,a3, a4);
	int aint[] = {0, 0, 0, 0};
	const int alen[] = {lenx, leny, lenz, lenw};
	const node_t awd[] = {XD, YD, ZD, WD};
	const node_t awu[] = {XU, YU, ZU, WU};
	
	for (aint[a3]=0; aint[a3]<alen[a3]; aint[a3]++) {
		for (aint[a1]=0; aint[a1]<alen[a1]; aint[a1]++) {
			
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 1
				// ROW 1
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					printf("%s", WALL);
					// BLOCK 2
					printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awd[a1] ? WALL : SPACE);
				}
				// BLOCK 3
				printf("%s", WALL);
				
				// SECTION 1
				// ROW 1
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", WALL);
					}
					// BLOCK 3
					printf("%s", WALL);
					printf("%s", HSKIP);
				}
			}
			printf("\n");
			
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 1
				// ROW 2
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awd[a0] ? WALL : SPACE);
					// BLOCK 2
					// USER/GOAL/ETC
					if (x==aint[0] && y==aint[1] && z==aint[2] && w==aint[3]) {
						printf("%s", USER);
					} else if (has_flag(aint[0],aint[1],aint[2],aint[3],F_GOAL)) {
						printf("%s", GOAL);
					} else {
						printf("%s", SPACE);
					}
				}
				aint[a0] = alen[a0]-1;
				// BLOCK 3
				printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a0] ? WALL : SPACE);
				
				// SECTION 1
				// ROW 2
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a2] ? WALL : SPACE);
					}
					// BLOCK 3
					printf("%s", WALL);
					printf("%s", HSKIP);
				}
			}
			printf("\n");
		}
		
		aint[a1] = alen[a1]-1;
		for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
			// SECTION 1
			// ROW 3
			// COL 1
			for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
				// BLOCK 1
				printf("%s", WALL);
				// BLOCK 2
				printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a1] ? WALL : SPACE);
			}
			// BLOCK 3
			printf("%s", WALL);
				
			// SECTION 1
			// ROW 3
			// COL 2
			if (aint[a2]<alen[a2]-1) {
				printf("%s", HSKIP);
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					printf("%s", WALL);
					// BLOCK 2
					printf("%s", WALL);
				}
				// BLOCK 3
				printf("%s", WALL);
				printf("%s", HSKIP);
			}
		}
		printf("\n");
		
		if (aint[a3]<alen[a3]-1) {
			printf("%s", VSKIP);
			
			for (aint[a1]=0; aint[a1]<alen[a1]; aint[a1]++) {
				for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
					// SECTION 2
					// ROW 1
					// COL 1
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", WALL);
					}
					// BLOCK 3
					printf("%s", WALL);
					
					// SECTION 2
					// ROW 1
					// COL 2
					if (aint[a2]<alen[a2]-1) {
						printf("%s", HSKIP);
						for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
							// BLOCK 1
							printf("%s", SPACE);
							// BLOCK 2
							printf("%s", SPACE);
						}
						// BLOCK 3
						printf("%s", SPACE);
						printf("%s", HSKIP);
					}
				}
				printf("\n");
				
				for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
					// SECTION 2
					// ROW 2
					// COL 1
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a3] ? WALL : SPACE);
					}
					// BLOCK 3
					printf("%s", WALL);
					
					// SECTION 2
					// ROW 2
					// COL 2
					if (aint[a2]<alen[a2]-1) {
						printf("%s", HSKIP);
						for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
							// BLOCK 1
							printf("%s", SPACE);
							// BLOCK 2
							printf("%s", SPACE);
						}
						// BLOCK 3
						printf("%s", SPACE);
						printf("%s", HSKIP);
					}
				}
				printf("\n");
			}
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 2
				// ROW 3
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					printf("%s", WALL);
					// BLOCK 2
					printf("%s", WALL);
				}
				// BLOCK 3
				printf("%s", WALL);
				
				// SECTION 2
				// ROW 3
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", SPACE);
						// BLOCK 2
						printf("%s", SPACE);
					}
					// BLOCK 3
					printf("%s", SPACE);
					printf("%s", HSKIP);
				}
			}
			printf("\n");
			printf("%s", VSKIP);
		}
	}
}

/*
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
                    
    ::::::::::::::  ::YD::YD::YD::  ::::::::::::::  
    ::ZU::ZU::ZU::  XD  XD  XD  XU  ::ZU::ZU::ZU::  
    ::::::::::::::  ::YD::YD::YD::  ::::::::::::::  
    ::ZU::ZU::ZU::  XD  XD  XD  XU  ::ZU::ZU::ZU::  
    ::::::::::::::  ::YD::YD::YD::  ::::::::::::::  
    ::ZU::ZU::ZU::  XD  XD  XD  XU  ::ZU::ZU::ZU::  
    ::::::::::::::  ::YU::YU::YU::  ::::::::::::::  
                    
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
    ..............  ::WU::WU::WU::  ..............  
    ..............  ::::::::::::::  ..............  
                    
    ::YD::  ::::::  
    XD  XU  ::ZU::  
    ::YU::  ::::::  
                    
    ::::::  ......  
    ::WU::  ......  
    ::::::  ......  
*/
/*
 * Not implemented. Prints discovered areas only.
 */
bool m4::disc_s1r1c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a0]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	}
	
	aint[a0]++;
	return false;
}

bool m4::disc_s1r1c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a1]++;
	return false;
}

bool m4::disc_s1r1c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	}
	
	aint[a1]++;
	return false;
}

bool m4::disc_s1r2c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 

	aint[a0]++;
	return false;
}

bool m4::disc_s1r2c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool m4::disc_s1r2c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool m4::disc_s1r3c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 
	
	aint[a0]++;
	return false;
}

bool m4::disc_s1r3c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool m4::disc_s1r3c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool m4::disc_s1r1c2b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a0]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	}
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a2]--;
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a2]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	}
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r1c2b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a2]--;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	}
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r1c2b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a0]--;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		return true;
	}
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a2]--;
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a2]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	}
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r2c2b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a2]--;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	} 
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r2c2b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	} 
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r2c2b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a2]--;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	} 
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r3c2b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a2]--;
		return true;
	} 

	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	} 
	
	aint[a2]--;
	return false;
}

bool m4::disc_s1r3c2b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	} 

	aint[a2]--;
	return false;
}

bool m4::disc_s1r3c2b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	}
	
	aint[a2]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a2]--;
		return true;
	}

	aint[a2]--;
	return false;
}

bool m4::disc_s2r1c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a0]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	}
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a3]--;
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a3]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	}
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r1c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a3]--;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	}
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r1c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a1]++;
		aint[a0]--;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		return true;
	}
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a3]--;
		return true;
	} 
	
	aint[a1]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a3]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		aint[a1]++;
		return true;
	} 
	
	aint[a1]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	}
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r2c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a3]--;
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	} 
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r2c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	} 
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r2c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]--;
		aint[a3]--;
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	} 
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r3c1b1 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a0]--;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a0]++;
		aint[a3]--;
		return true;
	}

	aint[a0]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	} 
	
	aint[a3]--;
	return false;
}

bool m4::disc_s2r3c1b2 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	} 

	aint[a3]--;
	return false;
}

bool m4::disc_s2r3c1b3 (int aint[4])
{
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	}
	
	aint[a3]++;
	if ( has_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[a3]--;
		return true;
	}

	aint[a3]--;
	return false;
}

void m4::print_disc (void)
{
	// debug
	//printf("address %0X\nlenx %i\nleny %i\nlenz %i\nlenw %i\n", arry, lenx, leny, lenz, lenw);
	//printf("a1 = %i, a2 = %i, a3 = %i, a4 = %i\n", a1, a2,a3, a4);
	int aint[] = {0, 0, 0, 0};
	const int alen[] = {lenx, leny, lenz, lenw};
	const node_t awd[] = {XD, YD, ZD, WD};
	const node_t awu[] = {XU, YU, ZU, WU};
	
	for (aint[a3]=0; aint[a3]<alen[a3]; aint[a3]++) {
		for (aint[a1]=0; aint[a1]<alen[a1]; aint[a1]++) {
			
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 1
				// ROW 1
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					if ( disc_s1r1c1b1(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					if ( disc_s1r1c1b2(aint) ) {
						printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awd[a1] ? WALL : SPACE);
					} else {
						printf("%s", UNDISC);
					}
				}
				// BLOCK 3
				aint[a0] = alen[a0]-1;
				if ( disc_s1r1c1b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 1
				// ROW 1
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						if ( disc_s1r1c2b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s1r1c2b2(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[a0] = alen[a0]-1;
					if ( disc_s1r1c2b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					printf("%s", HSKIP);
				}
			}
			printf("\n");
			
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 1
				// ROW 2
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					if ( disc_s1r2c1b1(aint) ) {
						printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awd[a0] ? WALL : SPACE);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					// USER/GOAL/ETC
					if ( !disc_s1r2c1b2(aint) ) {
						printf("%s", UNDISC);
					} else if (x==aint[0] && y==aint[1] && z==aint[2] && w==aint[3]) {
						printf("%s", USER);
					} else if (has_flag(aint[0],aint[1],aint[2],aint[3],F_GOAL)) {
						printf("%s", GOAL);
					} else {
						printf("%s", SPACE);
					}
				}
				aint[a0] = alen[a0]-1;
				// BLOCK 3
				if ( disc_s1r2c1b3(aint) ) {
					printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a0] ? WALL : SPACE);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 1
				// ROW 2
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						if ( disc_s1r2c2b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s1r2c2b2(aint) ) {
							printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a2] ? WALL : SPACE);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[a0] = alen[a0]-1;
					if ( disc_s1r2c2b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					printf("%s", HSKIP);
				}
			}
			printf("\n");
		}
		
		aint[a1] = alen[a1]-1;
		for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
			// SECTION 1
			// ROW 3
			// COL 1
			for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
				// BLOCK 1
				if ( disc_s1r3c1b1(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				// BLOCK 2
				if ( disc_s1r3c1b2(aint) ) {
					printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a1] ? WALL : SPACE);
				} else {
					printf("%s", UNDISC);
				}
			}
			// BLOCK 3
			aint[a0] = alen[a0]-1;
			if ( disc_s1r3c1b3(aint) ) {
				printf("%s", WALL);
			} else {
				printf("%s", UNDISC);
			}
			
			// SECTION 1
			// ROW 3
			// COL 2
			if (aint[a2]<alen[a2]-1) {
				printf("%s", HSKIP);
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					if ( disc_s1r3c2b1(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					if ( disc_s1r3c2b2(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
				}
				// BLOCK 3
				aint[a0] = alen[a0]-1;
				if ( disc_s1r3c2b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				printf("%s", HSKIP);
			}
		}
		printf("\n");
		
		if (aint[a3]<alen[a3]-1) {
			printf("%s", VSKIP);
			
			for (aint[a1]=0; aint[a1]<alen[a1]; aint[a1]++) {
				for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
					// SECTION 2
					// ROW 1
					// COL 1
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						if ( disc_s2r1c1b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s2r1c1b2(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[a0] = alen[a0]-1;
					if ( disc_s2r1c1b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					
					// SECTION 2
					// ROW 1
					// COL 2
					if (aint[a2]<alen[a2]-1) {
						printf("%s", HSKIP);
						for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
							// BLOCK 1
							printf("%s", SPACE);
							// BLOCK 2
							printf("%s", SPACE);
						}
						// BLOCK 3
						printf("%s", SPACE);
						printf("%s", HSKIP);
					}
				}
				printf("\n");
				
				for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
					// SECTION 2
					// ROW 2
					// COL 1
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						if ( disc_s2r2c1b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s2r2c1b2(aint) ) {
							printf("%s", arry[aint[0]][aint[1]][aint[2]][aint[3]]&awu[a3] ? WALL : SPACE);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[a0] = alen[a0]-1;
					if ( disc_s2r2c1b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					
					// SECTION 2
					// ROW 2
					// COL 2
					if (aint[a2]<alen[a2]-1) {
						printf("%s", HSKIP);
						for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
							// BLOCK 1
							printf("%s", SPACE);
							// BLOCK 2
							printf("%s", SPACE);
						}
						// BLOCK 3
						aint[a0] = alen[a0]-1;
						printf("%s", SPACE);
						printf("%s", HSKIP);
					}
				}
				printf("\n");
			}
			aint[a1] = alen[a1]-1;
			for (aint[a2]=0; aint[a2]<alen[a2]; aint[a2]++) {
				// SECTION 2
				// ROW 3
				// COL 1
				for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
					// BLOCK 1
					if ( disc_s2r3c1b1(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					if ( disc_s2r3c1b2(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
				}
				// BLOCK 3
				aint[a0] = alen[a0]-1;
				if ( disc_s2r3c1b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 2
				// ROW 3
				// COL 2
				if (aint[a2]<alen[a2]-1) {
					printf("%s", HSKIP);
					for (aint[a0]=0; aint[a0]<alen[a0]; aint[a0]++) {
						// BLOCK 1
						printf("%s", SPACE);
						// BLOCK 2
						printf("%s", SPACE);
					}
					// BLOCK 3
					aint[a0] = alen[a0]-1;
					printf("%s", SPACE);
					printf("%s", HSKIP);
				}
			}
			printf("\n");
			printf("%s", VSKIP);
		}
	}
}

/*
 * Print hex for debug.
 */
void m4::print_data (void)
{
	for (int h=0; h<lenw; h++) {
		for (int j=0; j<leny; j++) {
			for (int k=0; k<lenz; k++) {
				for (int i=0; i<lenx; i++) {
					printf("%08X  ", arry[i][j][k][h]);
				}
				if (k<lenz-1) {
					printf("  ");
				}
			}
			printf("\n");
		}
		printf("\n");
	}
}


/*
 * Discovers line of sight.
 * For line of sight only, clear disc flags before this in control loop.
 */
void m4::disc_line_of_sight (void)
{
	// current position
	set_flag(F_DISC);
	
	int i=x, j=y, k=z, h=w;
	
	// X-
	while (can_move(i,j,k,h,XM)) {
		i--;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// X+
	while (can_move(i,j,k,h,XP)) {
		i++;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// Y-
	while (can_move(i,j,k,h,YM)) {
		j--;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Y+
	while (can_move(i,j,k,h,YP)) {
		j++;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Z-
	while (can_move(i,j,k,h,ZM)) {
		k--;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// Z+
	while (can_move(i,j,k,h,ZP)) {
		k++;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// W-
	while (can_move(i,j,k,h,WM)) {
		h--;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
	
	// W+
	while (can_move(i,j,k,h,WP)) {
		h++;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
}

/*
 * Discovers ranged line of sight.
 * For line of sight only, clear disc flags before this in control loop.
 */
void m4::disc_line_of_sight_ranged (int range)
{
	if (range >= 0) {
		// current position
		set_flag(F_DISC);
	}
	
	int i=x, j=y, k=z, h=w;
	
	// X-
	for (int ii=0; ii<range && can_move(i,j,k,h,XM); ii++) {
		i--;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// X+
	for (int ii=0; ii<range && can_move(i,j,k,h,XP); ii++) {
		i++;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// Y-
	for (int ii=0; ii<range && can_move(i,j,k,h,YM); ii++) {
		j--;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Y+
	for (int ii=0; ii<range && can_move(i,j,k,h,YP); ii++) {
		j++;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Z-
	for (int ii=0; ii<range && can_move(i,j,k,h,ZM); ii++) {
		k--;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// Z+
	for (int ii=0; ii<range && can_move(i,j,k,h,ZP); ii++) {
		k++;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// W-
	for (int ii=0; ii<range && can_move(i,j,k,h,WM); ii++) {
		h--;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
	
	// W+
	for (int ii=0; ii<range && can_move(i,j,k,h,WP); ii++) {
		h++;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
}

/*
 * Discovers ranged sight.
 * For near sight only, clear disc flags before this in control loop.
 */
void m4::disc_ranged (int range)
{
	int i=x, j=y, k=z, h=w;
	
	int i_min = i-range > 0 ? i-range : 0;
	int i_max = i+range < lenx ? i+range : lenx;
	int j_min = j-range > 0 ? j-range : 0;
	int j_max = j+range < leny ? j+range : leny;
	int k_min = k-range > 0 ? k-range : 0;
	int k_max = k+range < lenz ? k+range : lenz;
	int h_min = h-range > 0 ? h-range : 0;
	int h_max = h+range < lenw ? h+range : lenw;
	
	for (int i=i_min; i<i_max; i++) {
		for (int j=j_min; j<j_max; j++) {
			for (int k=k_min; k<k_max; k++) {
				for (int h=h_min; h<h_max; h++) {
					set_flag(i,j,k,h,F_DISC);
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////
////////////////            CONTROLS            ////////////////
////////////////////////////////////////////////////////////////

/*
 * Get dimensions and size.
 */
void m4::get_size (void)
{
	// get n
	int n = 0;
	printf("dimensions (0-4):\n");
	scanf("%i", &n);
	if ( !(0 <= n && n <= 4) ) { // written this way to deal with NaN issues
		n = 2;
	}
	// get size
	lenx = 1; leny = 1; lenz = 1, lenw = 1;
	if (n >= 1) {
		printf("x size (<=%i):\n", LEN_MAX);
		scanf("%i", &lenx);
		if (n >= 2) {
			printf("y size (<=%i):\n", LEN_MAX);
			scanf("%i", &leny);
			if (n >= 3) {
				printf("z size (<=%i):\n", LEN_MAX);
				scanf("%i", &lenz);
				if (n >= 4) {
					printf("w size (<=%i):\n", LEN_MAX);
					scanf("%i", &lenw);
				}
			}
		}
	}
	
	printf(
		"Maze algorithm:\n"
		"0 = Random\n"
		"1 = Random solvable\n"
		"2 = Depth-first\n"
		"3 = Breadth-first (Prim)\n"
		"4 = Hunt-and-kill\n"
		);
	scanf("%i", &alg);

	printf(
		"Sight algorithm:\n"
		"0 = Full sight\n"
		"1 = Ranged sight\n"
		"2 = Ranged sight with memory\n"
		"3 = Line-of-sight\n"
		"4 = Line-of-sight with memory\n"
		"5 = Line-of-sight ranged\n"
		"6 = Line-of-sight ranged with memory\n"
		);
	scanf("%i", &sight);
}

/*
 * Build maze, selecting algorithm.
 */
void m4::build_maze(int algs)
{
	// build, choose one
	switch (algs) {
		case ALG_RAND:
			random_build();
			break;
		case ALG_RAND_S:
			random_solved_build();
			break;
		case ALG_DEPTH:
			depth_build();
			break;
		case ALG_BREAD:
			breadth_build();
			break;
		case ALG_HUNT:
			hunt_and_kill_build();
			break;
		default:
			box();
			gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
			set_flag(gx,gy,gz,gw,F_GOAL);
			break;
	}
}

/*
 * Main control loop.
 */
bool m4::control (void)
{
	x=0,y=0,z=0,w=0;
	
	bool cont = true;
	char comm = ' ';
	while (cont) {
		print_clr();
		print_man();
		print_sight(sight);
		// debug
		//printf ("%i %i %i %i\n", x, y, z, w);
		scanf("%c", &comm);
		if (comm == RESET) {
			cont = false;
		} else if (comm == XM && can_move(comm)) {
			x--;
		} else if (comm == XP && can_move(comm)) {
			x++;
		} else if (comm == YM && can_move(comm)) {
			y--;
		} else if (comm == YP && can_move(comm)) {
			y++;
		} else if (comm == ZM && can_move(comm)) {
			z--;
		} else if (comm == ZP && can_move(comm)) {
			z++;
		} else if (comm == WM && can_move(comm)) {
			w--;
		} else if (comm == WP && can_move(comm)) {
			w++;
		} else if (comm == DSWAPX) {
			d_swap_abs(DIMX, DIMX);
		} else if (comm == DSWAPY) {
			d_swap_abs(DIMX, DIMY);
		} else if (comm == DSWAPZ) {
			d_swap_abs(DIMX, DIMZ);
		} else if (comm == DSWAPW) {
			d_swap_abs(DIMX, DIMW);
		}
		
		if (has_flag(F_GOAL)) {
			print_clr();
			print_man();
			print_sight(sight);
			
			printf("WIN!\n\a");
			// delete remaining commands
			while (getchar() != '\n');
			// pause
			do {
				comm = getchar(); 
			} 
			while (comm != '\n');
			return true;
		}
	}
	return false;
}

/*
 * Dimension swap, swap through the axis that is originally X.
 */
void m4::d_swap_abs (int d1, int d2)
{
	if (d1 == DIMX) {
		if (d2 == DIMY) {
			int temp = a0;
			a0 = a1;
			a1 = temp;
		} else if (d2 == DIMZ) {
			int temp = a0;
			a0 = a2;
			a2 = temp;
		} else if (d2 == DIMW) {
			int temp = a0;
			a0 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMY) {
		if (d2 == DIMX) {
			int temp = a1;
			a1 = a0;
			a0 = temp;
		} else if (d2 == DIMZ) {
			int temp = a1;
			a1 = a2;
			a2 = temp;
		} else if (d2 == DIMW) {
			int temp = a1;
			a1 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMZ) {
		if (d2 == DIMX) {
			int temp = a2;
			a2 = a0;
			a0 = temp;
		} else if (d2 == DIMY) {
			int temp = a2;
			a2 = a1;
			a1 = temp;
		} else if (d2 == DIMW) {
			int temp = a2;
			a2 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMW) {
		if (d2 == DIMX) {
			int temp = a3;
			a3 = a0;
			a0 = temp;
		} else if (d2 == DIMY) {
			int temp = a3;
			a3 = a1;
			a1 = temp;
		} else if (d2 == DIMZ) {
			int temp = a3;
			a3 = a2;
			a2 = temp;
		}
	}
}

/*
 * Dimension swap, swap through the axis called X.
 */
void m4::d_swap_rel (int d1, int d2)
{
	if (d1 == a0) {
		if (d2 == a0) {
			a0 = d2;
			a0 = d1;
		} else if (d2 == a1) {
			a0 = d2;
			a1 = d1;
		} else if (d2 == a2) {
			a0 = d2;
			a2 = d1;
		} else if (d2 == a3) {
			a0 = d2;
			a3 = d1;
		}
	} else if (d1 == a1) {
		if (d2 == a0) {
			a1 = d2;
			a0 = d1;
		} else if (d2 == a1) {
			a1 = d2;
			a1 = d1;
		} else if (d2 == a2) {
			a1 = d2;
			a2 = d1;
		} else if (d2 == a3) {
			a1 = d2;
			a3 = d1;
		}
	} else if (d1 == a2) {
		if (d2 == a0) {
			a2 = d2;
			a0 = d1;
		} else if (d2 == a1) {
			a2 = d2;
			a1 = d1;
		} else if (d2 == a2) {
			a2 = d2;
			a2 = d1;
		} else if (d2 == a3) {
			a2 = d2;
			a3 = d1;
		}
	} else if (d1 == a3) {
		if (d1 == a0) {
			a3 = d2;
			a0 = d1;
		} else if (d2 == a1) {
			a3 = d2;
			a1 = d1;
		} else if (d2 == a2) {
			a3 = d2;
			a2 = d1;
		} else if (d2 == a3) {
			a3 = d2;
			a3 = d1;
		}
	}
}

////////////////////////////////////////////////////////////////
////////////////            CHECKS              ////////////////
////////////////////////////////////////////////////////////////

/*
 * Is position valid?
 */
bool inline m4::valid (void)
{
	return 0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw ? true : false; 
}

/*
 * Is position valid?
 */
bool inline m4::valid (int i, int j, int k, int h)
{
	return 0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw ? true : false; 
}

/*
 * Set flag at position.
 */
void inline m4::set_flag (node_t flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] |= flag;
}

/*
 * Set flag at position.
 */
void inline m4::set_flag (int i, int j, int k, int h, node_t flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] |= flag;
}

/*
 * Set flag in all maze.
 */
void inline m4::set_flag_all (node_t flag)
{
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					arry[i][j][k][h] |= flag;
				}
			}
		}
	}
}
/*
 * Clear flag from position.
 */
void inline m4::clear_flag (node_t flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] &= ~flag;
}

/*
 * Clear flag from position.
 */
void inline m4::clear_flag (int i, int j, int k, int h, node_t flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] &= ~flag;
}

/*
 * Clear flag from all maze.
 */
void inline m4::clear_flag_all (node_t flag)
{
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					arry[i][j][k][h] &= ~flag;
				}
			}
		}
	}
}

/*
 * Is this flag up at position?
 */
bool inline m4::has_flag (node_t flag)
{
	return valid(x,y,z,w) && arry[x][y][z][w]&flag ? true : false;
}

/*
 * Is this flag up at position?
 */
bool inline m4::has_flag (int i, int j, int k, int h, node_t flag)
{
	return valid(i,j,k,h) && arry[i][j][k][h]&flag ? true : false;
}

/*
 * Can move from position in direction?
 */
bool inline m4::can_move(char dir)
{
	switch(dir) {
		case XM:
			return 0 < x && !(arry[x][y][z][w]&XD) ? true : false;
		case XP:
			return x < lenx-1 && !(arry[x][y][z][w]&XU) ? true : false;
		case YM:
			return 0 < y && !(arry[x][y][z][w]&YD) ? true : false;
		case YP:
			return y < leny-1 && !(arry[x][y][z][w]&YU) ? true : false;
		case ZM:
			return 0 < z && !(arry[x][y][z][w]&ZD) ? true : false;
		case ZP:
			return z < lenz-1 && !(arry[x][y][z][w]&ZU) ? true : false;
		case WM:
			return 0 < w && !(arry[x][y][z][w]&WD) ? true : false;
		case WP:
			return w < lenw-1 && !(arry[x][y][z][w]&WU) ? true : false;
		default:
			return false;
	}
}

/*
 * Can move from position in direction?
 */
bool inline m4::can_move(int i, int j, int k, int h, char dir)
{
	switch(dir) {
		case XM:
			return 0 < i && !(arry[i][j][k][h]&XD) ? true : false;
		case XP:
			return i < lenx-1 && !(arry[i][j][k][h]&XU) ? true : false;
		case YM:
			return 0 < j && !(arry[i][j][k][h]&YD) ? true : false;
		case YP:
			return j < leny-1 && !(arry[i][j][k][h]&YU) ? true : false;
		case ZM:
			return 0 < k && !(arry[i][j][k][h]&ZD) ? true : false;
		case ZP:
			return k < lenz-1 && !(arry[i][j][k][h]&ZU) ? true : false;
		case WM:
			return 0 < h && !(arry[i][j][k][h]&WD) ? true : false;
		case WP:
			return h < lenw-1 && !(arry[i][j][k][h]&WU) ? true : false;
		default:
			return false;
	}
}

/*
 * HULK SMASH WALL FROM POSITION IN DIRECTION?!
 */
bool m4::smash(char dir) {
	switch(dir) {
		case XM:
			if (valid(x-1,y,z,w)) {
				// smash
				arry[x][y][z][w] &= ~XD;
				arry[x-1][y][z][w] &= ~XU;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case XP:
			if (valid(x+1,y,z,w)) {
				// smash
				arry[x][y][z][w] &= ~XU;
				arry[x+1][y][z][w] &= ~XD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case YM:
			if (valid(x,y-1,z,w)) {
				// smash
				arry[x][y][z][w] &= ~YD;
				arry[x][y-1][z][w] &= ~YU;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case YP:
			if (valid(x,y+1,z,w)) {
				// smash
				arry[x][y][z][w] &= ~YU;
				arry[x][y+1][z][w] &= ~YD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case ZM:
			if (valid(x,y,z-1,w)) {
				// smash
				arry[x][y][z][w] &= ~ZD;
				arry[x][y][z-1][w] &= ~ZU;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case ZP:
			if (valid(x,y,z+1,w)) {
				// smash
				arry[x][y][z][w] &= ~ZU;
				arry[x][y][z+1][w] &= ~ZD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case WM:
			if (valid(x,y,z,w-1)) {
				// smash
				arry[x][y][z][w] &= ~WD;
				arry[x][y][z][w-1] &= ~WU;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case WP:
			if (valid(x,y,z,w+1)) {
				// smash
				arry[x][y][z][w] &= ~WU;
				arry[x][y][z][w+1] &= ~WD;
				// return to w
				return true;
			} else {
				// did not smash
				return false;
			}
		default:
			return false;
	}
}

/*
 * HULK SMASH WALL FROM POSITION IN DIRECTION?!
 */
bool m4::smash(int i, int j, int k, int h, char dir) {
	switch(dir) {
		case XM:
			if (valid(i-1,j,k,h)) {
				arry[i][j][k][h] &= ~XD;
				arry[i-1][j][k][h] &= ~XU;
				return true;
			} else {
				return false;
			}
		case XP:
			if (valid(i+1,j,k,h)) {
				arry[i][j][k][h] &= ~XU;
				arry[i+1][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YM:
			if (valid(i,j-1,k,h)) {
				arry[i][j][k][h] &= ~YD;
				arry[i][j-1][k][h] &= ~YU;
				return true;
			} else {
				return false;
			}
		case YP:
			if (valid(i,j+1,k,h)) {
				arry[i][j][k][h] &= ~YU;
				arry[i][j+1][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case ZM:
			if (valid(i,j,k-1,h)) {
				arry[i][j][k][h] &= ~ZD;
				arry[i][j][k-1][h] &= ~ZU;
				return true;
			} else {
				return false;
			}
		case ZP:
			if (valid(i,j,k+1,h)) {
				arry[i][j][k][h] &= ~ZU;
				arry[i][j][k+1][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case WM:
			if (valid(i,j,k,h-1)) {
				arry[i][j][k][h] &= ~WD;
				arry[i][j][k][h-1] &= ~WU;
				return true;
			} else {
				return false;
			}
		case WP:
			if (valid(i,j,k,h+1)) {
				arry[i][j][k][h] &= ~WU;
				arry[i][j][k][h+1] &= ~WD;
				return true;
			} else {
				return false;
			}
		default:
			return false;
	}
}

////////////////////////////////////////////////////////////////
////////////////            SOLVERS             ////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////
//       breadth solve        //
////////////////////////////////

// set to valid position before and after
int m4::depth_solve (void)
{
	int xs=x, ys=y, zs=z, ws=w;
	x=0, y=0, z=0, w=0;
	
	int ans = rec_depth_solve();
	
	x=xs, y=ys, z=zs, w=ws;
	
	return ans;
}

int m4::rec_depth_solve (void)
{
	// assume current position is valid
	//printf("%i %i %i %i\n",x,y,z,w);

	// goal reached
	if (has_flag(F_GOAL)) {
		clear_flag_all(F_TEMP);
		return 0;
	}

	// mark as visited
	set_flag(x,y,z,w,F_TEMP);

	// XD
	if (can_move(XM) && !has_flag(x-1,y,z,w,F_TEMP)) {
		// move forward
		x--;
		// recusrive call
		//printf("XM %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		x++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// XU
	if (can_move(XP) && !has_flag(x+1,y,z,w,F_TEMP)) {
		// move
		x++;
		// recusrive call
		//printf("XP %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		x--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// YD
	if (can_move(YM) && !has_flag(x,y-1,z,w,F_TEMP)) {
		// move
		y--;
		// recusrive call
		//printf("YM %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		y++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// YU
	if (can_move(YP) && !has_flag(x,y+1,z,w,F_TEMP)) {
		// move
		y++;
		// recusrive call
		//printf("YP %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		y--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// ZD
	if (can_move(ZM) && !has_flag(x,y,z-1,w,F_TEMP)) {
		// move
		z--;
		// recusrive call
		//printf("ZM %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		z++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// ZU
	if (can_move(ZP) && !has_flag(x,y,z+1,w,F_TEMP)) {
		// move
		z++;
		// recusrive call
		//printf("ZP %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		z--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// WD
	if (can_move(WM) && !has_flag(x,y,z,w-1,F_TEMP)) {
		// move
		w--;
		// recusrive call
		//printf("WM %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		w++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// WU
	if (can_move(WP) && !has_flag(x,y,z,w+1,F_TEMP)) {
		// move
		w++;
		// recusrive call
		//printf("WP %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		w--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// failed
	return -1;
}

////////////////////////////////
//       breadth solve        //
////////////////////////////////

int m4::breadth_solve (void)
{
	// save position
	int xs=x, ys=y, zs=z, ws=w;

	clear_flag_all(F_TEMP);
	
	queue <unsigned int> nodes;
	nodes.push((unsigned int) (x<<24) + (y<<16) + (z<<8) + (w<<0));
	while (!nodes.empty()) {
		x = 0XFF & (nodes.front()>>24);
		y = 0XFF & (nodes.front()>>16);
		z = 0XFF & (nodes.front()>>8);
		w = 0XFF & (nodes.front());
		nodes.pop();
		
		//printf("%i %i %i %i\n",x,y,z,w);
		
		// goal reached
		if (has_flag(F_GOAL)) {
			clear_flag_all(F_TEMP);
			x=ws, y=ys, z=zs, w=ws;
			return 1;
		}

		// mark as visited
		set_flag(x,y,z,w,F_TEMP);
		// debug
		//print_all();
		
		// check
		// XM
		if (can_move(XM) && !has_flag(x-1,y,z,w,F_TEMP)) {
			//printf("XM %i %i %i %i\n",x-1,y,z,w);
			nodes.push((unsigned int) ((x-1)<<24) + (y<<16) + (z<<8) + (w<<0));
		}
		// XP
		if (can_move(XP) && !has_flag(x+1,y,z,w,F_TEMP)) {
			//printf("XP %i %i %i %i\n",x+1,y,z,w);
			nodes.push((unsigned int) ((x+1)<<24) + (y<<16) + (z<<8) + (w<<0));
		}
		// YM
		if (can_move(YM) && !has_flag(x,y-1,z,w,F_TEMP)) {
			//printf("YM %i %i %i %i\n",x,y-1,z,w);
			nodes.push((unsigned int) (x<<24) + ((y-1)<<16) + (z<<8) + (w<<0));
		}
		// YP
		if (can_move(YP) && !has_flag(x,y+1,z,w,F_TEMP)) {
			//printf("YP %i %i %i %i\n",x,y+1,z,w);
			nodes.push((unsigned int) (x<<24) + ((y+1)<<16) + (z<<8) + (w<<0));
		}
		// ZM
		if (can_move(ZM) && !has_flag(x,y,z-1,w,F_TEMP)) {
			//printf("ZM %i %i %i %i\n",x,y,z-1,w);
			nodes.push((unsigned int) (x<<24) + (y<<16) + ((z-1)<<8) + (w<<0));
		}
		// ZP
		if (can_move(ZP) && !has_flag(x,y,z+1,w,F_TEMP)) {
			//printf("ZP %i %i %i %i\n",x,y,z+1,w);
			nodes.push((unsigned int) (x<<24) + (y<<16) + ((z+1)<<8) + (w<<0));
		}
		// WM
		if (can_move(WM) && !has_flag(x,y,z,w-1,F_TEMP)) {
			//printf("WM %i %i %i %i\n",x,y,z,w-1);
			nodes.push((unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w-1)<<0));
		}
		// WP
		if (can_move(WP) && !has_flag(x,y,z,w+1,F_TEMP)) {
			//printf("ZP %i %i %i %i\n",x,y,z,w+1);
			nodes.push((unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w+1)<<0));
		}
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x=xs, y=ys, z=zs, w=ws;
	
	return -1;
}

////////////////////////////////////////////////////////////////
////////////////           BUILDERS             ////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////
//      random unsolved       //
////////////////////////////////

void m4::random_build (void)
{
	// completely random
	cage();
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					if (rand() > RAND_MAX/2) smash(i,j,k,h,XM);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,YM);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,ZM);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,WM);
				}
			}
		}
	}
	frame();

	// start
	x=0,y=0,z=0,w=0;
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
	set_flag(gx,gy,gz,gw,F_GOAL);
}

////////////////////////////////
//       random solved        //
////////////////////////////////

void m4::random_solved_build (void)
{
	int c = -1;
	while (c < 0) {
		// completely random
		cage();
		for (int i = 0; i < lenx; i++) {
			for (int j = 0; j < leny; j++) {
				for (int k = 0; k < lenz; k++) {
					for (int h = 0; h < lenw; h++) {
						if (rand() > RAND_MAX/2) smash(i,j,k,h,XM);
						if (rand() > RAND_MAX/2) smash(i,j,k,h,YM);
						if (rand() > RAND_MAX/2) smash(i,j,k,h,ZM);
						if (rand() > RAND_MAX/2) smash(i,j,k,h,WM);
					}
				}
			}
		}
		frame();

		c = breadth_solve();
		//c = rec_depth_solve();
	}

	// start
	x=0,y=0,z=0,w=0;
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
	set_flag(gx,gy,gz,gw,F_GOAL);
}

////////////////////////////////
//        depth-first         //
////////////////////////////////

void m4::depth_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0,y=0,z=0,w=0;
	// smashy-smashy
	rec_depth_build();
	// remove flags
	clear_flag_all(F_TEMP);
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
	set_flag(gx,gy,gz,gw,F_GOAL);
}

// set to valid position before
void m4::rec_depth_build (void)
{
	// assume current position is valid

	// mark as visited
	set_flag(x,y,z,w,F_TEMP);
	
	//print_all();
	//printf("%i %i %i %i\n",x,y,z,w);
	//getchar();
	
	char dirs[] = {XM,XP,YM,YP,ZM,ZP,WM,WP};
	int ind = 0;
	char dir = 0;
	for (int n = DIRS4; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// XD
		// if node is available and wall can be smashed
		if (dir==XM && valid(x-1,y,z,w) && !has_flag(x-1,y,z,w,F_TEMP) && smash(XM)) {
			// move forward
			x--;
			// recusrive call
			//printf("XD\n");
			rec_depth_build();
			// move back
			x++;
		}
		
		// XU
		// if node is available and wall can be smashed
		if (dir==XP && valid(x+1,y,z,w) && !has_flag(x+1,y,z,w,F_TEMP) && smash(XP)) {
			// move forward
			x++;
			// recusrive call
			//printf("XU\n");
			rec_depth_build();
			// move back
			x--;
		}
		
		// YD
		// if node is available and wall can be smashed
		if (dir==YM && valid(x,y-1,z,w) && !has_flag(x,y-1,z,w,F_TEMP) && smash(YM)) {
			// move forward
			y--;
			// recusrive call
			//printf("YD\n");
			rec_depth_build();
			// move back
			y++;
		}
		
		// YU
		// if node is available and wall can be smashed
		if (dir==YP && valid(x,y+1,z,w) && !has_flag(x,y+1,z,w,F_TEMP) && smash(YP)) {
			// move forward
			y++;
			// recusrive call
			//printf("YU\n");
			rec_depth_build();
			// move back
			y--;
		}
		
		// ZD
		// if node is available and wall can be smashed
		if (dir==ZM && valid(x,y,z-1,w) && !has_flag(x,y,z-1,w,F_TEMP) && smash(ZM)) {
			// move forward
			z--;
			// recusrive call
			//printf("ZD\n");
			rec_depth_build();
			// move back
			z++;
		}
		
		// ZU
		// if node is available and wall can be smashed
		if (dir==ZP && valid(x,y,z+1,w) && !has_flag(x,y,z+1,w,F_TEMP) && smash(ZP)) {
			// move forward
			z++;
			// recusrive call
			//printf("ZU\n");
			rec_depth_build();
			// move back
			z--;
		}
		
		// WD
		// if node is available and wall can be smashed
		if (dir==WM && valid(x,y,z,w-1) && !has_flag(x,y,z,w-1,F_TEMP) && smash(WM)) {
			// move forward
			w--;
			// recusrive call
			//printf("WD\n");
			rec_depth_build();
			// move back
			w++;
		}
		
		// WU
		// if node is available and wall can be smashed
		if (dir==WP && valid(x,y,z,w+1) && !has_flag(x,y,z,w+1,F_TEMP) && smash(WP)) {
			// move forward
			w++;
			// recusrive call
			//printf("WU\n");
			rec_depth_build();
			// move back
			w--;
		}
		// swap remaining directions
		dirs[ind] = dirs[n-1];
	}
		
	// failed
	return;
}

////////////////////////////////
//       breadth-first        //
////////////////////////////////

void m4::breadth_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0,y=0,z=0,w=0;
	
	// save position
	int xs=x, ys=y, zs=z, ws=w;
	x = rand() % lenx;
	y = rand() % leny;
	z = rand() % lenz;
	w = rand() % lenw;

	clear_flag_all(F_TEMP);
	
	vector<prim> nodes;
	prim temp;

	// visited origin
	set_flag(x,y,z,w,F_TEMP);
		
	// check
	// XM
	if (valid(x-1,y,z,w) && !has_flag(x-1,y,z,w,F_TEMP)) {
		temp.dir = XP;
		temp.node = (unsigned int) ((x-1)<<24) + (y<<16) + (z<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x-1,y,z,w,F_TEMP);
	}
	// XP
	if (valid(x+1,y,z,w) && !has_flag(x+1,y,z,w,F_TEMP)) {
		temp.dir = XM;
		temp.node = (unsigned int) ((x+1)<<24) + (y<<16) + (z<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x+1,y,z,w,F_TEMP);
	}
	// YM
	if (valid(x,y-1,z,w) && !has_flag(x,y-1,z,w,F_TEMP)) {
		temp.dir = YP;
		temp.node = (unsigned int) (x<<24) + ((y-1)<<16) + (z<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x,y-1,z,w,F_TEMP);
	}
	// YP
	if (valid(x,y+1,z,w) && !has_flag(x,y+1,z,w,F_TEMP)) {
		temp.dir = YM;
		temp.node = (unsigned int) (x<<24) + ((y+1)<<16) + (z<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x,y+1,z,w,F_TEMP);
	}
	// ZM
	if (valid(x,y,z-1,w) && !has_flag(x,y,z-1,w,F_TEMP)) {
		temp.dir = ZP;
		temp.node = (unsigned int) (x<<24) + (y<<16) + ((z-1)<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x,y,z-1,w,F_TEMP);
	}
	// ZP
	if (valid(x,y,z+1,w) && !has_flag(x,y,z+1,w,F_TEMP)) {
		temp.dir = ZM;
		temp.node = (unsigned int) (x<<24) + (y<<16) + ((z+1)<<8) + (w<<0);
		nodes.push_back(temp);
		set_flag(x,y,z+1,w,F_TEMP);
	}
	// WM
	if (valid(x,y,z,w-1) && !has_flag(x,y,z,w-1,F_TEMP)) {
		temp.dir = WP;
		temp.node = (unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w-1)<<0);
		nodes.push_back(temp);
		set_flag(x,y,z,w-1,F_TEMP);
	}
	// WP
	if (valid(x,y,z,w+1) && !has_flag(x,y,z,w+1,F_TEMP)) {
		temp.dir = WM;
			temp.node = (unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w+1)<<0);
		nodes.push_back(temp);
		set_flag(x,y,z,w+1,F_TEMP);
	}

	// loop
	while (!nodes.empty()) {
		
		
		// choose random node
		//int r = rand() % nodes.size();
		
		float f = ((float) rand()/RAND_MAX ) * 10; // warning: magic number
		int r = floor( nodes.size() * (1-exp(-f)) );
		//printf("%7.3f %3i %3lu\n", f, r, nodes.size());
		
		// collect data
		x = 0xFF & (nodes[r].node>>24);
		y = 0xFF & (nodes[r].node>>16);
		z = 0xFF & (nodes[r].node>>8);
		w = 0xFF & (nodes[r].node);
		char dir = nodes[r].dir;
		
		// break wall
		//bool hulk = 
		smash(dir);
		
		// debug
		//printf("%i %i %i %X, %c, %i\n", x,y,z,w,dir,hulk);
		//print_all();
		
		// swap and remove
		nodes[r].node = nodes[nodes.size()-1].node;
		nodes[r].dir = nodes[nodes.size()-1].dir;
		nodes.pop_back();
		
		// mark as visited
		set_flag(x,y,z,w,F_TEMP);
		
		// check
		// XM
		if (valid(x-1,y,z,w) && !has_flag(x-1,y,z,w,F_TEMP)) {
			temp.dir = XP;
			temp.node = (unsigned int) ((x-1)<<24) + (y<<16) + (z<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x-1,y,z,w,F_TEMP);
		}
		// XP
		if (valid(x+1,y,z,w) && !has_flag(x+1,y,z,w,F_TEMP)) {
			temp.dir = XM;
			temp.node = (unsigned int) ((x+1)<<24) + (y<<16) + (z<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x+1,y,z,w,F_TEMP);
		}
		// YM
		if (valid(x,y-1,z,w) && !has_flag(x,y-1,z,w,F_TEMP)) {
			temp.dir = YP;
			temp.node = (unsigned int) (x<<24) + ((y-1)<<16) + (z<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x,y-1,z,w,F_TEMP);
		}
		// YP
		if (valid(x,y+1,z,w) && !has_flag(x,y+1,z,w,F_TEMP)) {
			temp.dir = YM;
			temp.node = (unsigned int) (x<<24) + ((y+1)<<16) + (z<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x,y+1,z,w,F_TEMP);
		}
		// ZM
		if (valid(x,y,z-1,w) && !has_flag(x,y,z-1,w,F_TEMP)) {
			temp.dir = ZP;
			temp.node = (unsigned int) (x<<24) + (y<<16) + ((z-1)<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x,y,z-1,w,F_TEMP);
		}
		// ZP
		if (valid(x,y,z+1,w) && !has_flag(x,y,z+1,w,F_TEMP)) {
			temp.dir = ZM;
			temp.node = (unsigned int) (x<<24) + (y<<16) + ((z+1)<<8) + (w<<0);
			nodes.push_back(temp);
			set_flag(x,y,z+1,w,F_TEMP);
		}
		// WM
		if (valid(x,y,z,w-1) && !has_flag(x,y,z,w-1,F_TEMP)) {
			temp.dir = WP;
			temp.node = (unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w-1)<<0);
			nodes.push_back(temp);
			set_flag(x,y,z,w-1,F_TEMP);
		}
		// WP
		if (valid(x,y,z,w+1) && !has_flag(x,y,z,w+1,F_TEMP)) {
			temp.dir = WM;
			temp.node = (unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w+1)<<0);
			nodes.push_back(temp);
			set_flag(x,y,z,w+1,F_TEMP);
		}
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x=xs, y=ys, z=zs, w=ws;
	
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
	set_flag(gx,gy,gz,gw,F_GOAL);
	return;
}

////////////////////////////////
//       hunt and kill        //
////////////////////////////////

void m4::hunt_and_kill_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0,y=0,z=0,w=0;
	// save original coordinates
	int xo=x,yo=y,zo=z,wo=w;
	
	// mark starting point
	set_flag(F_TEMP);
	// make a starting path
	for (int l = 10; l > 0; l--) {
		if (!kill()) break;
	}
	
	// hunt
	bool done = false;
	while (!done) {
		for (x = 0; x < lenx; x++) {
			for (y = 0; y < leny; y++) {
				for (z = 0; z < lenz; z++) {
					for (w = 0; w < lenw; w++) {
						// if
						//     this is an un-used node next to a used node
						// then
						//     break to used node and begin path
						if (kill_node()) {
							// make a starting path
							for (int l = 10; l > 0; l--) {
								if (!kill()) break;
							}
						}
					}
				}
			}
		}
		
		// check if every node visited
		done = true;
		for (x = 0; x < lenx; x++) {
			for (y = 0; y < leny; y++) {
				for (z = 0; z < lenz; z++) {
					for (w = 0; w < lenw; w++) {
						done &= has_flag(F_TEMP); 
					}
				}
			}
		}
		
		//print_all();
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x=xo,y=yo,z=zo,w=wo;
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1, gw=lenw-1;
	set_flag(gx,gy,gz,gw,F_GOAL);
	return;
}

bool m4::kill (void)
{
	char dirs[] = {XM,XP,YM,YP,ZM,ZP,WM,WP};
	int ind = 0;
	char dir = 0;
	for (int n = DIRS4; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// if
		//     direction is picked
		//     it's valid
		//     it's new
		//     and it can be smashed
		// then
		//     move to it
		//     return true
		if (dir==XM && valid(x-1,y,z,w) && !has_flag(x-1,y,z,w,F_TEMP) && smash(XM)) {
			x--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==XP && valid(x+1,y,z,w) && !has_flag(x+1,y,z,w,F_TEMP) && smash(XP)) {
			x++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==YM && valid(x,y-1,z,w) && !has_flag(x,y-1,z,w,F_TEMP) && smash(YM)) {
			y--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==YP && valid(x,y+1,z,w) && !has_flag(x,y+1,z,w,F_TEMP) && smash(YP)) {
			y++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==ZM && valid(x,y,z-1,w) && !has_flag(x,y,z-1,w,F_TEMP) && smash(ZM)) {
			z--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==ZP && valid(x,y,z+1,w) && !has_flag(x,y,z+1,w,F_TEMP) && smash(ZP)) {
			z++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==WM && valid(x,y,z,w-1) && !has_flag(x,y,z,w-1,F_TEMP) && smash(WM)) {
			w--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==WP && valid(x,y,z,w+1) && !has_flag(x,y,z,w+1,F_TEMP) && smash(WP)) {
			w++;
			set_flag(F_TEMP);
			return true;
		}
		// swap remaining directions
		dirs[ind] = dirs[n-1];
	}
	// couldn't move
	return false;
}

bool m4::kill_node (void)
{
	// if
	//     this is an un-used node next to a used node
	// then
	//     break to used node
	if (!has_flag(F_TEMP)) {
		char dirs[] = {XM,XP,YM,YP,ZM,ZP,WM,WP};
		int ind = 0;
		char dir = 0;
		for (int n = DIRS4; n > 0; n--) {
			ind = rand() % n;
			dir = dirs[ind];
		
			// if
			//     direction is picked
			//     it's valid
			//     it's old
			//     and it can be smashed
			// then
			//     return true
			if (dir==XM && valid(x-1,y,z,w) && has_flag(x-1,y,z,w,F_TEMP) && smash(XM)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==XP && valid(x+1,y,z,w) && has_flag(x+1,y,z,w,F_TEMP) && smash(XP)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==YM && valid(x,y-1,z,w) && has_flag(x,y-1,z,w,F_TEMP) && smash(YM)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==YP && valid(x,y+1,z,w) && has_flag(x,y+1,z,w,F_TEMP) && smash(YP)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==ZM && valid(x,y,z-1,w) && has_flag(x,y,z-1,w,F_TEMP) && smash(ZM)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==ZP && valid(x,y,z+1,w) && has_flag(x,y,z+1,w,F_TEMP) && smash(ZP)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==WM && valid(x,y,z,w-1) && has_flag(x,y,z,w-1,F_TEMP) && smash(WM)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==WP && valid(x,y,z,w+1) && has_flag(x,y,z,w+1,F_TEMP) && smash(WP)) {
				set_flag(F_TEMP);
				return true;
			}
			// swap remaining directions
			dirs[ind] = dirs[n-1];
		}
		// couldn't move
	}
	return false;
}
