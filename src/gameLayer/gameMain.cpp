#include <raylib.h>
#include <iostream>
#include "gameMain.h"
#include "assetManager.h"
#include "gameMap.h"
#include "helpers.h"



struct GameData
{
	GameMap gameMap;
	Camera2D camera;
}gameData;

AssetManager assetManager;
 
bool initGame()
{
	assetManager.loadAll();
	gameData.gameMap.create(30, 10);

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::ironBlock;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::grass;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::glass;
	gameData.gameMap.getBlockUnsafe(4, 4).type = Block::blueRubyBlock;

	gameData.camera.target = { 0,0 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; }

	gameData.camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };

	ClearBackground(BLACK);


	#pragma region cameraMovement

	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= 5.f * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += 5.f * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= 5.f * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += 5.f * deltaTime;

	#pragma endregion

	BeginMode2D(gameData.camera);

	for (int y = 0; y < gameData.gameMap.h; y++)
	{
		for (int x = 0; x < gameData.gameMap.w; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air)
			{
				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(b.type,0,32,32),
					{ (float)x,(float)y,1,1 },
					{ 0,0 },
					0.0f,
					WHITE
				);
			}
		}
	}

	//DrawText("My game", 100, 200, 26, RED);
	EndMode2D();
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
