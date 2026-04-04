#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "gameMain.h"
#include "assetManager.h"
#include "gameMap.h"
#include "helpers.h"
#include "worldGenerator.h"
#include "imgui.h"



struct GameData
{
	GameMap gameMap;
	Camera2D camera;
}gameData;

AssetManager assetManager;
 
bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap,678912213);

	gameData.camera.target = { 450.0f, 80.0f };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 5.0f;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; }
	
	gameData.camera.offset = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f };

	ClearBackground(BLACK);


	#pragma region cameraMovement

	static float CAMERA_SPEED = 10;

	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += CAMERA_SPEED * deltaTime;

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

	Vector2 topLeftView = GetScreenToWorld2D({ 0,0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({(float)GetScreenWidth(),(float)GetScreenHeight()}, gameData.camera);

	int startXView = (int)floorf(topLeftView.x - 1);
	int endXView = (int)ceilf(bottomRightView.x + 1);
	int startYView = (int)floorf(topLeftView.y - 1);
	int endYView = (int)ceilf(bottomRightView.y + 1);

	startXView = Clamp(startXView, 0, gameData.gameMap.w- 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w+ 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h + 1);


	

	for (int y = startYView; y <=endYView; y++)
	{
		for (int x = startXView; x <=endXView; x++)
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

	ImGui::Begin("Game Control");
	ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 10, 150);
	ImGui::SliderFloat("Camera speed:", &CAMERA_SPEED, 10, 150);

	ImGui::End();


	DrawFPS(20, 20);
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
