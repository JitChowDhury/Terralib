#include <raylib.h>
#include <iostream>
#include "gameMain.h"

struct GameData
{
	float positionX = 100;
	float positionY = 100;
}gameData;


bool initGame()
{
	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();

	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; }

	//DrawText("My game", 100, 200, 26, RED);

	if (IsKeyDown(KEY_A)) { gameData.positionX -= 100 * deltaTime; }
	if (IsKeyDown(KEY_D)) { gameData.positionX += 100 * deltaTime; }
	if (IsKeyDown(KEY_W)) { gameData.positionY -= 100 * deltaTime; }
	if (IsKeyDown(KEY_S)) { gameData.positionY += 100 * deltaTime; }

	DrawRectangle(gameData.positionX, gameData.positionY, 80, 80, {172,67,235,255});
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
