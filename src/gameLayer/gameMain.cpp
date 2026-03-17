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
	gameData.gameMap.create(30, 30);

	for (int y = 0; y < gameData.gameMap.h; y++)
		for (int x = 0; x < gameData.gameMap.w; x++)
		{

			float s = (std::sin(x) + 1.f) / 2.f;

			if (gameData.gameMap.h - (gameData.gameMap.h * 0.3 * s) - gameData.gameMap.h * 0.5 < y)
			{
				gameData.gameMap.getBlockUnsafe(x, y).type = Block::grassBlock;
			}
			else
			{
				gameData.gameMap.getBlockUnsafe(x, y).type = Block::air;
			}
		}


	gameData.camera.target = { 0,0 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 50.0f;

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

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			*b = {};
		}
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			if (IsKeyPressed(KEY_ONE))
				b->type = Block::glass;
			else if (IsKeyPressed(KEY_TWO))
				b->type = Block::sand;
			else if (IsKeyPressed(KEY_THREE))
				b->type = Block::rubyBlock;
		}
	}


	#pragma region draw world

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

		//gamedata.gamemap
	}

	//DrawText("My game", 100, 200, 26, RED);

	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height },//source
		{ (float)blockX,(float)blockY,1,1 },//destination
		{ 0,0 },//origin
		0.0f,
		WHITE//tint
	);

	#pragma endregion

	EndMode2D();
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
