#include "raylib.h"
#include <iostream>

#include <imgui.h>
#include <rlImGui.h>
#include <gameMain.h>

int main()
{
	#if PRODUCTION_BUILD==1
	SetTraceLogLevel(LOG_NONE); 
	#endif


	//SetTargetFPS(240);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Terraria");
	SetExitKey(KEY_NULL);

	#pragma region imgui
	rlImGuiSetup(true);
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2;
	#pragma endregion

	int posX = 30;
	int posY = 40;
	int size = 100;

	if (!initGame())
	{
		return 0;
	}


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		#pragma region imgui
		rlImGuiBegin();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2); 
		#pragma endregion

		if (!updateGame())
		{
			CloseWindow();
		}
		    
		posX += 1;

		#pragma region imgui
		rlImGuiEnd();
		#pragma endregion 
		EndDrawing();
	}
	#pragma region imgui
	rlImGuiShutdown();
	#pragma endregion
	CloseWindow();
	closeGame();
}