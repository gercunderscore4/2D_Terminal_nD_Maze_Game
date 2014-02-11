////////////////////////////////
// BEG M2.CPP
////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "mn.hpp"
#include "m2.hpp"

void m2::play (void)
{
	while (true) {
		//lenx=3,leny=4; // for testing
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

m2::m2 (void) {
	// 
	lenx=0, leny=0;
	arry=NULL;
	x=0, y=0;
	gx=0, gy=0;
}

m2::~m2 (void) {
	degen();
}

////////////////////////////////
//        allocation          //
////////////////////////////////

void m2::gen (void)
{
	degen();
	// keep size within bounds
	if (lenx < SIZE_MIN) lenx = SIZE_MIN;
	if (lenx > SIZE_MAX) lenx = SIZE_MAX;
	if (leny < SIZE_MIN) leny = SIZE_MIN;
	if (leny > SIZE_MAX) leny = SIZE_MAX;
	// allocate
	arry = (char**) calloc(lenx+1,sizeof(char*));
	for (int i = 0; i < lenx+1; i++) {
		arry[i] = (char*) calloc(leny+1,sizeof(char));
	}
}

void m2::degen (void)
{
	if (arry != NULL) {
		for (int i = 0; i < lenx+1; i++) {
			free(arry[i]);
		}
		free(arry);
		arry = NULL;
	
		lenx = 0;
		leny = 0;
	}	
}

////////////////////////////////
//           boxes            //
////////////////////////////////

void m2::frame (void)
{
	int i=0, j=0;
	// make outer walls
	/*
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
	*/
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		arry[i][j] |= XD;
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		arry[i][j] |= YD;
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		arry[i][j] = XD;
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		arry[i][j] = YD;
	}
}

void m2::box (void)
{
	int i=0, j=0;
	// make outer walls
	for (i = 1; i < lenx; i++) {
		for (j = 1; j < leny; j++) {
			arry[i][j] = 0;
		}
	}
	// XD
	i = 0;
	for (j = 0; j < leny; j++) {
		arry[i][j] |= XD;
	}
	// YD
	j = 0;
	for (i = 0; i < lenx; i++) {
		arry[i][j] |= YD;
	}

	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		arry[i][j] = XD;
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		arry[i][j] = YD;
	}
}

void m2::cage (void)
{
	int i=0, j=0;
	// build walls
	for (i = 0; i < lenx; i++) {
		for (j = 0; j < leny; j++) {
			arry[i][j] = XD;
			arry[i][j] |= YD;
		}
	}
	
	// XU
	i = lenx;
	for (j = 0; j < leny; j++) {
		arry[i][j] = XD;
	}
	// YU
	j = leny;
	for (i = 0; i < lenx; i++) {
		arry[i][j] = YD;
	}
}

void m2::empty (void)
{
	int i=0, j=0;
	// make outer walls
	for (i = 0; i < lenx+1; i++) {
		for (j = 0; j < leny+1; j++) {
			arry[i][j] = 0;
		}
	}
}

////////////////////////////////
//          print             //
////////////////////////////////

void m2::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

void m2::print_man (void)
{
	printf(
		"  axes: \n"
		"     +----X \n"
		"     |      \n"
		"     |      \n"
		"     Y      \n"
		"  \n"
		"  controls: \n"
		"  X+   %c \n"
		"  X-   %c \n"
		"  Y+   %c \n"
		"  Y-   %c \n"
		"  new  ` \n"
		"  type it then hit enter \n"
		"  \n"
		"  USER: %s \n"
		"  GOAL: %s \n"
		"  \n",
		XP, XM, YP, YM, USER, GOAL);
}

void m2::print_all (void)
{
	// debug
	//printf("print_m2, %0X, %i, %i\n", arry, lenx, leny);		
			
	// Y
	for (int j = 0; j < leny+1; j++) {
		// halls YD to the north
		// X
		for (int i = 0; i < lenx+1; i++) {
			printf("%s", WALL);
			if (i != lenx) {
				printf("%s", arry[i][j]&YD ? WALL : SPACE);
			}
		}
		
		printf("\n");
	
		if (j != leny) {
			// halls XD to the west
			// X
			for (int i = 0; i < lenx+1; i++) {
				printf("%s", arry[i][j]&XD ? WALL : SPACE);
				if (i != lenx) {
					// this is on one important space
					// user
					if (i == x && j == y) {
						printf("%s", USER);
					}
					// goal
					else if (has_flag(i,j,F_GOAL)) {
						printf("%s", GOAL);
					}
					else {
						printf("%s", SPACE);
					}
				}
			}
			printf("\n");		
		}	
	}
}

////////////////////////////////
//         control            //
////////////////////////////////

void m2::get_size (void)
{
	// get size
	printf("x size (<=%i):\n", SIZE_MAX);
	scanf("%i", &lenx);
	printf("y size (<=%i):\n", SIZE_MAX);
	scanf("%i", &leny);
}

bool m2::control (void)
{
	//x = 0, y = 0;
	
	bool cont = true;
	char comm = ' ';
	while (cont) {
		print_clr();
		print_man();
		print_all();
		// debug
		//printf ("%i %i\n", x, y);
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

bool inline m2::valid (void)
{
	return 0 <= x && x < lenx && 0 <= y && y < leny ? true : false; 
}

bool inline m2::valid (int i, int j)
{
	return 0 <= i && i < lenx && 0 <= j && j < leny ? true : false; 
}

void inline m2::set_flag (char flag)
{
	if (valid(x,y)) arry[x][y] |= flag;
}

void inline m2::set_flag (int i, int j, char flag)
{
	if (valid(i,j)) arry[i][j] |= flag;
}

void inline m2::clear_flag (char flag)
{
	if (valid(x,y)) arry[x][y] &= ~flag;
}

void inline m2::clear_flag (int i, int j, char flag)
{
	if (valid(i,j)) arry[i][j] &= ~flag;
}

void inline m2::clear_flag_all (char flag)
{
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			clear_flag(i,j,flag);
		}
	}
}

bool inline m2::has_flag (char flag)
{
	return valid(x,y) && arry[x][y]&flag ? true : false;
}

bool inline m2::has_flag (int i, int j, char flag)
{
	return valid(i,j) && arry[i][j]&flag ? true : false;
}

bool inline m2::can_move(char dir)
{
	switch(dir) {
		case XM:
			return 0 < x && !(arry[x][y]&XD) ? true : false;
		case XP:
			return x < lenx && !(arry[x+1][y]&XD) ? true : false;
		case YM:
			return 0 < y && !(arry[x][y]&YD) ? true : false;
		case YP:
			return y < leny && !(arry[x][y+1]&YD) ? true : false;
		default:
			return false;
	}
}

bool inline m2::can_move(int i, int j, char dir)
{
	switch(dir) {
		case XM:
			return 0 < i && !(arry[i][j]&XD) ? true : false;
		case XP:
			return i < lenx && !(arry[i+1][j]&XD) ? true : false;
		case YM:
			return 0 < j && !(arry[i][j]&YD) ? true : false;
		case YP:
			return j < leny && !(arry[i][j+1]&YD) ? true : false;
		default:
			return false;
	}
}

bool m2::smash(char dir) {
	switch(dir) {
		case XM:
			if (0 < x && x < lenx && 0 <= y && y < leny) {
				// smash
				arry[x][y] &= ~XD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case XP:
			// XM for x+1
			x++;
			if (0 < x && x < lenx && 0 <= y && y < leny) {
				// smash
				arry[x][y] &= ~XD;
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
			if (0 < x && x < lenx && 0 <= y && y < leny) {
				// smash
				arry[x][y] &= ~YD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case YP:
			// YM for y+1
			y++;
			if (0 < x && x < lenx && 0 <= y && y < leny) {
				// smash
				arry[x][y] &= ~YD;
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
		default:
			return false;
	}
}

bool m2::smash(int i, int j, char dir) {
	switch(dir) {
		case XM:
			if (0 < i && i < lenx && 0 <= j && j < leny) {
				arry[i][j] &= ~XD;
				return true;
			} else {
				return false;
			}
		case XP:
			// XM for i+1
			i++;
			if (0 < i && i < lenx && 0 <= j && j < leny) {
				arry[i][j] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YM:
			if (0 < i && i < lenx && 0 <= j && j < leny) {
				arry[i][j] &= ~YD;
				return true;
			} else {
				return false;
			}
		case YP:
			// YM for j+1
			j++;
			if (0 < i && i < lenx && 0 <= j && j < leny) {
				arry[i][j] &= ~YD;
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
int m2::rec_depth_solve (void)
{
	// assume current position is valid

	// goal reached
	if (has_flag(F_GOAL)) {
		clear_flag_all(F_TEMP);
		return 0;
	}

	// mark as visited
	set_flag(x,y,F_TEMP);

	// XD
	if (can_move(XM) && !has_flag(x-1,y,F_TEMP)) {
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
	if (can_move(XP) && !has_flag(x+1,y,F_TEMP)) {
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
	if (can_move(YM) && !has_flag(x,y-1,F_TEMP)) {
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
	if (can_move(YP) && !has_flag(x,y+1,F_TEMP)) {
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
	
	// failed
	return -1;
}

int m2::rec_breadth_solve (void)
{
	return -1;
}

////////////////////////////////
//     maze algorithms        //
////////////////////////////////

void m2::random_build (void)
{
	// completely random
	empty();
	for (int i = 0; i < lenx; i++) {
		for (int j = 0; j < leny; j++) {
			if (rand() > RAND_MAX/2) arry[i][j] |= XD;
			if (rand() > RAND_MAX/2) arry[i][j] |= YD;
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
		//printf("%i %i\n%i\n",x,y,c);
		//getchar();
	}
	*/

	// start
	x=0, y=0;
	// goal
	gx=lenx-1, gy=leny-1;
	set_flag(gx,gy,F_GOAL);
}

////////////////////////////////
//        depth-first         //
////////////////////////////////

void m2::depth_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0,y=0;
	// smashy-smashy
	rec_depth_build();
	gx=lenx-1, gy=leny-1;
	set_flag(gx,gy,F_GOAL);
}

// set to valid position before
void m2::rec_depth_build (void)
{
	// assume current position is valid

	// mark as visited
	set_flag(x,y,F_TEMP);
	
	//print_all();
	//printf("%i %i\n",x,y);
	//getchar();
	
	char dirs[] = {XM,XP,YM,YP};
	int ind = 0;
	char dir = 0;
	for (int n = DIRS2; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// XD
		// if node is available and wall can be smashed
		if (dir==XM && valid(x-1,y) && !has_flag(x-1,y,F_TEMP) && smash(XM)) {
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
		if (dir==XP && valid(x+1,y) && !has_flag(x+1,y,F_TEMP) && smash(XP)) {
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
		if (dir==YM && valid(x,y-1) && !has_flag(x,y-1,F_TEMP) && smash(YM)) {
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
		if (dir==YP && valid(x,y+1) && !has_flag(x,y+1,F_TEMP) && smash(YP)) {
			// move forward
			y++;
			// recusrive call
			//printf("YU\n");
			rec_depth_build();
			// move back
			y--;
		}
		
		// swap remaining directions
		dirs[ind] = dirs[n-1];
	}
		
	// failed
	return;
}

////////////////////////////////
// END M2.CPP
////////////////////////////////
