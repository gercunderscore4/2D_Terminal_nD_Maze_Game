/*
 * FILE:    main.cpp
 * PURPOSE: 4D maze game
 * AUTHOR:  Geoffrey Card
 * DATE:    2014-02-06 - 2014-07-13
 * NOTES:   Controls are un-intuitive.
 *          Graphics are too big.
 */

#include <cstdlib>
#include <ctime>
#include "engine.h"

int main (void)
{
	srand((int) time(NULL));
	
	// game
	engine_c engine;
	engine.play();
	//engine.play(10,10,1,1,ALG_HUNT,S_LOS_DISC,G_LONGEST);
	//engine.test();
	//engine.tutorial();

	return 0;
}
