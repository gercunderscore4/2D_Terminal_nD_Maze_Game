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
	//printf("Hello, brave new world!\n");
	
	// game
	m4 maze;
	maze.play();
	//maze.play(3,4,2,1);
	//maze.do_stuff();
	//maze.test(10, 64,64,64,64); // should take 15 minutes on Moto X
	
	//printf("\nGood-bye, cruel world!\n");
	return 0;
}
