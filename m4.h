/*
 * FILE:    m4.hpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
 */

#ifndef M4_H_
#define M4_H_

#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <cmath>

typedef unsigned int node_t;
typedef node_t dir_t; // so that it can share XD...

// wall flags
// D down
// U up
#define XD 0x00000001
#define YD 0x00000002
#define ZD 0x00000004
#define WD 0x00000008
#define XU 0x00000010
#define YU 0x00000020
#define ZU 0x00000040
#define WU 0x00000080
// other flags
#define F_TEMP 0x00000100 // temporary marker, use for anything
#define F_GOAL 0x00000200 // ending block
#define F_DISC 0x00000400 // block discovered and visible to user
#define F_STAR 0x00000800 // starting block

// maze maximum size
#define LEN_MIN 1
#define LEN_MAX 40

// dimension swap
#define DIMX 0
#define DIMY 1
#define DIMZ 2
#define DIMW 3

// number of directions
#define DIRS1 2
#define DIRS2 4
#define DIRS3 6
#define DIRS4 8

// maze algorithm
enum alg_t {
	ALG_RAND,
	ALG_DEPTH,
	ALG_BREAD,
	ALG_HUNT,
	ALG_BOX,
	ALG_CAGE,
	ALG_DEFAULT
};

enum goal_t {
	G_SIMPLE,
	G_LONG,
	G_LONG_RAND,
	G_LONGEST,
	G_DEFAULT
};

// discovery
enum disc_t {
	S_FULL,
	S_RANGE,
	S_RANGE_DISC,
	S_LOS,
	S_LOS_DISC,
	S_LOS_RANGE,
	S_LOS_RANGE_DISC,
	S_DEFAULT
};
#define RANGE 2

typedef struct d4_t {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
} d4_t;

typedef struct prim_t {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
	dir_t dir;
} prim_t;

class m4_c {
	public:
		//////////////// CLASS ////////////////
		m4_c (void);
		~m4_c (void);
		
		//////////////// MAKE NEW MAZE ////////////////
		void make_maze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws);
		void make_maze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs);
		void make_maze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights);
		void make_maze (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights, goal_t goal);

		//////////////// MOVE ////////////////
		void move(dir_t dir);
		// dir one of XD, XU, etc
		bool can_move (dir_t dir);
		bool can_move (unsigned int i, unsigned int j, unsigned int k, unsigned int h, dir_t dir);
		// position
		bool set_x (unsigned int nx);
		bool set_y (unsigned int ny);
		bool set_z (unsigned int nz);
		bool set_w (unsigned int nw);

		//////////////// DIMENSION SWAP ////////////////
		void d_swap_abs (unsigned int d1, unsigned int d2);
		void d_swap_rel (unsigned int d1, unsigned int d2);
		void discover (void);
		
		//////////////// GET ////////////////
		// lengths
		unsigned int get_lenx(void);
		unsigned int get_leny(void);
		unsigned int get_lenz(void);
		unsigned int get_lenw(void);
		// position
		unsigned int get_x(void);
		unsigned int get_y(void);
		unsigned int get_z(void);
		unsigned int get_w(void);
		// axes
		unsigned int get_a0(void);
		unsigned int get_a1(void);
		unsigned int get_a2(void);
		unsigned int get_a3(void);		
		// flags
		bool get_flag (node_t flag);
		bool get_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag);
		
	private:
		//////////////// VARIABLES ////////////////
		unsigned int lenx, leny, lenz, lenw;
		node_t**** arry;
		disc_t sight;
		unsigned int a0, a1, a2, a3;
		unsigned int x,y,z,w;

		//////////////// ALLOCATION ////////////////
		void gen (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws);
		void degen (void);
		
		//////////////// BOXES ////////////////
		void frame (void);
		void box (void);
		void cage (void);
		
		//////////////// DISCOVERY ////////////////
		void disc_line_of_sight (void);
		void disc_line_of_sight_ranged (unsigned int range);
		void disc_ranged (unsigned int range);
		
		//////////////// CHECKS ////////////////
		bool valid (void);
		bool valid (unsigned int i, unsigned int j, unsigned int k, unsigned int h);
		void set_flag (node_t flag);
		void set_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag);
		void set_flag_all (node_t flag);
		void clear_flag (node_t flag);
		void clear_flag (unsigned int i, unsigned int j, unsigned int k, unsigned int h, node_t flag);
		void clear_flag_all (node_t flag);
		// HULK SMASH!
		bool smash (dir_t dir);
		bool smash (unsigned int i, unsigned int j, unsigned int k, unsigned int h, dir_t dir);

		//////////////// SOLVERS ////////////////
		int depth_solve (void);
		int rec_depth_solve (void);
		bool breadth_solve (void);
		
		//////////////// BUILDERS ////////////////
		void random_build (void);
		void depth_build (void);
		void rec_depth_build (void);
		void breadth_build (void);
		void hunt_and_kill_build (void);
		void hunt_and_kill_build (unsigned int len);
		bool kill (void);
		bool kill_node (void);
		
		//////////////// GOAL ////////////////
		d4_t longest_solve (void);
		void set_goal_simple (void);
		void set_goal_long (void);
		void set_goal_long_rand (void);
		void set_goal_longest (void);
};

#endif /* M4_H_ */
