#include "raylib.h"
#include "raymath.h"
int main()
{
  const int WINDOW_WIDTH{900};
  const int WINDOW_HEIGHT{506};

  SetTargetFPS(60);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terralib");

  int posX{0};
  int posY{0};
  int tileSize{32};

  int gridCountX = WINDOW_WIDTH / tileSize;
  int gridCountY = WINDOW_HEIGHT / tileSize;

  while (!WindowShouldClose())
  {
    Vector2 mousePos = GetMousePosition();

    int blockX = (int)(floor(mousePos.x / tileSize) * tileSize);
    int blockY = (int)(floor(mousePos.y / tileSize) * tileSize);

    if (IsKeyPressed(KEY_F11))
    {
      ToggleBorderlessWindowed();
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
        // DrawRectangleLinesEx(Rectangle{static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(tileSize - 2), static_cast<float>(tileSize - 2)}, 2.f, WHITE);
        posX += tileSize;
      }
      posY += tileSize;
    }

    DrawRectangle(blockX, blockY, tileSize, tileSize, WHITE);
    EndDrawing();
  }
}