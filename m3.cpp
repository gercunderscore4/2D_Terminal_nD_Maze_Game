////////////////////////////////
// BEG M3.CPP
////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "mn.hpp"
#include "m3.hpp"

void m3::play (void)
{
	while (true) {
		//lenx=3,leny=4,lenz=2; // for testing
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

m3::m3 (void) {
	// 
	lenx=0, leny=0, lenz=0;	
	arry=NULL;
	x=0, y=0, z=0;
	gx=0, gy=0, gz=0;
}

m3::~m3 (void) {
	degen();
}

////////////////////////////////
//        allocation          //
////////////////////////////////

void m3::gen (void)
{
	degen();
	// keep size within bounds
	if (lenx < SIZE_MIN) lenx = SIZE_MIN;
	if (lenx > SIZE_MAX) lenx = SIZE_MAX;
	if (leny < SIZE_MIN) leny = SIZE_MIN;
	if (leny > SIZE_MAX) leny = SIZE_MAX;
	if (lenz < SIZE_MIN) lenz = SIZE_MIN;
	if (lenz > SIZE_MAX) lenz = SIZE_MAX;
	// allocate
	arry = (char***) calloc(lenx+1,sizeof(char**));
	for (int i = 0; i < lenx+1; i++) {
		arry[i] = (char**) calloc(leny+1,sizeof(char*));
		for (int j = 0; j < leny+1; j++) {
			arry[i][j] = (char*) calloc(lenz+1,sizeof(char));
		}
	}
}

void m3::degen (void)
{
	if (arry != NULL) {
		for (int i = 0; i < lenx+1; i++) {
			for (int j = 0; j < leny+1; j++) {
				free(arry[i][j]);
			}
			free(arry[i]);
		}
		free(arry);
		arry = NULL;
	
		lenx = 0;
		leny = 0;
		lenz = 0;
	}	
}

////////////////////////////////
//           boxes            //
////////////////////////////////

void m3::frame (void)
{
	int i=0, j=0, k=0;
	// make outer walls
	/*
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			for (k = 1; k < lenz; k++) {
	*/
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] |= XD;
		}
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] |= YD;
		}
	}
	// ZD
	k = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j][k] |= ZD;
		}
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = XD;
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = YD;
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j][k] = ZD;
		}
	}
}

void m3::box (void)
{
	int i=0, j=0, k=0;
	// make outer walls
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			for (k = 1; k < lenz; k++) {
				arry[i][j][k] = 0;
			}
		}
	}
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] |= XD;
		}
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] |= YD;
		}
	}
	// ZD
	k = 0;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j][k] |= ZD;
		}
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = XD;
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = YD;
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j][k] = ZD;
		}
	}
}

void m3::cage (void)
{
	int i=0, j=0, k=0;
	// build walls
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			for (k = 0; k < lenz; k++) {
				arry[i][j][k] = XD;
				arry[i][j][k] |= YD;
				arry[i][j][k] |= ZD;
			}
		}
	}
	
	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = XD;
		}
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		for (k = 0; k < lenz; k++) {
			arry[i][j][k] = YD;
		}
	}
	// ZU
	k = lenz;
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j][k] = ZD;
		}
	}
}

void m3::empty (void)
{
	int i=0, j=0, k=0;
	// make outer walls
	for (i = 0; i < lenx+1; i++) {
		for (j = 0; j < leny+1; j++) {
			for (k = 0; k < lenz+1; k++) {
				arry[i][j][k] = 0;
			}
		}
	}
}

////////////////////////////////
//          print             //
////////////////////////////////

void m3::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

void m3::print_man (void)
{
	printf(
		"  axes: \n"
		"  +---------------------Z \n"
		"                          \n"
		"     +----X     +----X    \n"
		"     |          |         \n"
		"     |          |         \n"
		"     Y          Y         \n"
		"  \n"
		"  controls: \n"
		"  X+   %c \n"
		"  X-   %c \n"
		"  Y+   %c \n"
		"  Y-   %c \n"
		"  Z+   %c \n"
		"  Z-   %c \n"
		"  new  ` \n"
		"  type it then hit enter \n"
		"  \n"
		"  USER: %s \n"
		"  GOAL: %s \n"
		"  \n",
		XP, XM, YP, YM, ZP, ZM, USER, GOAL);
}

void m3::print_all (void)
{
	// debug
	//printf("print_m2, %0X, %i, %i\n", arry, lenx, leny);		
			
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
						printf("%s", arry[i][j][k]&YD ? WALL : SPACE);
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
								printf("%s", arry[i][j][k]&ZD ? WALL : SPACE);
							}
						}
						// HSKIP
						printf("%s", HSKIP);
					}

					// halls XD to the west
					// X
					for (int i = 0; i < lenx+1; i++) {
						printf("%s", arry[i][j][k]&XD ? WALL : SPACE);
						if (i != lenx) {
							// this is on one important space
							// user
							if (i == x && j == y && k == z) {
								printf("%s", USER);
							}
							// goal
							else if (has_flag(i,j,k,F_GOAL)) {
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

////////////////////////////////
//         control            //
////////////////////////////////

void m3::get_size (void)
{
	// get size
	printf("x size (<=%i):\n", SIZE_MAX);
	scanf("%i", &lenx);
	printf("y size (<=%i):\n", SIZE_MAX);
	scanf("%i", &leny);
	printf("z size (<=%i):\n", SIZE_MAX);
	scanf("%i", &lenz);
}

bool m3::control (void)
{
	//x = 0, y = 0, z = 0;
	
	bool cont = true;
	char comm = ' ';
	while (cont) {
		print_clr();
		print_man();
		print_all();
		// debug
		//printf ("%i %i %i\n", x, y, z);
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

bool inline m3::valid (void)
{
	return 0 <= x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz ? true : false; 
}

bool inline m3::valid (int i, int j, int k)
{
	return 0 <= i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz ? true : false; 
}

void inline m3::set_flag (char flag)
{
	if (valid(x,y,z)) arry[x][y][z] |= flag;
}

void inline m3::set_flag (int i, int j, int k, char flag)
{
	if (valid(i,j,k)) arry[i][j][k] |= flag;
}

void inline m3::clear_flag (char flag)
{
	if (valid(x,y,z)) arry[x][y][z] &= ~flag;
}

void inline m3::clear_flag (int i, int j, int k, char flag)
{
	if (valid(i,j,k)) arry[i][j][k] &= ~flag;
}

void inline m3::clear_flag_all (char flag)
{
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				clear_flag(i,j,k,flag);
			}
		}
	}
}

bool inline m3::has_flag (char flag)
{
	return valid(x,y,z) && arry[x][y][z]&flag ? true : false;
}

bool inline m3::has_flag (int i, int j, int k, char flag)
{
	return valid(i,j,k) && arry[i][j][k]&flag ? true : false;
}

bool inline m3::can_move(char dir)
{
	switch(dir) {
		case XM:
			return 0 < x && !(arry[x][y][z]&XD) ? true : false;
		case XP:
			return x < lenx && !(arry[x+1][y][z]&XD) ? true : false;
		case YM:
			return 0 < y && !(arry[x][y][z]&YD) ? true : false;
		case YP:
			return y < leny && !(arry[x][y+1][z]&YD) ? true : false;
		case ZM:
			return 0 < z && !(arry[x][y][z]&ZD) ? true : false;
		case ZP:
			return z < lenz && !(arry[x][y][z+1]&ZD) ? true : false;
		default:
			return false;
	}
}

bool inline m3::can_move(int i, int j, int k, char dir)
{
	switch(dir) {
		case XM:
			return 0 < i && !(arry[i][j][k]&XD) ? true : false;
		case XP:
			return i < lenx && !(arry[i+1][j][k]&XD) ? true : false;
		case YM:
			return 0 < j && !(arry[i][j][k]&YD) ? true : false;
		case YP:
			return j < leny && !(arry[i][j+1][k]&YD) ? true : false;
		case ZM:
			return 0 < k && !(arry[i][j][k]&ZD) ? true : false;
		case ZP:
			return k < lenz && !(arry[i][j][k+1]&ZD) ? true : false;
		default:
			return false;
	}
}

bool m3::smash(char dir) {
	switch(dir) {
		case XM:
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~XD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case XP:
			// XM for x+1
			x++;
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~XD;
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~YD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case YP:
			// YM for y+1
			y++;
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~YD;
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
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~ZD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case ZP:
			// ZM for z+1
			z++;
			if (0 < x && x < lenx && 0 <= y && y < leny && 0 <= z && z < lenz) {
				// smash
				arry[x][y][z] &= ~ZD;
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
		default:
			return false;
	}
}

bool m3::smash(int i, int j, int k, char dir) {
	switch(dir) {
		case XM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~XD;
				return true;
			} else {
				return false;
			}
		case XP:
			// XM for i+1
			i++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~YD;
				return true;
			} else {
				return false;
			}
		case YP:
			// YM for j+1
			j++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~YD;
				return true;
			} else {
				return false;
			}
		case ZM:
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case ZP:
			// ZM for k+1
			k++;
			if (0 < i && i < lenx && 0 <= j && j < leny && 0 <= k && k < lenz) {
				arry[i][j][k] &= ~ZD;
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
int m3::rec_depth_solve (void)
{
	// assume current position is valid

	// goal reached
	if (has_flag(F_GOAL)) {
		clear_flag_all(F_TEMP);
		return 0;
	}

	// mark as visited
	set_flag(x,y,z,F_TEMP);

	// XD
	if (can_move(XM) && !has_flag(x-1,y,z,F_TEMP)) {
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
	if (can_move(XP) && !has_flag(x+1,y,z,F_TEMP)) {
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
	if (can_move(YM) && !has_flag(x,y-1,z,F_TEMP)) {
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
	if (can_move(YP) && !has_flag(x,y+1,z,F_TEMP)) {
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
	if (can_move(ZM) && !has_flag(x,y,z-1,F_TEMP)) {
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
	if (can_move(ZP) && !has_flag(x,y,z+1,F_TEMP)) {
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
	
	// failed
	return -1;
}

int m3::rec_breadth_solve (void)
{
	return -1;
}

////////////////////////////////
//     maze algorithms        //
////////////////////////////////

void m3::random_build (void)
{
	// completely random
	empty();
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			for (int k = 0; k < lenz; k++) {
				if (rand() > RAND_MAX/2) arry[i][j][k] |= XD;
				if (rand() > RAND_MAX/2) arry[i][j][k] |= YD;
				if (rand() > RAND_MAX/2) arry[i][j][k] |= ZD;
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
		//printf("%i %i %i\n%i\n",x,y,z,c);
		//getchar();
	}
	*/

	// start
	x=0, y=0, z=0;
	// goal
	gx=lenx-1, gy=leny-1, gz=lenz-1;
	set_flag(gx,gy,gz,F_GOAL);
}

////////////////////////////////
//        depth-first         //
////////////////////////////////

void m3::depth_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0,y=0,z=0;
	// smashy-smashy
	rec_depth_build();
	gx=lenx-1, gy=leny-1, gz=lenz-1;
	set_flag(gx,gy,gz,F_GOAL);
}

// set to valid position before
void m3::rec_depth_build (void)
{
	// assume current position is valid

	// mark as visited
	set_flag(x,y,z,F_TEMP);
	
	//print_all();
	//printf("%i %i %i\n",x,y,z);
	//getchar();
	
	char dirs[] = {XM,XP,YM,YP,ZM,ZP};
	int ind = 0;
	char dir = 0;
	for (int n = DIRS3; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// XD
		// if node is available and wall can be smashed
		if (dir==XM && valid(x-1,y,z) && !has_flag(x-1,y,z,F_TEMP) && smash(XM)) {
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
		if (dir==XP && valid(x+1,y,z) && !has_flag(x+1,y,z,F_TEMP) && smash(XP)) {
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
		if (dir==YM && valid(x,y-1,z) && !has_flag(x,y-1,z,F_TEMP) && smash(YM)) {
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
		if (dir==YP && valid(x,y+1,z) && !has_flag(x,y+1,z,F_TEMP) && smash(YP)) {
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
		if (dir==ZM && valid(x,y,z-1) && !has_flag(x,y,z-1,F_TEMP) && smash(ZM)) {
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
		if (dir==ZP && valid(x,y,z+1) && !has_flag(x,y,z+1,F_TEMP) && smash(ZP)) {
			// move forward
			z++;
			// recusrive call
			//printf("ZU\n");
			rec_depth_build();
			// move back
			z--;
		}
		
		// swap remaining directions
		dirs[ind] = dirs[n-1];
	}
		
	// failed
	return;
}

////////////////////////////////
// END M3.CPP
////////////////////////////////
