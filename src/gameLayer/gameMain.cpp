#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "gameMain.h"
#include "assetManager.h"
#include "gameMap.h"
#include "helpers.h"
#include "worldGenerator.h"
#include "imgui.h"
#include "structure.h"
#include "saveMap.h"



struct GameData
{
	GameMap gameMap;
	Camera2D camera = {};
	int creativeSelectedBlock = Block::dirt;

	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};
	Structure copyStructure;

	char saveName[100] = {};
	
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
	
	if (showImgui)
	{
		if (IsKeyPressed(KEY_ONE)) { gameData.selectionStart = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_TWO)) { gameData.selectionEnd = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_THREE))
		{  
			gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{ (float)blockX,(float)blockY });
		}

		if (gameData.selectionStart.x > gameData.selectionEnd.x)
		{
			std::swap(gameData.selectionStart.x, gameData.selectionEnd.x);
		}

		if (gameData.selectionStart.y > gameData.selectionEnd.y)
		{
			std::swap(gameData.selectionStart.y, gameData.selectionEnd.y);
		}
	}
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

	//#pragma region draw background

	//Camera2D bgCamera = gameData.camera;
	//bgCamera.zoom = 1.0f;
	//bgCamera.offset = gameData.camera.offset;

	//BeginMode2D(bgCamera);

	//// visible world size
	//float viewWidth = GetScreenWidth() / gameData.camera.zoom;
	//float viewHeight = GetScreenHeight() / gameData.camera.zoom;

	//Vector2 camTarget = gameData.camera.target;

	//// top-left of camera view
	//Vector2 topLeft = {
	//	camTarget.x - viewWidth / 2,
	//	camTarget.y - viewHeight / 2
	//};

	//Rectangle dest = {
	//	topLeft.x,
	//	topLeft.y,
	//	viewWidth,
	//	viewHeight
	//};

	//DrawTexturePro(
	//	assetManager.bg,
	//	{ 0,0,(float)assetManager.bg.width,(float)assetManager.bg.height },
	//	dest,
	//	{ 0,0 },
	//	0,
	//	WHITE
	//);

	//EndMode2D();

	//#pragma endregion

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

	if (showImgui)
	{
		Rectangle rect;
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;
		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(rect, 0.1, { 20,101,250,145 });
	}

	#pragma endregion

	EndMode2D();

	if (showImgui)
	{
		ImGui::Begin("Game Control");
		ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 2, 150);
		ImGui::SliderFloat("Camera speed:", &CAMERA_SPEED, 10, 150);

		if (ImGui::Button("Copy"))
		{
			gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
		}
		ImGui::InputText("File name", gameData.saveName, sizeof(gameData.saveName));

		if (ImGui::Button("Save to file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			saveBlockDataToFile(
				gameData.copyStructure.mapData,
				gameData.copyStructure.w,
				gameData.copyStructure.h,
				path.c_str()
			);
		}

		if (ImGui::Button("Load from file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			loadBlockDataFromFile(
				gameData.copyStructure.mapData,
				gameData.copyStructure.w,
				gameData.copyStructure.h,
				path.c_str()
			);
		}
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
