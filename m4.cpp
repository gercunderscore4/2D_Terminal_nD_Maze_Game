/*
 * FILE:    m4.cpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-19
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
 */

#include "m4.h"

using namespace std;

////////////////////////////////////////////////////////////////
////////////////            CLASS               ////////////////
////////////////////////////////////////////////////////////////

/*
 * Class constructor.
 */
m4_c::m4_c (void)
{
	// all blank
	lenx = 0;
	leny = 0;
	lenz = 0;
	lenw = 0;

	arry = NULL;
	
	x = 0;
	y = 0;
	z = 0;
	w = 0;
	
	a0 = 0;
	a1 = 1;
	a2 = 2;
	a3 = 3;
	
	sight = S_DEFAULT;
}

m4_c::~m4_c (void)
{
	degen();
}

void m4_c::makeMaze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws)
{
	makeMaze(lxs, lys, lzs, lws, ALG_DEFAULT, S_DEFAULT, G_DEFAULT);
}

void m4_c::makeMaze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs)
{
	makeMaze(lxs, lys, lzs, lws, algs, S_DEFAULT, G_DEFAULT);
}

void m4_c::makeMaze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights)
{
	makeMaze(lxs, lys, lzs, lws, algs, sights, G_DEFAULT);
}

void m4_c::makeMaze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights, goal_t goal)
{
	// de-allocate any previous maze
	printf("DEGEN\n");
	degen();

	// allocate
	printf("GEN\n");
	gen(lxs, lys, lzs, lws);

	// build
	printf("ALG\n");
	switch (algs) {
		case ALG_RAND:
			random_build();
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
		case ALG_BOX:
			box();
			break;
		case ALG_CAGE:
			cage();
			break;
		case ALG_DEFAULT:
		default:
			hunt_and_kill_build();
			break;
	}

	// set goal, ensure that it's within bounds
	printf("GOAL\n");
	switch (goal) {
		case G_SIMPLE:
			set_goal_simple();
			break;
		case G_LONG:
			set_goal_long();
			break;
		case G_LONG_RAND:
			set_goal_long_rand();
			break;
		case G_LONGEST:
			set_goal_longest();
			break;
		case G_DEFAULT:
		default:
			set_goal_simple();
			break;
	}
	
	// set dimensions
	a0 = 0;
	a1 = 1;
	a2 = 2;
	a3 = 3;

	// dicovery method
	printf("SIGHT\n");
	sight = sights;
	if (sight == S_FULL) {
		set_flag_all(F_DISC);
	}
	
	printf("DISC\n");
	discover();
}

//////////////// MOVE ////////////////
void m4_c::move(dir_t dir)
{
	switch (dir) {
		case XD:
			if (can_move(XD) == true) x--;
			break;
		case XU:
			if (can_move(XU) == true) x++;
			break;
		case YD:
			if (can_move(YD) == true) y--;
			break;
		case YU:
			if (can_move(YU) == true) y++;
			break;
		case ZD:
			if (can_move(ZD) == true) z--;
			break;
		case ZU:
			if (can_move(ZU) == true) z++;
			break;
		case WD:
			if (can_move(WD) == true) w--;
			break;
		case WU:
			if (can_move(WU) == true) w++;
			break;
	}
}

void m4_c::discover(void)
{
	switch (sight) {
		case S_FULL:
			// already discovered (in makeMaze), do nothing
			break;
		case S_RANGE:
			clear_flag_all(F_DISC);
			disc_ranged(RANGE);
			break;
		case S_RANGE_DISC:
			disc_ranged(RANGE);
			break;
		case S_LOS:
			clear_flag_all(F_DISC);
			disc_line_of_sight();
			break;
		case S_LOS_DISC:
			disc_line_of_sight();
			break;
		case S_LOS_RANGE:
			clear_flag_all(F_DISC);
			disc_line_of_sight_ranged(RANGE);
			break;
		case S_LOS_RANGE_DISC:
			disc_line_of_sight_ranged(RANGE);
			break;
		case S_DEFAULT:
		default:
			disc_line_of_sight();
			break;
	}
}

//////////////// GET ////////////////
// lengths
unsigned int m4_c::get_lenx(void)
{
	return lenx;
}

unsigned int m4_c::get_leny(void)
{
	return leny;
}

unsigned int m4_c::get_lenz(void)
{
	return lenz;
}

unsigned int m4_c::get_lenw(void)
{
	return lenw;
}

// position
unsigned int m4_c::get_x(void)
{
	return x;
}

unsigned int m4_c::get_y(void)
{
	return y;
}

unsigned int m4_c::get_z(void)
{
	return z;
}

unsigned int m4_c::get_w(void)
{
	return w;
}

bool m4_c::set_x(unsigned int nx)
{
	if (valid(nx,y,z,w) == true) {
		x = nx;
		return true;
	}
	return false;
}

bool m4_c::set_y(unsigned int ny)
{
	if (valid(x,ny,z,w) == true) {
		y = ny;
		return true;
	}
	return false;
}

bool m4_c::set_z(unsigned int nz)
{
	if (valid(x,y,nz,w) == true) {
		z = nz;
		return true;
	}
	return false;
}

bool m4_c::set_w(unsigned int nw)
{
	if (valid(x,y,z,nw) == true) {
		w = nw;
		return true;
	}
	return false;
}

// axes
unsigned int m4_c::get_a0(void)
{
	return a0;
}

unsigned int m4_c::get_a1(void)
{
	return a1;
}

unsigned int m4_c::get_a2(void)
{
	return a2;
}

unsigned int m4_c::get_a3(void)		
{
	return a3;
}

////////////////////////////////////////////////////////////////
////////////////          ALLOCATION            ////////////////
////////////////////////////////////////////////////////////////

/*
 * Class constructor.
 */
void m4_c::gen (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws)
{
	degen();
	
	// keep size within bounds
	lenx = lxs;
	leny = lys;
	lenz = lzs;
	lenw = lws;
	if (lenx < LEN_MIN) lenx = LEN_MIN;
	if (lenx > LEN_MAX) lenx = LEN_MAX;
	if (leny < LEN_MIN) leny = LEN_MIN;
	if (leny > LEN_MAX) leny = LEN_MAX;
	if (lenz < LEN_MIN) lenz = LEN_MIN;
	if (lenz > LEN_MAX) lenz = LEN_MAX;
	if (lenw < LEN_MIN) lenw = LEN_MIN;
	if (lenw > LEN_MAX) lenw = LEN_MAX;

	// allocate
	arry = (node_t****) malloc(lenx*sizeof(node_t***));
	for (unsigned int i = 0; i < lenx; i++) {
		arry[i] = (node_t***) malloc(leny*sizeof(node_t**));
		for (unsigned int j = 0; j < leny; j++) {
			arry[i][j] = (node_t**) malloc(lenz*sizeof(node_t*));
			for (unsigned int k = 0; k < lenz; k++) {
				arry[i][j][k] = (node_t*) malloc(lenw*sizeof(node_t));
				for (unsigned int h = 0; h < lenw; h++) {
					arry[i][j][k][h] = 0;
				}
			}
		}
	}
}

/*
 * Class destructor.
 */
void m4_c::degen(void)
{
	if (arry != NULL) {
		for (unsigned int i = 0; i < lenx; i++) {
			for (unsigned int j = 0; j < leny; j++) {
				for (unsigned int k = 0; k < lenz; k++) {
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
void m4_c::frame(void)
{
	// make outer walls
	// XD & XU
	for (unsigned int j = 0; j < leny; j++) {
		for (unsigned int k = 0; k < lenz; k++) {
			for (unsigned int h = 0; h < lenw; h++) {
				// XD
				arry[0][j][k][h] |= XD;
				// XU
				arry[lenx-1][j][k][h] |= XU;
			}
		}
	}
	// YD & YU
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int k = 0; k < lenz; k++) {
			for (unsigned int h = 0; h < lenw; h++) {
				// YD
				arry[i][0][k][h] |= YD;
				// YU
				arry[i][leny-1][k][h] |= YU;
			}
		}
	}
	// ZD & ZU
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int h = 0; h < lenw; h++) {
				// ZD
				arry[i][j][0][h] |= ZD;
				// ZU
				arry[i][j][lenz-1][h] |= ZU;
			}
		}
	}
	// WD & WU
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
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
void m4_c::box(void)
{
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
				for (unsigned int h = 0; h < lenw; h++) {
					arry[i][j][k][h] = 0;
				}
			}
		}
	}
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
void m4_c::cage(void)
{
	// build walls
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
				for (unsigned int h = 0; h < lenw; h++) {
					arry[i][j][k][h] = XD|YD|ZD|WD | XU|YU|ZU|WU;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////
////////////////           GRAPHICS             ////////////////
////////////////////////////////////////////////////////////////

/*
 * Discovers line of sight.
 * For line of sight only, clear disc flags before this in control loop.
 */
void m4_c::disc_line_of_sight (void)
{
	// current position
	set_flag(F_DISC);
	
	unsigned int i=x, j=y, k=z, h=w;
	
	// X-
	while (can_move(i,j,k,h,XD)) {
		i--;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// X+
	while (can_move(i,j,k,h,XU)) {
		i++;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// Y-
	while (can_move(i,j,k,h,YD)) {
		j--;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Y+
	while (can_move(i,j,k,h,YU)) {
		j++;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Z-
	while (can_move(i,j,k,h,ZD)) {
		k--;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// Z+
	while (can_move(i,j,k,h,ZU)) {
		k++;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// W-
	while (can_move(i,j,k,h,WD)) {
		h--;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
	
	// W+
	while (can_move(i,j,k,h,WU)) {
		h++;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
}

/*
 * Discovers ranged line of sight.
 * For line of sight only, clear disc flags before this in control loop.
 */
void m4_c::disc_line_of_sight_ranged (unsigned int range)
{
	// current position
	set_flag(F_DISC);
	
	unsigned int i=x, j=y, k=z, h=w;
	
	// X-
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,XD); ii++) {
		i--;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// X+
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,XU); ii++) {
		i++;
		set_flag(i,j,k,h,F_DISC);
	}
	i=x;
	
	// Y-
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,YD); ii++) {
		j--;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Y+
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,YU); ii++) {
		j++;
		set_flag(i,j,k,h,F_DISC);
	}
	j=y;
	
	// Z-
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,ZD); ii++) {
		k--;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// Z+
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,ZU); ii++) {
		k++;
		set_flag(i,j,k,h,F_DISC);
	}
	k=z;
	
	// W-
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,WD); ii++) {
		h--;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
	
	// W+
	for (unsigned int ii=0; ii<range && can_move(i,j,k,h,WU); ii++) {
		h++;
		set_flag(i,j,k,h,F_DISC);
	}
	h=w;
}

/*
 * Discovers ranged sight.
 * For near sight only, clear disc flags before this in control loop.
 */
void m4_c::disc_ranged (unsigned int range)
{
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned int h;
	
	i = x;
	j = y;
	k = z;
	h = w;
	
	unsigned int i_min = i-range > 0 ? i-range : 0;
	unsigned int i_max = i+range < lenx ? i+range : lenx;
	unsigned int j_min = j-range > 0 ? j-range : 0;
	unsigned int j_max = j+range < leny ? j+range : leny;
	unsigned int k_min = k-range > 0 ? k-range : 0;
	unsigned int k_max = k+range < lenz ? k+range : lenz;
	unsigned int h_min = h-range > 0 ? h-range : 0;
	unsigned int h_max = h+range < lenw ? h+range : lenw;
	
	for (unsigned int i=i_min; i<i_max; i++) {
		for (unsigned int j=j_min; j<j_max; j++) {
			for (unsigned int k=k_min; k<k_max; k++) {
				for (unsigned int h=h_min; h<h_max; h++) {
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
 * Dimension swap, swap through the axis that is originally X.
 */
void m4_c::d_swap_abs (unsigned int d1, unsigned int d2)
{
	if (d1 == DIMX) {
		if (d2 == DIMY) {
			unsigned int temp = a0;
			a0 = a1;
			a1 = temp;
		} else if (d2 == DIMZ) {
			unsigned int temp = a0;
			a0 = a2;
			a2 = temp;
		} else if (d2 == DIMW) {
			unsigned int temp = a0;
			a0 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMY) {
		if (d2 == DIMX) {
			unsigned int temp = a1;
			a1 = a0;
			a0 = temp;
		} else if (d2 == DIMZ) {
			unsigned int temp = a1;
			a1 = a2;
			a2 = temp;
		} else if (d2 == DIMW) {
			unsigned int temp = a1;
			a1 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMZ) {
		if (d2 == DIMX) {
			unsigned int temp = a2;
			a2 = a0;
			a0 = temp;
		} else if (d2 == DIMY) {
			unsigned int temp = a2;
			a2 = a1;
			a1 = temp;
		} else if (d2 == DIMW) {
			unsigned int temp = a2;
			a2 = a3;
			a3 = temp;
		}
	} else if (d1 == DIMW) {
		if (d2 == DIMX) {
			unsigned int temp = a3;
			a3 = a0;
			a0 = temp;
		} else if (d2 == DIMY) {
			unsigned int temp = a3;
			a3 = a1;
			a1 = temp;
		} else if (d2 == DIMZ) {
			unsigned int temp = a3;
			a3 = a2;
			a2 = temp;
		}
	}
}

/*
 * Dimension swap, swap through the axis called X.
 */
void m4_c::d_swap_rel (unsigned int d1, unsigned int d2)
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
bool inline m4_c::valid (void)
{
	return x < lenx && y < leny && z < lenz && w < lenw ? true : false;
}

/*
 * Is position valid?
 */
bool inline m4_c::valid (unsigned int i, unsigned int j, unsigned int k, unsigned int h)
{
	return i < lenx && j < leny && k < lenz && h < lenw ? true : false;
}

/*
 * Set flag at position.
 */
void inline m4_c::set_flag (node_t flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] |= flag;
}

/*
 * Set flag at position.
 */
void inline m4_c::set_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] |= flag;
}

/*
 * Set flag in all maze.
 */
void inline m4_c::set_flag_all (node_t flag)
{
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
				for (unsigned int h = 0; h < lenw; h++) {
					arry[i][j][k][h] |= flag;
				}
			}
		}
	}
}
/*
 * Clear flag from position.
 */
void inline m4_c::clear_flag (node_t flag)
{
	if (valid(x,y,z,w)) arry[x][y][z][w] &= ~flag;
}

/*
 * Clear flag from position.
 */
void inline m4_c::clear_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag)
{
	if (valid(i,j,k,h)) arry[i][j][k][h] &= ~flag;
}

/*
 * Clear flag from all maze.
 */
void inline m4_c::clear_flag_all (node_t flag)
{
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
				for (unsigned int h = 0; h < lenw; h++) {
					arry[i][j][k][h] &= ~flag;
				}
			}
		}
	}
}

/*
 * Is this flag up at position?
 */
bool inline m4_c::get_flag (node_t flag)
{
	return valid(x,y,z,w) && arry[x][y][z][w]&flag;
}

/*
 * Is this flag up at position?
 */
bool inline m4_c::get_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag)
{
	return valid(i,j,k,h) && arry[i][j][k][h]&flag;
}

/*
 * Can move from position in direction?
 */
bool inline m4_c::can_move (dir_t dir)
{
	switch(dir) {
		case XD:
			return 0 < x && !(arry[x][y][z][w]&XD);
		case XU:
			return x < lenx-1 && !(arry[x][y][z][w]&XU);
		case YD:
			return 0 < y && !(arry[x][y][z][w]&YD);
		case YU:
			return y < leny-1 && !(arry[x][y][z][w]&YU);
		case ZD:
			return 0 < z && !(arry[x][y][z][w]&ZD);
		case ZU:
			return z < lenz-1 && !(arry[x][y][z][w]&ZU);
		case WD:
			return 0 < w && !(arry[x][y][z][w]&WD);
		case WU:
			return w < lenw-1 && !(arry[x][y][z][w]&WU);
		default:
			return false;
	}
}

/*
 * Can move from position in direction?
 */
bool inline m4_c::can_move (unsigned int i, unsigned int j, unsigned int k, unsigned int h, dir_t dir)
{
	switch(dir) {
		case XD:
			return 0 < i && !(arry[i][j][k][h]&XD);
		case XU:
			return i < lenx-1 && !(arry[i][j][k][h]&XU);
		case YD:
			return 0 < j && !(arry[i][j][k][h]&YD);
		case YU:
			return j < leny-1 && !(arry[i][j][k][h]&YU);
		case ZD:
			return 0 < k && !(arry[i][j][k][h]&ZD);
		case ZU:
			return k < lenz-1 && !(arry[i][j][k][h]&ZU);
		case WD:
			return 0 < h && !(arry[i][j][k][h]&WD);
		case WU:
			return h < lenw-1 && !(arry[i][j][k][h]&WU);
		default:
			return false;
	}
}

/*
 * HULK SMASH WALL FROM POSITION IN DIRECTION?!
 */
bool m4_c::smash (dir_t dir) {
	switch(dir) {
		case XD:
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
		case XU:
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
		case YD:
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
		case YU:
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
		case ZD:
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
		case ZU:
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
		case WD:
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
		case WU:
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
bool m4_c::smash (unsigned int i, unsigned int j, unsigned int k, unsigned int h, dir_t dir) {
	switch(dir) {
		case XD:
			if (valid(i-1,j,k,h)) {
				arry[i][j][k][h] &= ~XD;
				arry[i-1][j][k][h] &= ~XU;
				return true;
			} else {
				return false;
			}
		case XU:
			if (valid(i+1,j,k,h)) {
				arry[i][j][k][h] &= ~XU;
				arry[i+1][j][k][h] &= ~XD;
				return true;
			} else {
				return false;
			}
		case YD:
			if (valid(i,j-1,k,h)) {
				arry[i][j][k][h] &= ~YD;
				arry[i][j-1][k][h] &= ~YU;
				return true;
			} else {
				return false;
			}
		case YU:
			if (valid(i,j+1,k,h)) {
				arry[i][j][k][h] &= ~YU;
				arry[i][j+1][k][h] &= ~YD;
				return true;
			} else {
				return false;
			}
		case ZD:
			if (valid(i,j,k-1,h)) {
				arry[i][j][k][h] &= ~ZD;
				arry[i][j][k-1][h] &= ~ZU;
				return true;
			} else {
				return false;
			}
		case ZU:
			if (valid(i,j,k+1,h)) {
				arry[i][j][k][h] &= ~ZU;
				arry[i][j][k+1][h] &= ~ZD;
				return true;
			} else {
				return false;
			}
		case WD:
			if (valid(i,j,k,h-1)) {
				arry[i][j][k][h] &= ~WD;
				arry[i][j][k][h-1] &= ~WU;
				return true;
			} else {
				return false;
			}
		case WU:
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
int m4_c::depth_solve (void)
{
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;
	int ans;
	
	xs = x;
	ys = y;
	zs = z;
	ws = w;
	
	ans = rec_depth_solve();
	
	x = xs;
	y = ys;
	z = zs;
	w = ws;
	
	return ans;
}

int m4_c::rec_depth_solve (void)
{
	// goal reached
	if (get_flag(F_GOAL)) {
		clear_flag_all(F_TEMP);
		return 0;
	}

	// mark as visited
	set_flag(x,y,z,w,F_TEMP);

	// XD
	if (can_move(XD) && !get_flag(x-1,y,z,w,F_TEMP)) {
		// move forward
		x--;
		// recusrive call
		//printf("XD %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		x++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// XU
	if (can_move(XU) && !get_flag(x+1,y,z,w,F_TEMP)) {
		// move
		x++;
		// recusrive call
		//printf("XU %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		x--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// YD
	if (can_move(YD) && !get_flag(x,y-1,z,w,F_TEMP)) {
		// move
		y--;
		// recusrive call
		//printf("YD %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		y++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// YU
	if (can_move(YU) && !get_flag(x,y+1,z,w,F_TEMP)) {
		// move
		y++;
		// recusrive call
		//printf("YU %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		y--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// ZD
	if (can_move(ZD) && !get_flag(x,y,z-1,w,F_TEMP)) {
		// move
		z--;
		// recusrive call
		//printf("ZD %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		z++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// ZU
	if (can_move(ZU) && !get_flag(x,y,z+1,w,F_TEMP)) {
		// move
		z++;
		// recusrive call
		//printf("ZU %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		z--;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// WD
	if (can_move(WD) && !get_flag(x,y,z,w-1,F_TEMP)) {
		// move
		w--;
		// recusrive call
		//printf("WD %i %i %i %i\n",x,y,z,w);
		int c = rec_depth_solve();
		// move back
		w++;
		// if goal found, regress
		if (c >= 0) {
			return c+1;
		}
	}
	
	// WU
	if (can_move(WU) && !get_flag(x,y,z,w+1,F_TEMP)) {
		// move
		w++;
		// recusrive call
		//printf("WU %i %i %i %i\n",x,y,z,w);
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

int m4_c::breadth_solve (void)
{
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;
	
	// save position
	xs = x;
	ys = y;
	zs = z;
	ws = w;
	
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
		if (get_flag(F_GOAL)) {
			clear_flag_all(F_TEMP);
			x = xs;
			y = ys;
			z = zs;
			w = ws;
			return 1;
		}

		// mark as visited
		set_flag(x,y,z,w,F_TEMP);
		// debug
		//print_all();
		
		// check
		// XD
		if (can_move(XD) && !get_flag(x-1,y,z,w,F_TEMP)) {
			//printf("XD %i %i %i %i\n",x-1,y,z,w);
			nodes.push((unsigned int) ((x-1)<<24) + (y<<16) + (z<<8) + (w<<0));
		}
		// XU
		if (can_move(XU) && !get_flag(x+1,y,z,w,F_TEMP)) {
			//printf("XU %i %i %i %i\n",x+1,y,z,w);
			nodes.push((unsigned int) ((x+1)<<24) + (y<<16) + (z<<8) + (w<<0));
		}
		// YD
		if (can_move(YD) && !get_flag(x,y-1,z,w,F_TEMP)) {
			//printf("YD %i %i %i %i\n",x,y-1,z,w);
			nodes.push((unsigned int) (x<<24) + ((y-1)<<16) + (z<<8) + (w<<0));
		}
		// YU
		if (can_move(YU) && !get_flag(x,y+1,z,w,F_TEMP)) {
			//printf("YU %i %i %i %i\n",x,y+1,z,w);
			nodes.push((unsigned int) (x<<24) + ((y+1)<<16) + (z<<8) + (w<<0));
		}
		// ZD
		if (can_move(ZD) && !get_flag(x,y,z-1,w,F_TEMP)) {
			//printf("ZD %i %i %i %i\n",x,y,z-1,w);
			nodes.push((unsigned int) (x<<24) + (y<<16) + ((z-1)<<8) + (w<<0));
		}
		// ZU
		if (can_move(ZU) && !get_flag(x,y,z+1,w,F_TEMP)) {
			//printf("ZU %i %i %i %i\n",x,y,z+1,w);
			nodes.push((unsigned int) (x<<24) + (y<<16) + ((z+1)<<8) + (w<<0));
		}
		// WD
		if (can_move(WD) && !get_flag(x,y,z,w-1,F_TEMP)) {
			//printf("WD %i %i %i %i\n",x,y,z,w-1);
			nodes.push((unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w-1)<<0));
		}
		// WU
		if (can_move(WU) && !get_flag(x,y,z,w+1,F_TEMP)) {
			//printf("ZU %i %i %i %i\n",x,y,z,w+1);
			nodes.push((unsigned int) (x<<24) + (y<<16) + (z<<8) + ((w+1)<<0));
		}
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x = xs;
	y = ys;
	z = zs;
	w = ws;
	
	return -1;
}

////////////////////////////////////////////////////////////////
////////////////           BUILDERS             ////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////
//      random unsolved       //
////////////////////////////////

void m4_c::random_build (void)
{
	// completely random
	cage();
	for (unsigned int i = 0; i < lenx; i++) {
		for (unsigned int j = 0; j < leny; j++) {
			for (unsigned int k = 0; k < lenz; k++) {
				for (unsigned int h = 0; h < lenw; h++) {
					if (rand() > RAND_MAX/2) smash(i,j,k,h,XD);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,YD);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,ZD);
					if (rand() > RAND_MAX/2) smash(i,j,k,h,WD);
				}
			}
		}
	}
	frame();
}

////////////////////////////////
//        depth-first         //
////////////////////////////////

void m4_c::depth_build (void)
{
	// build walls
	cage();

	// if position not valid, move to origin
	if (!valid()) {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	// smashy-smashy
	rec_depth_build();

	// remove flags
	clear_flag_all(F_TEMP);
}

// set to valid position before
void m4_c::rec_depth_build (void)
{
	// assume current position is valid

	// mark as visited
	set_flag(x,y,z,w,F_TEMP);
	
	//print_all();
	//printf("%i %i %i %i\n",x,y,z,w);
	
	dir_t dirs[] = {XD,XU,YD,YU,ZD,ZU,WD,WU};
	int ind = 0;
	dir_t dir = 0;
	for (int n = DIRS4; n > 0; n--) {
		ind = rand() % n;
		dir = dirs[ind];
		
		// XD
		// if node is available and wall can be smashed
		if (dir==XD && valid(x-1,y,z,w) && !get_flag(x-1,y,z,w,F_TEMP) && smash(XD)) {
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
		if (dir==XU && valid(x+1,y,z,w) && !get_flag(x+1,y,z,w,F_TEMP) && smash(XU)) {
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
		if (dir==YD && valid(x,y-1,z,w) && !get_flag(x,y-1,z,w,F_TEMP) && smash(YD)) {
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
		if (dir==YU && valid(x,y+1,z,w) && !get_flag(x,y+1,z,w,F_TEMP) && smash(YU)) {
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
		if (dir==ZD && valid(x,y,z-1,w) && !get_flag(x,y,z-1,w,F_TEMP) && smash(ZD)) {
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
		if (dir==ZU && valid(x,y,z+1,w) && !get_flag(x,y,z+1,w,F_TEMP) && smash(ZU)) {
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
		if (dir==WD && valid(x,y,z,w-1) && !get_flag(x,y,z,w-1,F_TEMP) && smash(WD)) {
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
		if (dir==WU && valid(x,y,z,w+1) && !get_flag(x,y,z,w+1,F_TEMP) && smash(WU)) {
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

void m4_c::breadth_build (void)
{
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;

	// build walls
	cage();

	// if position not valid, move to origin
	if (!valid()) {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	
	// save position
	xs = x;
	ys = y;
	zs = z;
	ws = w;
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
	// XD
	if (valid(x-1,y,z,w) && !get_flag(x-1,y,z,w,F_TEMP)) {
		temp.dir = XU;
		temp.x = x-1;
		temp.y = y;
		temp.z = z;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x-1,y,z,w,F_TEMP);
	}
	// XU
	if (valid(x+1,y,z,w) && !get_flag(x+1,y,z,w,F_TEMP)) {
		temp.dir = XD;
		temp.x = x+1;
		temp.y = y;
		temp.z = z;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x+1,y,z,w,F_TEMP);
	}
	// YD
	if (valid(x,y-1,z,w) && !get_flag(x,y-1,z,w,F_TEMP)) {
		temp.dir = YU;
		temp.x = x;
		temp.y = y-1;
		temp.z = z;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x,y-1,z,w,F_TEMP);
	}
	// YU
	if (valid(x,y+1,z,w) && !get_flag(x,y+1,z,w,F_TEMP)) {
		temp.dir = YD;
		temp.x = x;
		temp.y = y+1;
		temp.z = z;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x,y+1,z,w,F_TEMP);
	}
	// ZD
	if (valid(x,y,z-1,w) && !get_flag(x,y,z-1,w,F_TEMP)) {
		temp.dir = ZU;
		temp.x = x;
		temp.y = y;
		temp.z = z-1;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x,y,z-1,w,F_TEMP);
	}
	// ZU
	if (valid(x,y,z+1,w) && !get_flag(x,y,z+1,w,F_TEMP)) {
		temp.dir = ZD;
		temp.x = x;
		temp.y = y;
		temp.z = z+1;
		temp.w = w;
		nodes.push_back(temp);
		set_flag(x,y,z+1,w,F_TEMP);
	}
	// WD
	if (valid(x,y,z,w-1) && !get_flag(x,y,z,w-1,F_TEMP)) {
		temp.dir = WU;
		temp.x = x;
		temp.y = y;
		temp.z = z;
		temp.w = w-1;
		nodes.push_back(temp);
		set_flag(x,y,z,w-1,F_TEMP);
	}
	// WU
	if (valid(x,y,z,w+1) && !get_flag(x,y,z,w+1,F_TEMP)) {
		temp.dir = WD;
		temp.x = x;
		temp.y = y;
		temp.z = z;
		temp.w = w+1;
		nodes.push_back(temp);
		set_flag(x,y,z,w+1,F_TEMP);
	}

	// loop
	while (!nodes.empty()) {
				
		// choose random node
		//int r = rand() % nodes.size();
		
		float f = ((float) rand()/RAND_MAX ) * 10; // warning: magic number
		unsigned int r = floor( nodes.size() * (1-exp(-f)) );
		//printf("%7.3f %3i %3lu\n", f, r, nodes.size());
		
		// collect data
		x = nodes[r].x;
		y = nodes[r].y;
		z = nodes[r].z;
		w = nodes[r].w;
		dir_t dir = nodes[r].dir;
		
		// break wall
		//bool hulk = 
		smash(dir);
		
		// debug
		//printf("%i %i %i %X, %c, %i\n", x,y,z,w,dir,hulk);
		//print_all();
		
		// swap and remove
		nodes[r].x = nodes[nodes.size()-1].x;
		nodes[r].y = nodes[nodes.size()-1].y;
		nodes[r].z = nodes[nodes.size()-1].z;
		nodes[r].w = nodes[nodes.size()-1].w;
		nodes[r].dir = nodes[nodes.size()-1].dir;
		nodes.pop_back();
		
		// mark as visited
		set_flag(x,y,z,w,F_TEMP);
		
		// check
		// XD
		if (valid(x-1,y,z,w) && !get_flag(x-1,y,z,w,F_TEMP)) {
			temp.dir = XU;
			temp.x = x-1;
			temp.y = y;
			temp.z = z;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x-1,y,z,w,F_TEMP);
		}
		// XU
		if (valid(x+1,y,z,w) && !get_flag(x+1,y,z,w,F_TEMP)) {
			temp.dir = XD;
			temp.x = x+1;
			temp.y = y;
			temp.z = z;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x+1,y,z,w,F_TEMP);
		}
		// YD
		if (valid(x,y-1,z,w) && !get_flag(x,y-1,z,w,F_TEMP)) {
			temp.dir = YU;
			temp.x = x;
			temp.y = y-1;
			temp.z = z;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x,y-1,z,w,F_TEMP);
		}
		// YU
		if (valid(x,y+1,z,w) && !get_flag(x,y+1,z,w,F_TEMP)) {
			temp.dir = YD;
			temp.x = x;
			temp.y = y+1;
			temp.z = z;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x,y+1,z,w,F_TEMP);
		}
		// ZD
		if (valid(x,y,z-1,w) && !get_flag(x,y,z-1,w,F_TEMP)) {
			temp.dir = ZU;
			temp.x = x;
			temp.y = y;
			temp.z = z-1;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x,y,z-1,w,F_TEMP);
		}
		// ZU
		if (valid(x,y,z+1,w) && !get_flag(x,y,z+1,w,F_TEMP)) {
			temp.dir = ZD;
			temp.x = x;
			temp.y = y;
			temp.z = z+1;
			temp.w = w;
			nodes.push_back(temp);
			set_flag(x,y,z+1,w,F_TEMP);
		}
		// WD
		if (valid(x,y,z,w-1) && !get_flag(x,y,z,w-1,F_TEMP)) {
			temp.dir = WU;
			temp.x = x;
			temp.y = y;
			temp.z = z;
			temp.w = w-1;
			nodes.push_back(temp);
			set_flag(x,y,z,w-1,F_TEMP);
		}
		// WU
		if (valid(x,y,z,w+1) && !get_flag(x,y,z,w+1,F_TEMP)) {
			temp.dir = WD;
			temp.x = x;
			temp.y = y;
			temp.z = z;
			temp.w = w+1;
			nodes.push_back(temp);
			set_flag(x,y,z,w+1,F_TEMP);
		}
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x = xs;
	y = ys;
	z = zs;
	w = ws;
}

////////////////////////////////
//       hunt and kill        //
////////////////////////////////

void m4_c::hunt_and_kill_build (void)
{
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;	
	unsigned int len_max;
	
	// get maximum unidirectional length of maze
	len_max = (unsigned int) sqrt( (float) lenx*lenx + leny*leny + lenz*lenz + lenw*lenw );
	
	// build walls
	cage();
	
	// if position not valid, move to origin
	if (!valid()) {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	
	// save original coordinates
	xs = x;
	ys = y;
	zs = z;
	ws = w;
	
	// mark starting point
	set_flag(F_TEMP);
	// make a starting path
	unsigned int len = rand()%len_max;
	for (unsigned int l = len; l > 0; l--) {
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
							len = rand()%len_max;
							for (unsigned int l = len; l > 0; l--) {
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
						done &= get_flag(F_TEMP); 
					}
				}
			}
		}
		
		//print_all();
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x = xs;
	y = ys;
	z = zs;
	w = ws;
}

void m4_c::hunt_and_kill_build (unsigned int len)
{
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;	

	if (len <= 0) {
		hunt_and_kill_build();
	}
	
	// build walls
	cage();

	// if position not valid, move to origin
	if (!valid()) {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	// save original coordinates
	xs = x;
	ys = y;
	zs = z;
	ws = w;
	
	// mark starting point
	set_flag(F_TEMP);

	// make a starting path
	for (unsigned int l = len; l > 0; l--) {
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
							for (unsigned int l = len; l > 0; l--) {
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
						done &= get_flag(F_TEMP); 
					}
				}
			}
		}
		
		//print_all();
	}
	
	// reset
	clear_flag_all(F_TEMP);
	x = xs;
	y = ys;
	z = zs;
	w = ws;
}

bool m4_c::kill (void)
{
	unsigned int ind;
	dir_t dir;
	dir_t dirs[] = {XD,XU,YD,YU,ZD,ZU,WD,WU};

	for (unsigned int n = DIRS4; n > 0; n--) {
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
		if (dir==XD && valid(x-1,y,z,w) && !get_flag(x-1,y,z,w,F_TEMP) && smash(XD)) {
			x--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==XU && valid(x+1,y,z,w) && !get_flag(x+1,y,z,w,F_TEMP) && smash(XU)) {
			x++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==YD && valid(x,y-1,z,w) && !get_flag(x,y-1,z,w,F_TEMP) && smash(YD)) {
			y--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==YU && valid(x,y+1,z,w) && !get_flag(x,y+1,z,w,F_TEMP) && smash(YU)) {
			y++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==ZD && valid(x,y,z-1,w) && !get_flag(x,y,z-1,w,F_TEMP) && smash(ZD)) {
			z--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==ZU && valid(x,y,z+1,w) && !get_flag(x,y,z+1,w,F_TEMP) && smash(ZU)) {
			z++;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==WD && valid(x,y,z,w-1) && !get_flag(x,y,z,w-1,F_TEMP) && smash(WD)) {
			w--;
			set_flag(F_TEMP);
			return true;
		}
		else if (dir==WU && valid(x,y,z,w+1) && !get_flag(x,y,z,w+1,F_TEMP) && smash(WU)) {
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

bool m4_c::kill_node (void)
{
	// if
	//     this is an un-used node next to a used node
	// then
	//     break to used node
	if (!get_flag(F_TEMP)) {
		dir_t dirs[] = {XD,XU,YD,YU,ZD,ZU,WD,WU};
		unsigned int ind = 0;
		dir_t dir = 0;
		for (unsigned int n = DIRS4; n > 0; n--) {
			ind = rand() % n;
			dir = dirs[ind];
		
			// if
			//     direction is picked
			//     it's valid
			//     it's old
			//     and it can be smashed
			// then
			//     return true
			if (dir==XD && valid(x-1,y,z,w) && get_flag(x-1,y,z,w,F_TEMP) && smash(XD)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==XU && valid(x+1,y,z,w) && get_flag(x+1,y,z,w,F_TEMP) && smash(XU)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==YD && valid(x,y-1,z,w) && get_flag(x,y-1,z,w,F_TEMP) && smash(YD)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==YU && valid(x,y+1,z,w) && get_flag(x,y+1,z,w,F_TEMP) && smash(YU)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==ZD && valid(x,y,z-1,w) && get_flag(x,y,z-1,w,F_TEMP) && smash(ZD)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==ZU && valid(x,y,z+1,w) && get_flag(x,y,z+1,w,F_TEMP) && smash(ZU)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==WD && valid(x,y,z,w-1) && get_flag(x,y,z,w-1,F_TEMP) && smash(WD)) {
				set_flag(F_TEMP);
				return true;
			}
			else if (dir==WU && valid(x,y,z,w+1) && get_flag(x,y,z,w+1,F_TEMP) && smash(WU)) {
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


////////////////////////////////////////////////////////////////
////////////////          GOAL SETUP            ////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////
//       breadth solve        //
////////////////////////////////

d4 m4_c::longest_solve (void)
{
	d4 temp;
	queue <d4> nodes;
	
	clear_flag_all(F_TEMP);
	
	// save, mark as visited, push
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.w = w;
	set_flag(x,y,z,w,F_TEMP);
	nodes.push(temp);
	
	while (!nodes.empty()) {
		x = nodes.front().x;
		y = nodes.front().y;
		z = nodes.front().z;
		w = nodes.front().w;
		nodes.pop();

		// debug
		//print_all();
		//printf("%i %i %i %i\n",x,y,z,w);
		
		// check
		// XD
		if (can_move(XD) && !get_flag(x-1,y,z,w,F_TEMP)) {
			temp.x = x-1;
			temp.y = y;
			temp.z = z;
			temp.w = w;
			set_flag(x-1,y,z,w,F_TEMP);
			nodes.push(temp);
		}
		// XU
		if (can_move(XU) && !get_flag(x+1,y,z,w,F_TEMP)) {
			temp.x = x+1;
			temp.y = y;
			temp.z = z;
			temp.w = w;
			set_flag(x+1,y,z,w,F_TEMP);
			nodes.push(temp);
		}
		// YD
		if (can_move(YD) && !get_flag(x,y-1,z,w,F_TEMP)) {
			temp.x = x;
			temp.y = y-1;
			temp.z = z;
			temp.w = w;
			set_flag(x,y-1,z,w,F_TEMP);
			nodes.push(temp);
		}
		// YU
		if (can_move(YU) && !get_flag(x,y+1,z,w,F_TEMP)) {
			temp.x = x;
			temp.y = y+1;
			temp.z = z;
			temp.w = w;
			set_flag(x,y+1,z,w,F_TEMP);
			nodes.push(temp);
		}
		// ZD
		if (can_move(ZD) && !get_flag(x,y,z-1,w,F_TEMP)) {
			temp.x = x;
			temp.y = y;
			temp.z = z-1;
			temp.w = w;
			set_flag(x,y,z-1,w,F_TEMP);
			nodes.push(temp);
		}
		// ZU
		if (can_move(ZU) && !get_flag(x,y,z+1,w,F_TEMP)) {
			temp.x = x;
			temp.y = y;
			temp.z = z+1;
			temp.w = w;
			set_flag(x,y,z+1,w,F_TEMP);
			nodes.push(temp);
		}
		// WD
		if (can_move(WD) && !get_flag(x,y,z,w-1,F_TEMP)) {
			temp.x = x;
			temp.y = y;
			temp.z = z;
			temp.w = w-1;
			set_flag(x,y,z,w-1,F_TEMP);
			nodes.push(temp);
		}
		// WU
		if (can_move(WU) && !get_flag(x,y,z,w+1,F_TEMP)) {
			temp.x = x;
			temp.y = y;
			temp.z = z;
			temp.w = w+1;
			set_flag(x,y,z,w+1,F_TEMP);
			nodes.push(temp);
		}
	}
	
	// reset
	clear_flag_all(F_TEMP);
	
	// return final position
	return temp;
}

void m4_c::set_goal_simple (void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
	
	set_flag(x,y,z,w,F_STAR);
	
	set_flag(lenx-1,leny-1,lenz-1,lenw-1,F_GOAL);
}

void m4_c::set_goal_long (void)
{
	d4 temp;
	
	x = 0;
	y = 0;
	z = 0;
	w = 0;
	
	// ending block
	temp = longest_solve();
	x = temp.x;
	y = temp.y;
	z = temp.z;
	w = temp.w;
	set_flag(x,y,z,w,F_GOAL);	
	
	// starting block
	x = 0;
	y = 0;
	z = 0;
	w = 0;
	
	set_flag(x,y,z,w,F_STAR);	
}

void m4_c::set_goal_long_rand (void)
{
	d4 temp;
	unsigned int xs;
	unsigned int ys;
	unsigned int zs;
	unsigned int ws;	

	// choose a random point
	x = rand() % lenx;
	y = rand() % leny;
	z = rand() % lenz;
	w = rand() % lenw;

	// save original coordinates
	xs = x;
	ys = y;
	zs = z;
	ws = w;
	
	// ending block
	temp = longest_solve();
	x = temp.x;
	y = temp.y;
	z = temp.z;
	w = temp.w;
	set_flag(x,y,z,w,F_GOAL);	
	
	// starting block
	set_flag(xs,ys,zs,ws,F_STAR);	
	x = xs;
	y = ys;
	z = zs;
	w = ws;
}

void m4_c::set_goal_longest (void)
{
	d4 temp;
	
	x = rand() % lenx;
	y = rand() % leny;
	z = rand() % lenz;
	w = rand() % lenw;
	
	// ending block
	temp = longest_solve();
	x = temp.x;
	y = temp.y;
	z = temp.z;
	w = temp.w;
	set_flag(x,y,z,w,F_GOAL);	
	
	// starting block
	temp = longest_solve();
	x = temp.x;
	y = temp.y;
	z = temp.z;
	w = temp.w;
	set_flag(x,y,z,w,F_STAR);	
}
