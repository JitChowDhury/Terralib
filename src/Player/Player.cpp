#include "player.h"

Player::Player(float h, float w, Vector2 p) : width(w), height(h)
{
  position = {p.x - width / 2.f, p.y - height / 2.f};
  rec = {position.x, position.y, width, height};
}

void Player::HandleInput(int tileSize, int groundTileY)
{
  velocity.x = 0;
  float dt = GetFrameTime();

  if (IsKeyDown(KEY_A))
  {
    velocity.x = -speed;
  }
  if (IsKeyDown(KEY_D))
  {
    velocity.x = speed;
  }

  if (IsKeyPressed(KEY_SPACE) && isGrounded)
  {
    velocity.y = -jumpForce;
    isGrounded = false;
  }
  if (isGrounded)
    velocity.y = 0;
  else
  {
    velocity.y += gravity * dt;
  }

  position.x += velocity.x * dt;
  position.y += velocity.y * dt;

  float playerBottom = position.y + height;
  int tileY = (int)floor(playerBottom / tileSize);

  if (tileY >= groundTileY)
  {
    position.y = groundTileY * tileSize - height;
    velocity.y = 0;
    isGrounded = true;
  }
  else
  {
    isGrounded = false;
  }

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