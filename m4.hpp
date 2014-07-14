/*
 * FILE:    m4.hpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-11
 * NOTES:   d-swap cannot handle multiples of a dimension (an == am)
 */

#ifndef M4_HPP_
#define M4_HPP_

#include <cstdint>
typedef uint32_t node_t;
typedef char dir_t; // so that it can be mapped to keys

typedef struct prim {
	unsigned int node;
	dir_t dir;
} prim;

class m4 {
	public:
		// maze dimensions
		int lenx, leny, lenz, lenw;

		// maze data
		// total size is [lenx][leny][lenz][lenw]
		node_t**** arry;
	
		// user location
		int x,y,z,w;
	
		// goal coordinates
		int gx,gy,gz,gw;
		
		// axes
		int a0, a1, a2, a3;

		// constructor
		m4 (void);
		// destructor
		~m4 (void);
		
		// test stuff here
		void do_stuff (void);
		void play (void);
		void play (int xs, int ys, int zs, int ws);
		void test (int n, int xs, int ys, int zs, int ws);

		// allocate and deallocate maze data
		void gen (void);
		void degen (void);
		
		// contructicon
		// these may destroy any non-wall flags
		// closes off maze, does not interfere with internal walls
		void frame (void);
		// open space inside
		void box (void);
		// no space inside
		void cage (void);
		// empty
		void empty (void);
		
		// print
		void print_clr (void);
		void print_all (void);
		void print_man (void);
		void print_data (void);
		
		// user input
		void get_size (void);
		bool control (void);
		// dimension swap
		void d_swap (int d1, int d2);

		// checks
		bool inline valid (void);
		bool inline valid (int i, int j, int k, int h);
		void inline set_flag (node_t flag);
		void inline set_flag (int i, int j, int k, int h, node_t flag);
		void inline clear_flag (node_t flag);
		void inline clear_flag (int i, int j, int k, int h, node_t flag);
		void inline clear_flag_all (node_t flag);
		bool inline has_flag (node_t flag);
		bool inline has_flag (int i, int j, int k, int h, node_t flag);
		// dir one of XD, XU, etc
		bool inline can_move (char dir);
		bool inline can_move (int i, int j, int k, int h, char dir);
		// HULK SMASH!
		bool smash (char dir);
		bool smash (int i, int j, int k, int h, char dir);

		// solvers
		int rec_depth_solve (void);
		int breadth_solve (void);
		
		// maze algorithms
		// random, returns solvability
		void random_build (void);
		void depth_build (void);
		void rec_depth_build (void);
		void breadth_build (void);
		void hunt_and_kill_build (void);
		bool kill (void);
		bool kill_node (void);
};

#endif // M4_HPP_
