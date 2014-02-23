////////////////////////////////
// BEG M4.CPP
////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

#include "mn.hpp"
#include "m4.hpp"

void m4::play (void)
{
	while (true) {
		// clear screen
		print_clr();
		
		// size
		get_size();
		
		// allocate
		gen();
		
		// build, choose one
		//random_build();
		depth_build();
		//breadth_build();
		
		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		//getchar();
		
		// play
		control();
		
		// deallocate
		degen();
	}
}

void m4::play (int xs, int ys, int zs, int ws)
{
	while (true) {
		// clear screen
		print_clr();
		
		// size
		lenx=xs, leny=ys, lenz=zs, lenw=ws; // for testing
		
		// allocate
		gen();
		
		// build, choose one
		random_build();
		//depth_build();
		//breadth_build();
		
		// solve
		//printf("solving:\n");
		//printf("depth solve: %i\n", rec_depth_solve());
		//printf("breadth solve: %i\n", breadth_solve());
		//getchar();
		
		// play
		control();
		
		// deallocate
		degen();
	}
}

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
		depth_build();
		//breadth_build();
		//
		build_time[i] = (int) time(NULL) - temp;
		
		temp = (int) time(NULL);
		// solve, choose one
		//rec_depth_solve();
		breadth_solve();
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
	
	// deallocate
	degen();	
}

////////////////////////////////
//       constructicons       //
////////////////////////////////

m4::m4 (void)
{
	// all blank
	lenx=0, leny=0, lenz=0, lenw=0;	
	arry=NULL;
	x=0, y=0, z=0, w=0;
	gx=0, gy=0, gz=0, gw=0;
}

m4::~m4 (void)
{
	degen();
}

////////////////////////////////
//        allocation          //
////////////////////////////////

void m4::gen (void)
{
	degen();
	// keep size within bounds
	if (lenx < SIZE_MIN) lenx = SIZE_MIN;
	if (lenx > SIZE_MAX) lenx = SIZE_MAX;
	if (leny < SIZE_MIN) leny = SIZE_MIN;
	if (leny > SIZE_MAX) leny = SIZE_MAX;
	if (lenz < SIZE_MIN) lenz = SIZE_MIN;
	if (lenz > SIZE_MAX) lenz = SIZE_MAX;
	if (lenw < SIZE_MIN) lenw = SIZE_MIN;
	if (lenw > SIZE_MAX) lenw = SIZE_MAX;
	// allocate
	arry = (char****) calloc(lenx+1,sizeof(char***));
	for (int i = 0; i < lenx+1; i++) {
		arry[i] = (char***) calloc(leny+1,sizeof(char**));
		for (int j = 0; j < leny+1; j++) {
			arry[i][j] = (char**) calloc(lenz+1,sizeof(char*));
			for (int k = 0; k < lenz+1; k++) {
				arry[i][j][k] = (char*) calloc(lenw+1,sizeof(char));
			}
		}
	}
}

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
	
		lenx = 0;
		leny = 0;
		lenz = 0;
		lenw = 0;
	}	
}

////////////////////////////////
//           boxes            //
////////////////////////////////

void m4::frame (void)
{
	int i=0, j=0, k=0, h=0;
	// make outer walls
	/*
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			for (k = 1; k < lenz; k++) {
				for (h = 1; h < lenw; h++) {
	*/
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= XD;
			}
		}
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= YD;
			}
		}
	}
	// ZD
	k = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= ZD;
			}
		}
	}
	// WD
	h = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k][h] |= WD;
			}
		}
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = XD;
			}
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = YD;
			}
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = ZD;
			}
		}
	}
	// WU
	h = lenw;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k][h] = WD;
			}
		}
	}
}

void m4::box (void)
{
	int i=0, j=0, k=0, h=0;
	// make outer walls
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			for (k = 1; k < lenz; k++) {
				for (h = 1; h < lenw; h++) {
					arry[i][j][k][h] = 0;
				}
			}
		}
	}
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= XD;
			}
		}
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= YD;
			}
		}
	}
	// ZD
	k = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] |= ZD;
			}
		}
	}
	// WD
	h = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k][h] |= WD;
			}
		}
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = XD;
			}
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = YD;
			}
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = ZD;
			}
		}
	}
	// WU
	h = lenw;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k][h] = WD;
			}
		}
	}
}

void m4::cage (void)
{
	int i=0, j=0, k=0, h=0;
	// build walls
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				for (h = 0; h < lenw; h++) {
					arry[i][j][k][h] = XD;
					arry[i][j][k][h] |= YD;
					arry[i][j][k][h] |= ZD;
					arry[i][j][k][h] |= WD;
				}
			}
		}
	}
	
	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = XD;
			}
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = YD;
			}
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (h = 0; h < lenw; h++) {
				arry[i][j][k][h] = ZD;
			}
		}
	}
	// WU
	h = lenw;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k][h] = WD;
			}
		}
	}	
}

void m4::empty (void)
{
	int i=0, j=0, k=0, h=0;
	// make outer walls
	for (i = 0; i < lenx+1; i++) {
		for (j = 0; j < leny+1; j++) {
			for (k = 0; k < lenz+1; k++) {
				for (h = 0; h < lenw+1; h++) {
					arry[i][j][k][h] = 0;
				}
			}
		}
	}
}

////////////////////////////////
//          print             //
////////////////////////////////

void m4::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

void m4::print_man (void)
{
	printf(
		"  axes: \n"
		"  +---------------------Z \n"
		"  |                       \n"
		"  |  +----X     +----X    \n"
		"  |  |          |         \n"
		"  |  |          |         \n"
		"  |  Y          Y         \n"
		"  |                       \n"
		"  |  +----X     +----X    \n"
		"  |  |          |         \n"
		"  |  |          |         \n"
		"  |  Y          Y         \n"
		"  |                       \n"
		"  W                       \n"
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
		"  new  ` \n"
		"  type it then hit enter \n"
		"  \n"
		"  USER: %s \n"
		"  GOAL: %s \n"
		"  \n",
		XP, XM, YP, YM, ZP, ZM, WP, WM, USER, GOAL);
}

void m4::print_all (void)
{
	// debug
	//printf("print_m2, %0X, %i, %i\n", arry, lenx, leny);
	// W
	for (int h = 0; h < lenw; h++) {
		
		// Y
		if (h != 0) {
			for (int j = 0; j < leny+1; j++) {
	
				// Z
				for (int k = 0; k < lenz; k++) {
					if (k != 0) {
						// nothing
						// X
						for (int i = 0; i < lenx+1; i++) {
							printf("%s", SPACE);
							if (i != lenx) {
								printf("%s", SPACE);
							}
						}
						// HSKIP	
						printf("%s", HSKIP);			
					}
					
					// teeth to the east
					if (k != lenz) {
						// floor WD
						// X
						for (int i = 0; i < lenx+1; i++) {
							printf("%s", WALL);
							if (i != lenx) {			
								printf("%s", WALL);
							}
						}
						// HSKIP
						printf("%s", HSKIP);
					}
				}
				printf("\n");
			
				// no teeth to the south
				if (j != leny) {
					// Z
					for (int k = 0; k < lenz; k++) {
						if (k != 0) {
							// nothing
							// X
							for (int i = 0; i < lenx+1; i++) {
								printf("%s", SPACE);
								if (i != lenx) {
									printf("%s", SPACE);
								}
							}
							// HSKIP
							printf("%s", HSKIP);
						}
						
						// floor WD to the north
						// X
						for (int i = 0; i < lenx+1; i++) {
							printf("%s", WALL);
							if (i != lenx) {
								printf("%s", arry[i][j][k][h]&WD ? WALL : SPACE);
							}
						}
						// HSKIP
						printf("%s", HSKIP);
					}
				}
				printf("\n");	
			}
			// VSKIP
		}
			
		// Y
		for (int j = 0; j < leny+1; j++) {	
	
			// Z
			for (int k = 0; k < lenz; k++) {
				if (k != 0) {
					// floor
					// X
					for (int i = 0; i < lenx+1; i++) {
						printf("%s", WALL);
						if (i != lenx) {
							printf("%s", WALL);
						}
					}
					// HSKIP
					printf("%s", HSKIP);
				}
				
				// halls YD to the north
				// X
				for (int i = 0; i < lenx+1; i++) {
					printf("%s", WALL);
					if (i != lenx) {
						printf("%s", arry[i][j][k][h]&YD ? WALL : SPACE);
					}
				}
				// HSKIP
				printf("%s", HSKIP);
			}
			printf("\n");
	
			if (j != leny) {
				// Z
				for (int k = 0; k < lenz; k++) {
					if (k != 0) {
						// X
						for (int i = 0; i < lenx+1; i++) {
							printf("%s", WALL);
							if (i != lenx) {
								printf("%s", arry[i][j][k][h]&ZD ? WALL : SPACE);
							}
						}
						// HSKIP
						printf("%s", HSKIP);
					}

					// halls XD to the west
					// X
					for (int i = 0; i < lenx+1; i++) {
						printf("%s", arry[i][j][k][h]&XD ? WALL : SPACE);
						if (i != lenx) {
							// this is on one important space
							// user
							if (i == x && j == y && k == z && h == w) {
								printf("%s", USER);
							}
							// goal
							else if (has_flag(i,j,k,h,F_GOAL)) {
								printf("%s", GOAL);
							}
							// temp
							else if (has_flag(i,j,k,h,F_TEMP)) {
								printf("%s", TEMP);
							}
							else {
								printf("%s", SPACE);
							}
						}
					}
					// HSKIP
					printf("%s", HSKIP);
				}
			}
			printf("\n");		
		}
		// VSKIP
	}
	
}

////////////////////////////////
//         control            //
////////////////////////////////

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
		printf("x size (<=%i):\n", SIZE_MAX);
		scanf("%i", &lenx);
		if (n >= 2) {
			printf("y size (<=%i):\n", SIZE_MAX);
			scanf("%i", &leny);
			if (n >= 3) {
				printf("z size (<=%i):\n", SIZE_MAX);
				scanf("%i", &lenz);
				if (n >= 4) {
					printf("w size (<=%i):\n", SIZE_MAX);
					scanf("%i", &lenw);
				}
			}
		}
	}
}

bool m4::control (void)
{
	x = 0, y = 0, z = 0, w = 0;
	
	bool cont = true;
	char comm = ' ';
	while (cont) {
		print_clr();
		print_man();
		print_all();
		// debug
		//printf ("%i %i %i %i\n", x, y, z, w);
		scanf("%c", &comm);
		if (comm == RESET) {
			cont = false;
		}
		else if (comm == XM && can_move(comm)) {
			x--;
		}
		else if (comm == XP && can_move(comm)) {
			x++;
		}
		else if (comm == YM && can_move(comm)) {
			y--;
		}
		else if (comm == YP && can_move(comm)) {
			y++;
		}
		else if (comm == ZM && can_move(comm)) {
			z--;
		}
		else if (comm == ZP && can_move(comm)) {
			z++;
		}
		else if (comm == WM && can_move(comm)) {
			w--;
		}
		else if (comm == WP && can_move(comm)) {
			w++;
		}
		
		if (has_flag(F_GOAL)) {
			print_clr();
			print_man();
			print_all();
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

////////////////////////////////
//          checks            //
////////////////////////////////

bool inline m4::valid (void)
{
	return 0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw ? true : false; 
}

bool inline m4::valid (int i, int j, int k, int h)
{
	return 0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw ? true : false; 
}

void inline m4::set_flag (char flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] |= flag;
}

void inline m4::set_flag (int i, int j, int k, int h, char flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] |= flag;
}

void inline m4::clear_flag (char flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] &= ~flag;
}

void inline m4::clear_flag (int i, int j, int k, int h, char flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] &= ~flag;
}

void inline m4::clear_flag_all (char flag)
{
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				for (int h = 0; h < lenw; h++) {
					clear_flag(i,j,k,h,flag);
				}
			}
		}
	}
}

bool inline m4::has_flag (char flag)
{
	return valid(x,y,z,w) && arry[x][y][z][w]&flag ? true : false;
}

bool inline m4::has_flag (int i, int j, int k, int h, char flag)
{
	return valid(i,j,k,h) && arry[i][j][k][h]&flag ? true : false;
}

bool inline m4::can_move(char dir)
{
	switch(dir) {
		case XM:
			return 0 < x && !(arry[x][y][z][w]&XD) ? true : false;
		case XP:
			return x < lenx && !(arry[x+1][y][z][w]&XD) ? true : false;
		case YM:
			return 0 < y && !(arry[x][y][z][w]&YD) ? true : false;
		case YP:
			return y < leny && !(arry[x][y+1][z][w]&YD) ? true : false;
		case ZM:
			return 0 < z && !(arry[x][y][z][w]&ZD) ? true : false;
		case ZP:
			return z < lenz && !(arry[x][y][z+1][w]&ZD) ? true : false;
		case WM:
			return 0 < w && !(arry[x][y][z][w]&WD) ? true : false;
		case WP:
			return w < lenw && !(arry[x][y][z][w+1]&WD) ? true : false;
		default:
			return false;
	}
}

bool inline m4::can_move(int i, int j, int k, int h, char dir)
{
	switch(dir) {
		case XM:
			return 0 < i && !(arry[i][j][k][h]&XD) ? true : false;
		case XP:
			return i < lenx && !(arry[i+1][j][k][h]&XD) ? true : false;
		case YM:
			return 0 < j && !(arry[i][j][k][h]&YD) ? true : false;
		case YP:
			return j < leny && !(arry[i][j+1][k][h]&YD) ? true : false;
		case ZM:
			return 0 < k && !(arry[i][j][k][h]&ZD) ? true : false;
		case ZP:
			return k < lenz && !(arry[i][j][k+1][h]&ZD) ? true : false;
		case WM:
			return 0 < h && !(arry[i][j][k][h]&WD) ? true : false;
		case WP:
			return h < lenw && !(arry[i][j][k][h+1]&WD) ? true : false;
		default:
			return false;
	}
}

bool m4::smash(char dir) {
	switch(dir) {
		case XM:
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~XD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case XP:
			// XM for x+1
			x++;
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~XD;
				// return to x
				x--;
				// did smash
				return true;
			} else {
				// return to x
				x--;
				// did not smash
				return false;
			}
		case YM:
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~YD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case YP:
			// YM for y+1
			y++;
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~YD;
				// return to y
				y--;
				// did smash
				return true;
			} else {
				// return to y
				y--;
				// did not smash
				return false;
			}
		case ZM:
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~ZD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case ZP:
			// ZM for z+1
			z++;
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~ZD;
				// return to z
				z--;
				// did smash
				return true;
			} else {
				// return to z
				z--;
				// did not smash
				return false;
			}
		case WM:
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~WD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case WP:
			// WM for w+1
			w++;
			if (0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
				// smash
				arry[x][y][z][w] &= ~WD;
				// return to w
				w--;
				// did smash
				return true;
			} else {
				// return to w
				w--;
				// did not smash
				return false;
			}
		default:
			return false;
	}
}

bool m4::smash(int i, int j, int k, int h, char dir) {
	switch(dir) {
		case XM:
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case XP:
			// XM for i+1
			i++;
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YM:
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case YP:
			// YM for j+1
			j++;
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case ZM:
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case ZP:
			// ZM for k+1
			k++;
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case WM:
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~WD;
				return true;
			} else {
				return false;
			}
		case WP:
			// WM for h+1
			h++;
			if (0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~WD;
				return true;
			} else {
				return false;
			}
		default:
			return false;
	}
}

////////////////////////////////
//         solvers            //
////////////////////////////////

// set to valid position before and after
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

////////////////////////////////
//     maze algorithms        //
////////////////////////////////

void m4::random_build (void)
{
	//int c = -1;
	//while (c < 0) {
		// completely random
		empty();
		for (int i = 0; i < lenx; i++) {
			for (int j = 0; j < leny; j++) {
				for (int k = 0; k < lenz; k++) {
					for (int h = 0; h < lenw; h++) {
						if (rand() > RAND_MAX/2) arry[i][j][k][h] |= XD;
						if (rand() > RAND_MAX/2) arry[i][j][k][h] |= YD;
						if (rand() > RAND_MAX/2) arry[i][j][k][h] |= ZD;
						if (rand() > RAND_MAX/2) arry[i][j][k][h] |= WD;
					}
				}
			}
		}
		frame();

		//c = breadth_solve();
		//c = rec_depth_solve();
	//}

	// start
	x=0, y=0, z=0, w=0;
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
		x = 0XFF & (nodes[r].node>>24);
		y = 0XFF & (nodes[r].node>>16);
		z = 0XFF & (nodes[r].node>>8);
		w = 0XFF & (nodes[r].node);
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
// END M4.CPP
////////////////////////////////
