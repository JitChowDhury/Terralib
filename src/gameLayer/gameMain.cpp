#include <raylib.h>
#include <iostream>
#include "gameMain.h"
#include "assetManager.h"
#include "gameMap.h"



struct GameData
{
	GameMap gameMap;
}gameData;

AssetManager assetManager;
 
bool initGame()
{
	assetManager.loadAll();
	gameData.gameMap.create(30, 10);

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(4, 4).type = Block::dirt;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; }

	ClearBackground(BLACK);

	for (int y = 0; y < gameData.gameMap.h; y++)
	{
		for (int x = 0; x < gameData.gameMap.w; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air)
			{
				float size = 32;
				float posX = x * size;
				float posY = y * size;

				DrawTexturePro(
					assetManager.dirt,
					Rectangle{ 0.f,0.f,(float)assetManager.dirt.width,(float)assetManager.dirt.height },
					{ posX,posY,size,size },
					{ 0,0 },
					0.0f,
					WHITE
				);
			}
		}
	}

	//DrawText("My game", 100, 200, 26, RED);

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
