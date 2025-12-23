#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>

int main()
{
  const int WINDOW_WIDTH{1280};
  const int WINDOW_HEIGHT{720};

  SetTargetFPS(60);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terralib");

  // GRID LOGIC

  int posX{0};
  int posY{0};
  int tileSize{32};
  bool isGridActive{true};

  int gridCountX = WINDOW_WIDTH / tileSize;
  int gridCountY = WINDOW_HEIGHT / tileSize;

  // BLOCKS
  Texture2D tileSet = LoadTexture("assets/sprites/world_tileset.png");
  SetTextureFilter(tileSet, TEXTURE_FILTER_POINT);
  std::vector<Rectangle> placedBlocks;
  Rectangle grass{0.0f, 0.0f, 16.0f, 16.0f};
  Rectangle dirt{0.0f, 16.0f, 16.0f, 16.0f};

  // WORLD GEN
  int height = 5;

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
    if (IsKeyPressed(KEY_N))
    {
      isGridActive = !isGridActive;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    // DRAW GRIDS
    for (int y{0}; y <= gridCountY; y++)
    {

      for (int x{0}; x <= gridCountX; x++)
      {

        posX = x * tileSize;
        posY = y * tileSize;
        if (isGridActive)
        {
          DrawRectangleLines(posX, posY, tileSize, tileSize, WHITE);
        }
        if (gridCountY - y == height - 1)
        {
          DrawTexturePro(tileSet, grass, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
        else if (gridCountY - y < height - 1)
        {
          DrawTexturePro(tileSet, dirt, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
      }
    }

    for (auto &rec : placedBlocks)
    {
      DrawTexturePro(tileSet, dirt, rec, Vector2{0.f, 0.f}, 0.0f, WHITE);
    }
    EndDrawing();
  }
}