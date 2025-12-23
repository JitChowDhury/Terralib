#include "raylib.h"
#include "raymath.h"

class Player
{
private:
  float speed{100.f};
  float height;
  float width;
  Vector2 position{};
  Vector2 velocity;
  Rectangle rec;

public:
  Player(float h, float w, Vector2 p);
  ~Player();
  void HandleInput();
  void Draw();
  Vector2 GetPosition() { return position; }
};
