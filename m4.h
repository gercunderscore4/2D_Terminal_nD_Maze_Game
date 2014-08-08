/*
 * FILE:    m4.hpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-11
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
 */

#ifndef M4_H_
#define M4_H_

#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdint>

typedef uint32_t node_t;
typedef node_t dir_t; // so that it can share XD...
typedef uint32_t pos_t;

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
#define LEN_MAX 50 // cannot be greater than 255, or pos_t size must change

// directions as numbers
#define DIRS1 2
#define DIRS2 4
#define DIRS3 6
#define DIRS4 8

// dimension swap
#define DIMX 0
#define DIMY 1
#define DIMZ 2
#define DIMW 3

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
#define RANGE 1

// for storing 3D points
typedef struct d3 {
	int x;
	int y;
	int z;
} d3;

// for storing 4D points
typedef struct d4 {
	int x;
	int y;
	int z;
	int w;
} d4;

// for storing points and direction of origin
typedef struct prim {
	pos_t node;
	int x;
	int y;
	int z;
	int w;
	dir_t dir;
} prim;

class m4 {
	public:
		//////////////// CLASS ////////////////
		m4 (void);
		~m4 (void);
		
		//////////////// MAKE NEW MAZE ////////////////
		void makeMaze (int lxs, int lys, int lzs, int lws);
		void makeMaze (int lxs, int lys, int lzs, int lws, alg_t algs);
		void makeMaze (int lxs, int lys, int lzs, int lws, alg_t algs, disc_t sights);
		void makeMaze (int lxs, int lys, int lzs, int lws, alg_t algs, disc_t sights, goal_t goal);

		//////////////// DIMENSION SWAP ////////////////
		void d_swap_abs (int d1, int d2);
		void d_swap_rel (int d1, int d2);

		//////////////// MOVE ////////////////
		void move(dir_t dir);
		
		void discover(void);
		
		//////////////// GET ////////////////
		// lengths
		int get_lenx(void);
		int get_leny(void);
		int get_lenz(void);
		int get_lenw(void);
		// position
		int get_x(void);
		int get_y(void);
		int get_z(void);
		int get_w(void);
		// discovery
		disc_t get_sight(void);
		// axes
		int get_a0(void);
		int get_a1(void);
		int get_a2(void);
		int get_a3(void);		
		// flags
		bool get_flag (node_t flag);
		bool get_flag (int i, int j, int k, int h, node_t flag);
		
		// user location
		int x,y,z,w;

		/*
		m4();
		{
			makeMaze();
			while (get_flag(F_GOAL) == false) {
				
				move -> discover
				OR
				d_swap
			}
		}	
		~m4
		*/
	private:
		//////////////// VARIABLES ////////////////
		// maze dimensions
		int lenx, leny, lenz, lenw;
		// maze data
		// total size is [lenx][leny][lenz][lenw]
		node_t**** arry;
		// sight algorithm
		disc_t sight;
		// axes
		int a0, a1, a2, a3;

		//////////////// ALLOCATION ////////////////
		void gen (void);
		void degen (void);
		
		//////////////// BOXES ////////////////
		// closes off maze, does not interfere with internal walls
		void frame (void);
		// open space inside
		void box (void);
		// no space inside
		void cage (void);
		// empty
		void empty (void);
		
		//////////////// DISCOVERY ////////////////
		void disc_line_of_sight (void);
		void disc_line_of_sight_ranged (int range);
		void disc_ranged (int range);
		//////////////// CHECKS ////////////////
		// checks
		bool inline valid (void);
		bool inline valid (int i, int j, int k, int h);
		void inline set_flag (node_t flag);
		void inline set_flag (int i, int j, int k, int h, node_t flag);
		void inline set_flag_all (node_t flag);
		void inline clear_flag (node_t flag);
		void inline clear_flag (int i, int j, int k, int h, node_t flag);
		void inline clear_flag_all (node_t flag);
		bool inline has_flag (node_t flag);
		bool inline has_flag (int i, int j, int k, int h, node_t flag);
		// dir one of XD, XU, etc
		bool inline can_move (dir_t dir);
		bool inline can_move (int i, int j, int k, int h, dir_t dir);
		// HULK SMASH!
		bool smash (dir_t dir);
		bool smash (int i, int j, int k, int h, dir_t dir);

		//////////////// SOLVERS ////////////////
		// solvers
		int depth_solve (void);
		int rec_depth_solve (void);
		int breadth_solve (void);
		
		//////////////// BUILDERS ////////////////
		// maze algorithms
		void random_build (void);
		void depth_build (void);
		void rec_depth_build (void);
		void breadth_build (void);
		void hunt_and_kill_build (void);
		void hunt_and_kill_build (int len);
		bool kill (void);
		bool kill_node (void);
		
		//////////////// GOAL ////////////////
		d4 longest_solve (void);
		void set_goal_simple (void);
		void set_goal_long (void);
		void set_goal_long_rand (void);
		void set_goal_longest (void);
};

#endif /* M4_H_ */
