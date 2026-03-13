#include <raylib.h>
#include <iostream>
#include "gameMain.h"
#include "assetManager.h"



struct GameData
{

}gameData;

AssetManager assetManager;
 
bool initGame()
{
	assetManager.loadAll();
	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();

	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; }

	//DrawText("My game", 100, 200, 26, RED);
	DrawTexturePro(assetManager.dirt, { 0,0,(float)assetManager.dirt.width,(float)assetManager.dirt.height }, { 100,100,50,50 }, {}, 0, WHITE);
	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}
