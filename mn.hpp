#ifndef MN_H_
#define MN_H_

#define CLR_LEN 100

// all tiles must be equal size
#define SPACE "  "
#define WALL  "▓▓"
#define USER  "[]"
#define GOAL  "()"
#define TEMP  "::"
#define HSKIP "  "
#define VSKIP "\n"

// wall flags
// D down
// U up
#define XD 0x01
#define YD 0x02
#define ZD 0x04
#define WD 0x08
// other flags
#define F_TEMP 0x10
#define F_GOAL 0x20

// maze maximum size
#define SIZE_MIN 1
#define SIZE_MAX 100

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

#endif // MN_H_