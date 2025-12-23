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

  // CAMERA
  Camera2D camera;
  camera.target = Vector2{0.f, 0.f}; // this is what camera follows
  camera.offset = Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
  // camera.offset = {0.f, 0.f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // GRID LOGIC

  int posX{0};
  int posY{0};
  int tileSize{32};
  bool isGridActive{true};

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
    if (IsKeyDown(KEY_A))
      camera.target.x -= 5;
    if (IsKeyDown(KEY_D))
      camera.target.x += 5;
    if (IsKeyDown(KEY_W))
      camera.target.y -= 5;
    if (IsKeyDown(KEY_S))
      camera.target.y += 5;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    int blockX = (int)(floor(mousePos.x / tileSize) * tileSize);
    int blockY = (int)(floor(mousePos.y / tileSize) * tileSize);

    int tileOnScreenX = (WINDOW_WIDTH / tileSize) + 2;  // 2 for safety check
    int tileOnScreenY = (WINDOW_HEIGHT / tileSize) + 2; // 2 for safety check

    int startX = (int)(floor(camera.target.x / tileSize) - tileOnScreenX / 2);
    int startY = (int)(floor(camera.target.y / tileSize) - tileOnScreenY / 2);

    int endX = startX + tileOnScreenX;
    int endY = startY + tileOnScreenY;

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

    BeginMode2D(camera);
    // DRAW GRIDS
    for (int y{startY}; y < endY; y++)
    {
      for (int x{startX}; x < endX; x++)
      {

        posX = x * tileSize;
        posY = y * tileSize;
        if (isGridActive)
        {
          DrawRectangleLines(posX, posY, tileSize, tileSize, WHITE);
        }
        if (y == height)
        {
          DrawTexturePro(tileSet, grass, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
        else if (y > height)
        {
          DrawTexturePro(tileSet, dirt, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
      }
    }

    for (auto &rec : placedBlocks)
    {
      DrawTexturePro(tileSet, dirt, rec, Vector2{0.f, 0.f}, 0.0f, WHITE);
    }
    EndMode2D();
    EndDrawing();
  }
}