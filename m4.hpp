///////////////////////////////
// BEG M4.HPP
////////////////////////////////

#ifndef M4_H_
#define M4_H_

class m4 {
	public:
		// maze dimensions
		int lenx, leny, lenz, lenw;

		// maze data
		// total size is [lenx+1][leny+1][lenz+1][lenw+1]
		char**** arry;
	
		// user location
		int x, y, z, w;
	
		// goal coordinates
		int gx, gy, gz, gw;

		// test stuff here
		void test (void);

		// constructor
		m4 (void);
		// destructor
		~m4 (void);
		
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
		
		// user input
		void get_size (void);
		bool control (void);

		// checks
		bool inline valid (int i, int j, int k, int h);
		void inline set_flag (char flag);
		void inline set_flag (int i, int j, int k, int h, char flag);
		void inline clear_flag (char flag);
		void inline clear_flag (int i, int j, int k, int h, char flag);
		void inline clear_flag_all (char flag);
		bool inline has_flag (char flag);
		bool inline has_flag (int i, int j, int k, int h, char flag);
		// dir one of XD, XU, etc
		bool inline can_move (char dir);
		bool inline can_move (int i, int j, int k, int h, char dir);
		// HULK SMASH!
		bool smash (char dir);
		bool smash (int i, int j, int k, int h, char dir);
		
		// maze algorithms
		// random, returns solvability
		void random (void);
		int rec_depth_solve (void);
		int rec_breadth_solve (void);
		
};

#endif // M4_H_

////////////////////////////////
// END M4.HPP
////////////////////////////////
