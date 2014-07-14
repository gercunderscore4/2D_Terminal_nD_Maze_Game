/*
 * FILE:    m.cpp
 * PURPOSE: 4D maze game
 * AUTHOR:  Geoffrey Card
 * DATE:    2014-02-06 - 2014-07-13
 * NOTES:   Controls are un-intuitive.
 *          Graphics are too big.
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "mn.hpp"
#include "m4.hpp"

int main (void)
{
	//printf("Hello, brave new world!\n\n");
	//srand((int) time(NULL));
	
	/*
	// clear screen
	for (int i = 0; i < CLR_LEN; i++) {
		printf("\n");
	}
	
	// title screen
	printf(
		"2D Terminal nD Maze Game\n"
		"                        \n"
		"   By Geoffrey Card     \n"
		"                        \n"
		"      press enter       \n"
		"                        \n"
	);
	// press enter
	char c = 0;
	do {
		c = getchar();
	} while (c != '\n');
	*/
	
	// game
	m4 maze;
	//maze.play();
	maze.play(3,4,2,1);
	//maze.do_stuff();
	
	//printf("\n\nGood-bye, cruel world!\n");
	return 0;
}
