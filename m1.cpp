////////////////////////////////
// BEG M1.CPP
////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "mn.hpp"
#include "m1.hpp"

void m1::play (void)
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

m1::m1 (void) {
	// 
	lenx=0;
	arry=NULL;
	x=0;
	gx=0;
}

m1::~m1 (void) {
	degen();
}

////////////////////////////////
//        allocation          //
////////////////////////////////

void m1::gen (void)
{
	degen();
	// keep size within bounds
	if (lenx < SIZE_MIN) lenx = SIZE_MIN;
	if (lenx > SIZE_MAX) lenx = SIZE_MAX;
	// allocate
	arry = (char*) calloc(lenx+1,sizeof(char));
}

void m1::degen (void)
{
	if (arry != NULL) {
		free(arry);
		arry = NULL;
	
		lenx = 0;
	}	
}

////////////////////////////////
//           boxes            //
////////////////////////////////

void m1::frame (void)
{
	int i=0;
	// make outer walls
	/*
	for (i = 1; i < lenx; i++) {
	*/
	// XD
	i = 0;
	arry[i] |= XD;

	// XU
	i = lenx;
	arry[i] = XD;
}

void m1::box (void)
{
	int i=0;
	// make outer walls
	for (i = 1; i < lenx; i++) {
		arry[i] = 0;
	}
	// XD
	i = 0;
	arry[i] |= XD;

	// XU
	i = lenx;
	arry[i] = XD;
}

void m1::cage (void)
{
	int i=0;
	// build walls
	for (i = 0; i < lenx; i++) {
		arry[i] = XD;
	}
	
	// XU
	i = lenx;
	arry[i] = XD;
}

void m1::empty (void)
{
	int i=0;
	// make outer walls
	for (i = 0; i < lenx+1; i++) {
		arry[i] = 0;
	}
}

////////////////////////////////
//          print             //
////////////////////////////////

void m1::print_clr (void)
{
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
}

void m1::print_man (void)
{
	printf(
		"  axes: \n"
		"  +----X \n"
		"  \n"
		"  controls: \n"
		"  X+   %c \n"
		"  X-   %c \n"
		"  new  ` \n"
		"  type it then hit enter \n"
		"  \n"
		"  USER: %s \n"
		"  GOAL: %s \n"
		"  \n",
		XP, XM, USER, GOAL);
}

void m1::print_all (void)
{
	// debug
	//printf("print_m1, %0X, %i\n", arry, lenx);
			
	// halls YD to the north
	// X
	for (int i = 0; i < lenx+1; i++) {
		printf("%s", WALL);
		if (i != lenx) {
			printf("%s", WALL);
		}
	}
	
	printf("\n");

	// halls XD to the west
	// X
	for (int i = 0; i < lenx+1; i++) {
		printf("%s", arry[i]&XD ? WALL : SPACE);
		if (i != lenx) {
			// this is on one important space
			// user
			if (i == x) {
				printf("%s", USER);
			}
			// goal
			else if (has_flag(i,F_GOAL)) {
				printf("%s", GOAL);
			}
			else {
				printf("%s", SPACE);
			}
		}
	}	

	printf("\n");

	// halls YD to the north
	// X
	for (int i = 0; i < lenx+1; i++) {
		printf("%s", WALL);
		if (i != lenx) {
			printf("%s", WALL);
		}
	}
	
	printf("\n");
}

////////////////////////////////
//         control            //
////////////////////////////////

void m1::get_size (void)
{
	// get size
	printf("x size (<=%i):\n", SIZE_MAX);
	scanf("%i", &lenx);
}

bool m1::control (void)
{
	//x = 0;
	
	bool cont = true;
	char comm = ' ';
	while (cont) {
		print_clr();
		print_man();
		print_all();
		// debug
		//printf ("%i\n", x;
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

bool inline m1::valid (void)
{
	return 0 <= x && x < lenx ? true : false; 
}

bool inline m1::valid (int i)
{
	return 0 <= i && i < lenx ? true : false; 
}

void inline m1::set_flag (char flag)
{
	if (valid(x)) arry[x] |= flag;
}

void inline m1::set_flag (int i, char flag)
{
	if (valid(i)) arry[i] |= flag;
}

void inline m1::clear_flag (char flag)
{
	if (valid(x)) arry[x] &= ~flag;
}

void inline m1::clear_flag (int i, char flag)
{
	if (valid(i)) arry[i] &= ~flag;
}

void inline m1::clear_flag_all (char flag)
{
	for (int i = 0; i < lenx; i++) {
		clear_flag(i,flag);
	}
}

bool inline m1::has_flag (char flag)
{
	return valid(x) && arry[x]&flag ? true : false;
}

bool inline m1::has_flag (int i, char flag)
{
	return valid(i) && arry[i]&flag ? true : false;
}

bool inline m1::can_move(char dir)
{
	switch(dir) {
		case XM:
			return 0 < x && !(arry[x]&XD) ? true : false;
		case XP:
			return x < lenx && !(arry[x+1]&XD) ? true : false;
		default:
			return false;
	}
}

bool inline m1::can_move(int i, char dir)
{
	switch(dir) {
		case XM:
			return 0 < i && !(arry[i]&XD) ? true : false;
		case XP:
			return i < lenx && !(arry[i+1]&XD) ? true : false;
		default:
			return false;
	}
}

bool m1::smash(char dir) {
	switch(dir) {
		case XM:
			if (0 < x && x < lenx) {
				// smash
				arry[x] &= ~XD;
				// did smash
				return true;
			} else {
				// did not smash
				return false;
			}
		case XP:
			// XM for x+1
			x++;
			if (0 < x && x < lenx) {
				// smash
				arry[x] &= ~XD;
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
		default:
			return false;
	}
}

bool m1::smash(int i, char dir) {
	switch(dir) {
		case XM:
			if (0 < i && i < lenx) {
				arry[i] &= ~XD;
				return true;
			} else {
				return false;
			}
		case XP:
			// XM for i+1
			i++;
			if (0 < i && i < lenx) {
				arry[i] &= ~XD;
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
int m1::rec_depth_solve (void)
{
	// assume current position is valid

	// goal reached
	if (has_flag(F_GOAL)) {
		clear_flag_all(F_TEMP);
		return 0;
	}

	// mark as visited
	set_flag(x,F_TEMP);

	// XD
	if (can_move(XM) && !has_flag(x-1,F_TEMP)) {
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
	if (can_move(XP) && !has_flag(x+1,F_TEMP)) {
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
		
	// failed
	return -1;
}

int rec_breadth_solve (void)
{
	return -1;
}

////////////////////////////////
//     maze algorithms        //
////////////////////////////////

void m1::random_build (void)
{
	// completely random
	empty();
	for (int i = 0; i < lenx; i++) {
		if (rand() > RAND_MAX/2) arry[i] |= XD;
	}
	frame();

	// when you get a breadth-first solver, implement this
	/*
	int c = -1;
	while (c < 0) {
		random_build();
		c = rec_depth_solve();
		//print_all();
		//printf("%i\n%i\n",x,c);
		//getchar();
	}
	*/

	// start
	x=0;
	// goal
	gx=lenx-1;
	set_flag(gx,F_GOAL);
}

////////////////////////////////
//        depth-first         //
////////////////////////////////

void m1::depth_build (void)
{
	// build walls
	cage();
	// if position not valid, move to origin
	if (!valid()) x=0;
	// smashy-smashy
	rec_depth_build();
	gx=lenx-1;
	set_flag(gx,F_GOAL);
}

// set to valid position before
void m1::rec_depth_build (void)
{
	// assume current position is valid

	// mark as visited
	set_flag(x,F_TEMP);
	
	//print_all();
	//printf("%i\n",x);
	//getchar();
	
	char dirs[] = {XM,XP};
	int ind = 0;
	char dir = 0;
	for (int n = DIRS1; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// XD
		// if node is available and wall can be smashed
		if (dir==XM && valid(x-1) && !has_flag(x-1,F_TEMP) && smash(XM)) {
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
		if (dir==XP && valid(x+1) && !has_flag(x+1,F_TEMP) && smash(XP)) {
			// move forward
			x++;
			// recusrive call
			//printf("XU\n");
			rec_depth_build();
			// move back
			x--;
		}
		
		// swap remaining directions
		dirs[ind] = dirs[n-1];
	}
		
	// failed
	return;
}

////////////////////////////////
// END M1.CPP
////////////////////////////////
