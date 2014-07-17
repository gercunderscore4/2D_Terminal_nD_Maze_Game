#ifndef MN_HPP_
#define MN_HPP_

#define CLR_LEN 100

// all tiles must be equal size
#define SPACE "  "
#define WALL  "▓▓"
#define USER  "[]"
#define GOAL  "()"
#define TEMP  "::"
#define HSKIP "  "
#define VSKIP "\n"
#define UNDIS ".."

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
#define F_TEMP 0x00000100
#define F_GOAL 0x00000200
#define F_DISC 0x00000400 // block discovered

// maze maximum size
#define LEN_MIN 1
#define LEN_MAX 100

// controls
#define RESET ' '
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
#define DIRS1 2
#define DIRS2 4
#define DIRS3 6
#define DIRS4 8

// dimension swap
#define DIMX 0
#define DIMY 1
#define DIMZ 2
#define DIMW 3
// d-swap controls
#define DSWAPX '0'
#define DSWAPY '1'
#define DSWAPZ '2'
#define DSWAPW '3'

#endif // MN_HPP_
