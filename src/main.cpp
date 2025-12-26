#include "raylib.h"
#include "raymath.h"

#include "Player/Player.h"

#include <iostream>
#include <vector>

int main()
{
  const int WINDOW_WIDTH{1280};
  const int WINDOW_HEIGHT{720};

  SetTargetFPS(60);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terralib");

  InitAudioDevice();

  // music
  Music music = LoadMusicStream("assets/music/01. Overworld Day.mp3");
  music.looping = true;
  SetMusicVolume(music, 0.03f);

  PlayMusicStream(music);

  // CAMERA
  Camera2D camera;
  camera.target = Vector2{0.f, 0.f};                                                  // this is what camera follows
  camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}; // camera draws target at offset
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // PLAYER
  Player player(64.f, 16.f, camera.target);
  // GRID LOGIC

  int posX{0};
  int posY{0};
  int tileSize{32};
  bool isGridActive{false};

  // BLOCKS
  Texture2D grassTileSet = LoadTexture("assets/Terraria_assets/grass.png");
  Texture2D dirtTileSet = LoadTexture("assets/Terraria_assets/Tiles_0.png");
  Texture2D stoneTileSet = LoadTexture("assets/Terraria_assets/Tiles_9.png");
  Texture2D background = LoadTexture("assets/Terraria_assets/SplashScreens/Splash_9_0.png");
  std::vector<Rectangle> placedBlocks;
  Rectangle grass{18.0f, 0.0f, 16.0f, 16.0f};
  Rectangle dirt{36.0f, 18.0f, 16.0f, 16.0f};
  Rectangle stone{18.0f, 18.0f, 16.0f, 16.0f};
  SetTextureFilter(grassTileSet, TEXTURE_FILTER_POINT);
  SetTextureFilter(dirtTileSet, TEXTURE_FILTER_POINT);
  SetTextureFilter(background, TEXTURE_FILTER_POINT);
  SetTextureFilter(stoneTileSet, TEXTURE_FILTER_POINT);

  // WORLD GEN
  int height = 5;
  int grassHeight = 7;

  //  MAIN GAME LOOP
  while (!WindowShouldClose())
  {
    std::cout << GetScreenWidth() << std::endl;
    UpdateMusicStream(music);
    player.HandleInput(tileSize, height);
    camera.target = player.GetPosition();

    // if (IsKeyDown(KEY_A))
    //   camera.target.x -= 5;
    // if (IsKeyDown(KEY_D))
    //   camera.target.x += 5;
    // if (IsKeyDown(KEY_W))
    //   camera.target.y -= 5;
    // if (IsKeyDown(KEY_S))
    //   camera.target.y += 5;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    int blockX = (int)(floor(mousePos.x / tileSize) * tileSize);
    int blockY = (int)(floor(mousePos.y / tileSize) * tileSize);

    // how many tile can fit on screen + 5 for safety check
    int tileOnScreenX = (GetScreenWidth() / tileSize) + 5;
    int tileOnScreenY = (GetScreenHeight() / tileSize) + 5;

    // start of drawing tile
    // cam.target.x/tilesize means in which grid the target is on then substract half of the tile number
    int startX = (int)(floor(camera.target.x / tileSize) - tileOnScreenX / 2);
    int startY = (int)(floor(camera.target.y / tileSize) - tileOnScreenY / 2);

    int endX = startX + tileOnScreenX;
    int endY = startY + tileOnScreenY;

    if (IsKeyPressed(KEY_F11))
    {
      ToggleBorderlessWindowed();
      camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}; // camera draws target at offset
    }

    if (IsWindowMaximized())
    {
      camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}; // camera draws target at offset
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
    DrawTexture(background, 0, 0, WHITE);
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
          DrawTexturePro(grassTileSet, grass, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
        else if (y > height && y <= grassHeight)
        {
          DrawTexturePro(dirtTileSet, dirt, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
        else if (y > grassHeight)
        {
          DrawTexturePro(stoneTileSet, stone, Rectangle{(float)posX, (float)posY, (float)tileSize, (float)tileSize}, Vector2{0.f, 0.f}, 0.0f, WHITE);
        }
      }
    }

    for (auto &rec : placedBlocks)
    {
      DrawTexturePro(grassTileSet, grass, rec, Vector2{0.f, 0.f}, 0.0f, WHITE);
    }
    player.Draw();
    EndMode2D();
    EndDrawing();
  }
  UnloadMusicStream(music);
  CloseAudioDevice();
  UnloadTexture(background);
  CloseWindow();
}