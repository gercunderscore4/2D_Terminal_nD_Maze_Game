/*
 * FILE:    m.cpp
 * PURPOSE: 4D maze game
 * AUTHOR:  Geoffrey Card
 * DATE:    2014-02-06 - 2014-02-06
 * NOTES:   Controls are un-intuitive.
 *          Graphics are too big.
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "m4.hpp"

int main (void)
{
	//printf("Hello, brave new world!\n\n");
	srand((int) time(NULL));
	
	// clear screen
	for (int i = 0; i < 200; i++) {
		printf("\n");
	}
	
	// title screen
	printf(
		"2D Terminal 4D Maze Game\n"
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
	for (int i = 0; i < 200; i++) {
		printf("\n");
	}
	m4 maze;
	maze.test();
	
	//printf("\n\nGood-bye, cruel world!\n");
	return 0;
}

