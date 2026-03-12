#include <raylib.h>
#include <iostream>
#include "gameMain.h"

bool initGame()
{
	return true;
}

bool updateGame()
{

	DrawText("My game", 100, 200, 26, RED);
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
