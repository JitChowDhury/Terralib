#include "raylib.h"
#include <iostream>

using namespace std;

int main()
{
	SetTargetFPS(60);
	InitWindow(800, 450, "Terraria");

	int posX = 30;
	int posY = 40;
	int size = 100;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawRectangle(posX, posY, size, size, BLUE);
		posX += 1;
		EndDrawing();
	}
}