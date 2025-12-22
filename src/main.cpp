#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>

int main()
{
  const int WINDOW_WIDTH{900};
  const int WINDOW_HEIGHT{506};

  SetTargetFPS(60);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terralib");

  // GRID LOGIC

  int posX{0};
  int posY{0};
  int tileSize{32};

  int gridCountX = WINDOW_WIDTH / tileSize;
  int gridCountY = WINDOW_HEIGHT / tileSize;

  // BLOCKS
  Texture2D tileSet = LoadTexture("assets/sprites/world_tileset.png");
  SetTextureFilter(tileSet, TEXTURE_FILTER_POINT);
  std::vector<Rectangle> placedBlocks;
  Rectangle source{0.0f, 0.0f, 16.0f, 16.0f};

  while (!WindowShouldClose())
  {
    Vector2 mousePos = GetMousePosition();

    int blockX = (int)(floor(mousePos.x / tileSize) * tileSize);
    int blockY = (int)(floor(mousePos.y / tileSize) * tileSize);

    if (IsKeyPressed(KEY_F11))
    {
      ToggleBorderlessWindowed();
    }

    if (IsMouseButtonPressed(0))
    {
      placedBlocks.emplace_back(Rectangle{(float)blockX, (float)blockY, (float)tileSize, (float)tileSize});
    }

    BeginDrawing();
    ClearBackground(BLACK);
    posY = 0.f;
    for (int y{0}; y <= gridCountY; y++)
    {
      posX = 0;
      for (int x{0}; x <= gridCountX; x++)
      {

        DrawRectangleLines(posX, posY, tileSize, tileSize, WHITE);

        posX += tileSize;
      }
      posY += tileSize;
    }

    for (auto &rec : placedBlocks)
    {
      DrawTexturePro(tileSet, source, rec, Vector2{0.f, 0.f}, 0.0f, WHITE);
    }
    EndDrawing();
  }
}