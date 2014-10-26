/*
 * FILE:    m4.hpp
 * PURPOSE: 4D maze class
 * AUTHOR:  Geoffrey Card
 * DATE:    ????-??-?? - 
 * NOTES:   
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "m4.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#define CLR_LEN 100

// all tiles must be equal size
#define SPACE  "  "
#define WALL   "▓▓"
#define USER   "[]"
#define GOAL   "()"
#define TEMP   "::"
#define HSKIP  "  "
#define VSKIP  "\n"
#define UNDISC ".."

// controls
#define RESET '`'
// M minus --
// P plus ++
#define XM 'a'
#define XP 'd'
#define YM 'w'
#define YP 's'
#define ZM 'q'
#define ZP 'e'
#define WM 'z'
#define WP 'c'
// d-swap controls
#define DSWAPX '0'
#define DSWAPY '1'
#define DSWAPZ '2'
#define DSWAPW '3'

class engine_c: protected m4_c {
	public:
		//////////////// CLASS ////////////////
		engine_c (void);
		~engine_c (void);
		
		//////////////// META ////////////////
		void play (void);
		void play (unsigned int lxs, unsigned int lys, unsigned int lzs, unsigned int lws, alg_t algs, disc_t sights, goal_t goal);
		void test (void);
		void tutorial (void);
		
	private:
		//////////////// CONTROLS ////////////////
		// user input
		bool get_size_and_make (void);
		bool control (void);
		
		//////////////// GRAPHICS ////////////////
		void print_sight (void);
		void print_clr (void);
		void print_title_screen (void);
		void print_man (void);
		void print_all (void);
		bool disc_s1r1c1b1 (unsigned int aint[4]);
		bool disc_s1r1c1b2 (unsigned int aint[4]);
		bool disc_s1r1c1b3 (unsigned int aint[4]);
		bool disc_s1r2c1b1 (unsigned int aint[4]);
		bool disc_s1r2c1b2 (unsigned int aint[4]);
		bool disc_s1r2c1b3 (unsigned int aint[4]);
		bool disc_s1r3c1b1 (unsigned int aint[4]);
		bool disc_s1r3c1b2 (unsigned int aint[4]);
		bool disc_s1r3c1b3 (unsigned int aint[4]);
		bool disc_s1r1c2b1 (unsigned int aint[4]);
		bool disc_s1r1c2b2 (unsigned int aint[4]);
		bool disc_s1r1c2b3 (unsigned int aint[4]);
		bool disc_s1r2c2b1 (unsigned int aint[4]);
		bool disc_s1r2c2b2 (unsigned int aint[4]);
		bool disc_s1r2c2b3 (unsigned int aint[4]);
		bool disc_s1r3c2b1 (unsigned int aint[4]);
		bool disc_s1r3c2b2 (unsigned int aint[4]);
		bool disc_s1r3c2b3 (unsigned int aint[4]);
		bool disc_s2r1c1b1 (unsigned int aint[4]);
		bool disc_s2r1c1b2 (unsigned int aint[4]);
		bool disc_s2r1c1b3 (unsigned int aint[4]);
		bool disc_s2r2c1b1 (unsigned int aint[4]);
		bool disc_s2r2c1b2 (unsigned int aint[4]);
		bool disc_s2r2c1b3 (unsigned int aint[4]);
		bool disc_s2r3c1b1 (unsigned int aint[4]);
		bool disc_s2r3c1b2 (unsigned int aint[4]);
		bool disc_s2r3c1b3 (unsigned int aint[4]);
		void print_disc (void);
		void print_data (void);
};

#endif /* ENGINE_H_ */
