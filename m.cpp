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
#include "m4.cpp"

int main (void)
{
	//printf("Hello, brave new world!\n\n");
	srand((int) time(NULL));
	
	printf(
		"2D terminal 4D maze game\n"
		"(3D window 4D maze game\n"
		"\n"
		"press enter");
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
