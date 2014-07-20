/*
 * FILE:    m4.hpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 2014-07-11
 * NOTES:   print_all cannot handle multiples of a dimension (an == am)
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
		//////////////// CLASS ////////////////
		m4 (void);
		~m4 (void);
		
		//////////////// META ////////////////
		void do_stuff (void);
		void play (void);
		void play (int xs, int ys, int zs, int ws, int algs);
		void test (int n, int xs, int ys, int zs, int ws);
		void tutorial (void);
		
	private:
		//////////////// VARIABLES ////////////////
		// maze dimensions
		int lenx, leny, lenz, lenw;
		// maze data
		// total size is [lenx][leny][lenz][lenw]
		node_t**** arry;
		// user location
		int x,y,z,w;
		// goal coordinates
		int gx,gy,gz,gw;
		// maze algorithm
		int alg;
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
		
		//////////////// GRAPHICS ////////////////
		void print_clr (void);
		void print_title_screen (void);
		void print_man (void);
		void print_all (void);
		void print_disc (void);
		void print_data (void);
		void disc_line_of_sight (void);
		void disc_line_of_sight_ranged (int range);
		void disc_ranged (int range);

		//////////////// CONTROLS ////////////////
		// user input
		void get_size (void);
		void build_maze(int algs);
		bool control (void);
		// dimension swap
		void d_swap_abs (int d1, int d2);
		void d_swap_rel (int d1, int d2);

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
		bool inline can_move (char dir);
		bool inline can_move (int i, int j, int k, int h, char dir);
		// HULK SMASH!
		bool smash (char dir);
		bool smash (int i, int j, int k, int h, char dir);

		//////////////// SOLVERS ////////////////
		// solvers
		int depth_solve (void);
		int rec_depth_solve (void);
		int breadth_solve (void);
		
		//////////////// BUILDERS ////////////////
		// maze algorithms
		// random, returns solvability
		void random_build (void);
		void random_solved_build (void);
		void depth_build (void);
		void rec_depth_build (void);
		void breadth_build (void);
		void hunt_and_kill_build (void);
		bool kill (void);
		bool kill_node (void);
};

#endif // M4_HPP_
