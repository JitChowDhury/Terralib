#include "player.h"

Player::Player(float h, float w, Vector2 p) : width(w), height(h)
{
  position = {p.x - width / 2.f, p.y - height / 2.f};
  rec = {position.x, position.y, width, height};
}

void Player::HandleInput()
{
  velocity = {0, 0};
  if (IsKeyDown(KEY_W))
  {
    velocity.y -= speed * GetFrameTime();
  }
  if (IsKeyDown(KEY_S))
  {
    velocity.y += speed * GetFrameTime();
  }
  if (IsKeyDown(KEY_A))
  {
    velocity.x -= speed * GetFrameTime();
  }
  if (IsKeyDown(KEY_D))
  {
    velocity.x += speed * GetFrameTime();
  }

  position = Vector2Add(position, velocity);
  rec.x = position.x;
  rec.y = position.y;
}

void Player::Draw()
{
  DrawRectangleRec(rec, RED);
}

Player::~Player()
{
}