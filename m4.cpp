////////////////////////////////
// BEG M4.CPP
////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "mn.hpp"
#include "m4.hpp"

void m4::play (void)
{
	while (true) {
		print_clr();
		//lenx=3,leny=4,lenz=2,lenw=2; // for testing
		get_size();
		gen();
		//random_build();
		depth_build();
		control();
		degen();
	}
}

////////////////////////////////
//       constructicons       //
////////////////////////////////

m4::m4 (void)
{
	// 
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
		if (comm == '`') {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz && 0 <= w && w < lenw) {
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
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case XP:
			// XM for i+1
			i++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case YP:
			// YM for j+1
			j++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case ZM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case ZP:
			// ZM for k+1
			k++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case WM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
				arry[i][j][k][h] &= ~WD;
				return true;
			} else {
				return false;
			}
		case WP:
			// WM for h+1
			h++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz && 0 <= h && h < lenw) {
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

int m4::rec_breadth_solve (void)
{
	return -1;
}

////////////////////////////////
//     maze algorithms        //
////////////////////////////////

void m4::random_build (void)
{
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

	// when you get a breadth-first solver, implement this
	/*
	int c = -1;
	while (c < 0) {
		random_build();
		c = rec_depth_solve();
		//print_all();
		//printf("%i %i %i %i\n%i\n",x,y,z,w,c);
		//getchar();
	}
	*/

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
// END M4.CPP
////////////////////////////////
