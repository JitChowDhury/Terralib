#include "raylib.h"
#include "raymath.h"

class Player
{
private:
  float speed{100.f};
  float jumpForce{400.f};
  float gravity{1200.f};
  float height;
  float width;
  Vector2 position{};
  Vector2 velocity{0, 0};
  Rectangle rec;

  bool isGrounded{true};

public:
  Player(float h, float w, Vector2 p);
  ~Player();
  void HandleInput(int tileSize, int groundTileY);
  ;
  void Draw();
  Vector2 GetPosition() { return position; }
};
