#include "raylib.h"
#include <iostream>

#include <imgui.h>
#include <rlImGui.h>

using namespace std;

int main()
{
	SetTargetFPS(60);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Terraria");

	rlImGuiSetup(true);

	int posX = 30;
	int posY = 40;
	int size = 100;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		rlImGuiBegin();

		DrawText("My game", 100, 200,26, RED);
		DrawRectangle(posX, posY, size, size, BLUE);
		DrawRectangle(posX, posY, size, size, {210,59,207,127 });
		posX += 1;

		ImGui::Begin("test");
		ImGui::Text("Hello");
		ImGui::Button("button");
		ImGui::ShowDemoWindow();
		ImGui::End();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}