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
	Camera2D camera = {};
	int creativeSelectedBlock = Block::dirt;
}gameData;

AssetManager assetManager;

bool showImgui = false;
 
bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap,32442321233);

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

	ClearBackground({75,75,150,255});
	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

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

	if (gameData.creativeSelectedBlock < 0) { gameData.creativeSelectedBlock = 0; }
	if (gameData.creativeSelectedBlock >= Block::BLOCKS_COUNT) { gameData.creativeSelectedBlock = Block::BLOCKS_COUNT - 1; }

	if (!showImgui)
	{
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
				b->type = gameData.creativeSelectedBlock;
			}
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
	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);

	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);


	

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

	if (showImgui)
	{
		ImGui::Begin("Game Control");
		ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 2, 150);
		ImGui::SliderFloat("Camera speed:", &CAMERA_SPEED, 10, 150);

		ImGui::Separator();
		/*ImGui::InputInt("Select Block", &gameData.creativeSelectedBlock);*/
		for (int i = 0; i < Block::BLOCKS_COUNT; i++)
		{
			auto atlas = getTextureAtlas(i, 0, 32, 32);
			atlas.x /= assetManager.textures.width;
			atlas.width /= assetManager.textures.width;
			atlas.y /= assetManager.textures.height;
			atlas.height /= assetManager.textures.height;

			ImGui::PushID(i);

			ImTextureID tex = (ImTextureID)(intptr_t)assetManager.textures.id;
			if (ImGui::ImageButton(tex,
				{ 35, 35 },
				{ atlas.x, atlas.y },
				{ atlas.x + atlas.width, atlas.y + atlas.height }))
			{
				gameData.creativeSelectedBlock = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
			{
				ImGui::SameLine();
			}
		}
		ImGui::End();

	}



	DrawFPS(20, 20);
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
