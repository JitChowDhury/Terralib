#include "raylib.h"
#include <iostream>

using namespace std;

int main()
{
	SetTargetFPS(60);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Terraria");

	int posX = 30;
	int posY = 40;
	int size = 100;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawRectangle(posX, posY, size, size, BLUE);
		DrawText("My game", 100, 200,26, RED);
		posX += 1;
		EndDrawing();
	}
}