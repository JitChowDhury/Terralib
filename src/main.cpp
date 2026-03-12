#include "raylib.h"
#include <iostream>

#include <imgui.h>
#include <rlImGui.h>


int main()
{
	SetTargetFPS(60);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Terraria");

	#pragma region imgui
	rlImGuiSetup(true);
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2;
	#pragma endregion

	int posX = 30;
	int posY = 40;
	int size = 100;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		rlImGuiBegin();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);

		DrawText("My game", 100, 200,26, RED);
		DrawRectangle(posX, posY, size, size, BLUE);
		DrawRectangle(posX, posY, size, size, {210,59,207,127 });
		posX += 1;

		#pragma region imgui
		//first window

		ImGui::Begin("test");
		ImGui::Text("Hello");
		if(ImGui::Button("button"))
		{
			std::cout << "Button Pressed" << std::endl;
		};
		//ImGui::SameLine(); same line button
		if (ImGui::Button("button"))
		{
			std::cout << "Button2 Pressed" << std::endl;
		}

		ImGui::ShowDemoWindow();
		ImGui::End();
		//second window
		ImGui::Begin("second window");
		ImGui::Text("Slider");
		ImGui::NewLine();
		static float a = 0;
		ImGui::SliderFloat("speed", &a, 0, 1);

		ImGui::End();
		#pragma endregion


		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}