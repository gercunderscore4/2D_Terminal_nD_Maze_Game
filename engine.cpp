/*
 * FILE:    m4.cpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-19
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
 */

#include "engine.h"

using namespace std;

////////////////////////////////////////////////////////////////
////////////////            CLASS               ////////////////
////////////////////////////////////////////////////////////////

/*
 * Class constructor.
 */
engine_c::engine_c (void)
{
}

engine_c::~engine_c (void)
{
}

////////////////////////////////////////////////////////////////
////////////////             META               ////////////////
////////////////////////////////////////////////////////////////

/*
 * Play, manual size selection.
 */
void engine_c::play (void)
{
	print_title_screen();
	
	while (true) {
		// clear screen
		print_clr();
		
		// size and build
		if (!get_size_and_make()) {
			break;
		}
		
		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		// pause
		//getchar();
		
		// play
		control();
	}
}

/*
 * Play, automatic size selection.
 */
void engine_c::play (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights, goal_t goal)
{
	while (true) {
		// clear screen
		print_clr();
				
		// build
		makeMaze(lxs, lys, lzs, lws, algs, sights, goal);		

		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		//getchar();
		
		// play
		control();
	}
}

/*
 * Testing for benchmarking.
 */
void engine_c::test (void)
{
}

void engine_c::tutorial (void)
{
	printf("You are %s.\n", USER);
	printf("Your goal is to get to %s.\n", GOAL);
	printf("\n");
	printf("%s is walls.\n", WALL);
	printf("%s is empty space.\n", SPACE);
	printf("%s is unknown territory.\n", UNDISC);
	printf("\n");
	printf("To move, type %c, %c, %c, %c, %c, %c, %c, %c.\n", XM, XP, YM, YP, ZM, ZP, WM, WP);
	printf("To give up, type %c.\n", RESET);
	printf("To rotate your view, type %c, %c, %c, %c.\n", DSWAPX, DSWAPY, DSWAPZ, DSWAPW);
	printf("You must hit enter to execute a command.\n");
	printf("\n");
}

////////////////////////////////////////////////////////////////
////////////////            CONTROLS            ////////////////
////////////////////////////////////////////////////////////////

/*
 * Get dimensions and size.
 */
bool engine_c::get_size_and_make (void)
{
	unsigned int n = 0;
	unsigned int lxs = 1;
	unsigned int lys = 1;
	unsigned int lzs = 1;
	unsigned int lws = 1;
	alg_t alg = ALG_DEFAULT;
	goal_t goal = G_DEFAULT;
	disc_t sights = S_DEFAULT;
	
	// get number of dimensions
	printf("dimensions (1-4) (0 to quit):\n");
	scanf("%u", &n);
	
	if (n == 0) {
		return false;
	} else if ( !(n <= 4) ) { // written this way to deal with NaN issues
		n = 2;
	}
	
	// get size
	if (n >= 1) {
		printf("x size (<=%u):\n", LEN_MAX);
		scanf("%u", &lxs);
		if (n >= 2) {
			printf("y size (<=%u):\n", LEN_MAX);
			scanf("%u", &lys);
			if (n >= 3) {
				printf("z size (<=%u):\n", LEN_MAX);
				scanf("%u", &lzs);
				if (n >= 4) {
					printf("w size (<=%u):\n", LEN_MAX);
					scanf("%u", &lws);
				}
			}
		}
	}
	
	printf(
		"Maze algorithm:\n"
		"0 = Random\n"
		"1 = Depth-first\n"
		"2 = Breadth-first (Prim)\n"
		"3 = Hunt-and-kill\n"
		"4 = No maze\n"
		);
	scanf("%u", &n);
	switch (n) {
		case 0:
			alg = ALG_RAND;
			goal = G_LONGEST;
			break;
		case 1:
			alg = ALG_DEPTH;
			goal = G_LONGEST;
			break;
		case 2:
			alg = ALG_BREAD;
			goal = G_LONGEST;
			break;
		case 3:
			alg = ALG_HUNT;
			goal = G_LONGEST;
			break;
		case 4:
			alg = ALG_BOX;
			goal = G_SIMPLE;
			break;
		case 5:
			alg = ALG_CAGE;
			goal = G_LONGEST;
			break;
		default:
			alg = ALG_DEFAULT;
			goal = G_DEFAULT;
			break;
	}

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
	scanf("%u", &n);
	switch (n) {
		case 0:
			sights = S_FULL;
			break;
		case 1:
			sights = S_RANGE;
			break;
		case 2:
			sights = S_RANGE_DISC;
			break;
		case 3:
			sights = S_LOS;
			break;
		case 4:
			sights = S_LOS_DISC;
			break;
		case 5:
			sights = S_LOS_RANGE;
			break;
		case 6:
			sights = S_LOS_RANGE_DISC;
			break;
		default:
			sights = S_DEFAULT;
			break;
	}
	
	makeMaze(lxs, lys, lzs, lws, alg, sights, goal);
	
	return true;
}

/*
 * Main control loop.
 */
bool engine_c::control (void)
{
	bool cont = true;
	char comm = ' ';

	cont = true;
	while (cont) {
		// draw
		print_clr();
		print_man();
		print_sight();
		
		// interpret
		scanf("%c", &comm);
		switch (comm) {
			case RESET:
				cont = false;
				break;
			case XM:
				if (can_move(XD)) set_x(get_x()-1);
				break;
			case XP:
				if (can_move(XU)) set_x(get_x()+1);
				break;
			case YM:
				if (can_move(YD)) set_y(get_y()-1);
				break;
			case YP:
				if (can_move(YU)) set_y(get_y()+1);
				break;
			case ZM:
				if (can_move(ZD)) set_z(get_z()-1);
				break;
			case ZP:
				if (can_move(ZU)) set_z(get_z()+1);
				break;
			case WM:
				if (can_move(WD)) set_w(get_w()-1);
				break;
			case WP:
				if (can_move(WU)) set_w(get_w()+1);
				break;
			case DSWAPX:
				d_swap_abs(DIMX, DIMX);				
				break;
			case DSWAPY:
				d_swap_abs(DIMX, DIMY);				
				break;
			case DSWAPZ:
				d_swap_abs(DIMX, DIMZ);				
				break;
			case DSWAPW:
				d_swap_abs(DIMX, DIMW);				
				break;
			default:
				break;
		}
		
		if (get_flag(F_GOAL)) {
			// draw
			print_clr();
			print_man();
			print_sight();
			
			// victory!
			printf("YOU WON!\n\a");

			// delete remaining commands
			while (getchar() != '\n');
			// pause
			do {
				comm = getchar(); 
			} while (comm != '\n');
			
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////
////////////////           GRAPHICS             ////////////////
////////////////////////////////////////////////////////////////

/*
 * Print based on sight algorithm.
 */
void engine_c::print_sight (void)
{
	discover();
	print_disc();
}

/*
 * Clear screen.
 */
void engine_c::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

/*
 * Title screen.
 */
void engine_c::print_title_screen (void)
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
void engine_c::print_man (void)
{
	unsigned int a0s = get_a0();
	unsigned int a1s = get_a1();
	unsigned int a2s = get_a2();
	unsigned int a3s = get_a3();
	
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
		"  \n",
		alet[a2s],
		alet[a0s], alet[a0s], 
		alet[a1s], alet[a1s], 
		alet[a0s], alet[a0s], 
		alet[a1s], alet[a1s], 
		alet[a3s]);
	/*
	printf(
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
		XP, XM, YP, YM, ZP, ZM, WP, WM, RESET, 
		DSWAPX, DSWAPX, DSWAPY, DSWAPZ, DSWAPW, 
		USER, GOAL);
	*/
	printf("You are %s.\n", USER);
	printf("Your goal is to get to %s.\n", GOAL);
	printf("\n");
	printf("%s is walls.\n", WALL);
	printf("%s is empty space.\n", SPACE);
	printf("%s is unknown territory.\n", UNDISC);
	printf("\n");
	printf("To move, type %c, %c, %c, %c, %c, %c, %c, %c.\n", XM, XP, YM, YP, ZM, ZP, WM, WP);
	printf("To give up, type %c.\n", RESET);
	printf("To rotate your view, type %c, %c, %c, %c.\n", DSWAPX, DSWAPY, DSWAPZ, DSWAPW);
	printf("You must hit enter to execute a command.\n");
	printf("\n");
}

/*
    ::YD::  ::::::  
    XD  XU  ::ZU::  
    ::YU::  ::::::  
                    
    ::::::  ......  
    ::WU::  ......  
    ::::::  ......  
                    
*/ 
void engine_c::print_all (void)
{
	// debug
	//printf("address %0X\nlenx %i\nleny %i\nlenz %i\nlenw %i\n", arry, lenx, leny, lenz, lenw);
	//printf("a1 = %i, a2 = %i, a3 = %i, a4 = %i\n", a1, a2,a3, a4);
	unsigned int aint[] = {0, 0, 0, 0};
	const node_t awd[] = {XD, YD, ZD, WD};
	const node_t awu[] = {XU, YU, ZU, WU};
	const unsigned int alen[] = {get_lenx(), get_leny(), get_lenz(), get_lenw()};
	
	for (aint[get_a3()]=0; aint[get_a3()]<alen[get_a3()]; aint[get_a3()]++) {
		for (aint[get_a1()]=0; aint[get_a1()]<alen[get_a1()]; aint[get_a1()]++) {
			
			for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 1
				// ROW 1
				// COL 1
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
					// BLOCK 1
					printf("%s", WALL);
					// BLOCK 2
					printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awd[get_a1()]) ? WALL : SPACE);
				}
				// BLOCK 3
				printf("%s", WALL);
				
				// SECTION 1
				// ROW 1
				// COL 2
				if (aint[get_a2()]<alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
			
			for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 1
				// ROW 2
				// COL 1
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
					// BLOCK 1
					printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awd[get_a0()]) ? WALL : SPACE);
					// BLOCK 2
					// USER/GOAL/ETC
					if (get_x() == aint[0] && get_y() == aint[1] && get_z() == aint[2] && get_w() == aint[3]) {
						printf("%s", USER);
					} else if (get_flag(aint[0],aint[1],aint[2],aint[3],F_GOAL)) {
						printf("%s", GOAL);
					} else {
						printf("%s", SPACE);
					}
				}
				aint[get_a0()] = alen[get_a0()]-1;
				// BLOCK 3
				printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a0()]) ? WALL : SPACE);
				
				// SECTION 1
				// ROW 2
				// COL 2
				if (aint[get_a2()]<alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a2()]) ? WALL : SPACE);
					}
					// BLOCK 3
					printf("%s", WALL);
					printf("%s", HSKIP);
				}
			}
			printf("\n");
		}
		
		aint[get_a1()] = alen[get_a1()]-1;
		for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
			// SECTION 1
			// ROW 3
			// COL 1
			for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
				// BLOCK 1
				printf("%s", WALL);
				// BLOCK 2
				printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a1()]) ? WALL : SPACE);
			}
			// BLOCK 3
			printf("%s", WALL);
				
			// SECTION 1
			// ROW 3
			// COL 2
			if (aint[get_a2()]<alen[get_a2()]-1) {
				printf("%s", HSKIP);
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
		
		if (aint[get_a3()]<alen[get_a3()]-1) {
			printf("%s", VSKIP);
			
			for (aint[get_a1()]=0; aint[get_a1()]<alen[get_a1()]; aint[get_a1()]++) {
				for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
					// SECTION 2
					// ROW 1
					// COL 1
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
					if (aint[get_a2()]<alen[get_a2()]-1) {
						printf("%s", HSKIP);
						for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
				
				for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
					// SECTION 2
					// ROW 2
					// COL 1
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
						// BLOCK 1
						printf("%s", WALL);
						// BLOCK 2
						printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a3()]) ? WALL : SPACE);
					}
					// BLOCK 3
					printf("%s", WALL);
					
					// SECTION 2
					// ROW 2
					// COL 2
					if (aint[get_a2()]<alen[get_a2()]-1) {
						printf("%s", HSKIP);
						for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
			for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 2
				// ROW 3
				// COL 1
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
				if (aint[get_a2()]<alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
bool engine_c::disc_s1r1c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	}
	
	aint[get_a0()]++;
	return false;
}

bool engine_c::disc_s1r1c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	return false;
}

bool engine_c::disc_s1r1c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	}
	
	aint[get_a1()]++;
	return false;
}

bool engine_c::disc_s1r2c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 

	aint[get_a0()]++;
	return false;
}

bool engine_c::disc_s1r2c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool engine_c::disc_s1r2c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool engine_c::disc_s1r3c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a0()]++;
	return false;
}

bool engine_c::disc_s1r3c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool engine_c::disc_s1r3c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	return false;
}

bool engine_c::disc_s1r1c2b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	}
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a2()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	}
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r1c2b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	}
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r1c2b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a0()]--;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		return true;
	}
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a2()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	}
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r2c2b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r2c2b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r2c2b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r3c2b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a2()]--;
		return true;
	} 

	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	} 
	
	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r3c2b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	} 

	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s1r3c2b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	}
	
	aint[get_a2()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a2()]--;
		return true;
	}

	aint[get_a2()]--;
	return false;
}

bool engine_c::disc_s2r1c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	}
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a3()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	}
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r1c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	}
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r1c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a1()]++;
		aint[get_a0()]--;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		return true;
	}
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a1()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a3()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		aint[get_a1()]++;
		return true;
	} 
	
	aint[get_a1()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	}
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r2c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r2c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r2c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]--;
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r3c1b1 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a0()]--;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a0()]++;
		aint[get_a3()]--;
		return true;
	}

	aint[get_a0()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	} 
	
	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r3c1b2 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	} 
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	} 

	aint[get_a3()]--;
	return false;
}

bool engine_c::disc_s2r3c1b3 (unsigned int aint[4])
{
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		return true;
	}
	
	aint[get_a3()]++;
	if ( get_flag(aint[0],aint[1],aint[2],aint[3],F_DISC) ) {
		aint[get_a3()]--;
		return true;
	}

	aint[get_a3()]--;
	return false;
}

void engine_c::print_disc (void)
{
	// debug
	//printf("address %0X\nlenx %i\nleny %i\nlenz %i\nlenw %i\n", arry, lenx, leny, lenz, lenw);
	//printf("a1 = %i, a2 = %i, a3 = %i, a4 = %i\n", a1, a2,a3, a4);
	unsigned int aint[] = {0, 0, 0, 0};
	const node_t awd[] = {XD, YD, ZD, WD};
	const node_t awu[] = {XU, YU, ZU, WU};
	const unsigned int alen[] = {get_lenx(), get_leny(), get_lenz(), get_lenw()};
	
	for (aint[get_a3()] = 0; aint[get_a3()] < alen[get_a3()]; aint[get_a3()]++) {
		for (aint[get_a1()] = 0; aint[get_a1()] < alen[get_a1()]; aint[get_a1()]++) {
			
			for (aint[get_a2()] = 0; aint[get_a2()] < alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 1
				// ROW 1
				// COL 1
				for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
					// BLOCK 1
					if ( disc_s1r1c1b1(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					if ( disc_s1r1c1b2(aint) ) {
						printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awd[get_a1()]) ? WALL : SPACE);
					} else {
						printf("%s", UNDISC);
					}
				}
				// BLOCK 3
				aint[get_a0()] = alen[get_a0()]-1;
				if ( disc_s1r1c1b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 1
				// ROW 1
				// COL 2
				if (aint[get_a2()]<alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
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
					aint[get_a0()] = alen[get_a0()]-1;
					if ( disc_s1r1c2b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					printf("%s", HSKIP);
				}
			}
			printf("\n");
			
			for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 1
				// ROW 2
				// COL 1
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
					// BLOCK 1
					if ( disc_s1r2c1b1(aint) ) {
						printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awd[get_a0()]) ? WALL : SPACE);
					} else {
						printf("%s", UNDISC);
					}
					// BLOCK 2
					// USER/GOAL/ETC
					if ( !disc_s1r2c1b2(aint) ) {
						printf("%s", UNDISC);
					} else if (get_x() == aint[0] && get_y() == aint[1] && get_z() == aint[2] && get_w() == aint[3]) {
						printf("%s", USER);
					} else if (get_flag(aint[0],aint[1],aint[2],aint[3],F_GOAL)) {
						printf("%s", GOAL);
					} else {
						printf("%s", SPACE);
					}
				}
				aint[get_a0()] = alen[get_a0()]-1;
				// BLOCK 3
				if ( disc_s1r2c1b3(aint) ) {
					printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a0()]) ? WALL : SPACE);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 1
				// ROW 2
				// COL 2
				if (aint[get_a2()]<alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
						// BLOCK 1
						if ( disc_s1r2c2b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s1r2c2b2(aint) ) {
							printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a2()]) ? WALL : SPACE);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[get_a0()] = alen[get_a0()]-1;
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
		
		aint[get_a1()] = alen[get_a1()]-1;
		for (aint[get_a2()] = 0; aint[get_a2()] < alen[get_a2()]; aint[get_a2()]++) {
			// SECTION 1
			// ROW 3
			// COL 1
			for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
				// BLOCK 1
				if ( disc_s1r3c1b1(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				// BLOCK 2
				if ( disc_s1r3c1b2(aint) ) {
					printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a1()]) ? WALL : SPACE);
				} else {
					printf("%s", UNDISC);
				}
			}
			// BLOCK 3
			aint[get_a0()] = alen[get_a0()]-1;
			if ( disc_s1r3c1b3(aint) ) {
				printf("%s", WALL);
			} else {
				printf("%s", UNDISC);
			}
			
			// SECTION 1
			// ROW 3
			// COL 2
			if (aint[get_a2()]<alen[get_a2()]-1) {
				printf("%s", HSKIP);
				for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
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
				aint[get_a0()] = alen[get_a0()]-1;
				if ( disc_s1r3c2b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				printf("%s", HSKIP);
			}
		}
		printf("\n");
		
		if (aint[get_a3()] < alen[get_a3()]-1) {
			printf("%s", VSKIP);
			
			for (aint[get_a1()]=0; aint[get_a1()]<alen[get_a1()]; aint[get_a1()]++) {
				for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
					// SECTION 2
					// ROW 1
					// COL 1
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
					aint[get_a0()] = alen[get_a0()]-1;
					if ( disc_s2r1c1b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					
					// SECTION 2
					// ROW 1
					// COL 2
					if (aint[get_a2()]<alen[get_a2()]-1) {
						printf("%s", HSKIP);
						for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
				
				for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
					// SECTION 2
					// ROW 2
					// COL 1
					for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
						// BLOCK 1
						if ( disc_s2r2c1b1(aint) ) {
							printf("%s", WALL);
						} else {
							printf("%s", UNDISC);
						}
						// BLOCK 2
						if ( disc_s2r2c1b2(aint) ) {
							printf("%s", get_flag(aint[0],aint[1],aint[2],aint[3],awu[get_a3()]) ? WALL : SPACE);
						} else {
							printf("%s", UNDISC);
						}
					}
					// BLOCK 3
					aint[get_a0()] = alen[get_a0()]-1;
					if ( disc_s2r2c1b3(aint) ) {
						printf("%s", WALL);
					} else {
						printf("%s", UNDISC);
					}
					
					// SECTION 2
					// ROW 2
					// COL 2
					if (aint[get_a2()]<alen[get_a2()]-1) {
						printf("%s", HSKIP);
						for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
							// BLOCK 1
							printf("%s", SPACE);
							// BLOCK 2
							printf("%s", SPACE);
						}
						// BLOCK 3
						aint[get_a0()] = alen[get_a0()]-1;
						printf("%s", SPACE);
						printf("%s", HSKIP);
					}
				}
				printf("\n");
			}
			aint[get_a1()] = alen[get_a1()]-1;
			for (aint[get_a2()]=0; aint[get_a2()]<alen[get_a2()]; aint[get_a2()]++) {
				// SECTION 2
				// ROW 3
				// COL 1
				for (aint[get_a0()]=0; aint[get_a0()]<alen[get_a0()]; aint[get_a0()]++) {
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
				aint[get_a0()] = alen[get_a0()]-1;
				if ( disc_s2r3c1b3(aint) ) {
					printf("%s", WALL);
				} else {
					printf("%s", UNDISC);
				}
				
				// SECTION 2
				// ROW 3
				// COL 2
				if (aint[get_a2()] < alen[get_a2()]-1) {
					printf("%s", HSKIP);
					for (aint[get_a0()] = 0; aint[get_a0()] < alen[get_a0()]; aint[get_a0()]++) {
						// BLOCK 1
						printf("%s", SPACE);
						// BLOCK 2
						printf("%s", SPACE);
					}
					// BLOCK 3
					aint[get_a0()] = alen[get_a0()]-1;
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
void engine_c::print_data (void)
{
	for (unsigned int h = 0; h < get_lenw(); h++) {
		for (unsigned int j = 0; j < get_leny(); j++) {
			for (unsigned int k = 0; k < get_lenz(); k++) {
				for (unsigned int i = 0; i < get_lenx(); i++) {
					//printf("%08X  ", arry[i][j][k][h]);
				}
				if (k < get_lenz()-1) {
					printf("  ");
				}
			}
			printf("\n");
		}
		printf("\n");
	}
}
