#include "hero.h"

#include <iostream>

const double kHorizontalVel = 7.0;
const double kGravityAcc = 20.0;
const double kJumpVel = -15.0;

void Hero::Update(double t, ButtonState buttons, const TileMap& tilemap) {
  if (buttons.left == buttons.right) {
    vel.x = 0;
  } else if (buttons.left) {
    facing_right = false;
    vel.x = -kHorizontalVel;
  } else if (buttons.right) {
    facing_right = true;
    vel.x = kHorizontalVel;
  }

  if (buttons.jump && !falling) {
    falling = true;
    vel.y = kJumpVel;
  }

  double dx = t*vel.x;
  if (CollisionInfo info = tilemap.XCollide(bounding_box, dx);
      info.type == TileType::NONE) {
    bounding_box.x += dx;
  } else {
    bounding_box.x += dx + info.correction;
    vel.x = 0;
  }

  vel.y += t*kGravityAcc;
  double dy = t*vel.y;
  if (CollisionInfo info = tilemap.YCollide(bounding_box, dy);
      info.type == TileType::NONE) {
    falling = true;
    bounding_box.y += dy;
  } else {
    if (info.correction < 0) {
      // We've hit ground, so no longer falling.
      falling = false;
    }
    bounding_box.y += dy + info.correction;
    vel.y = 0;
  }
}

void Hero::Draw(SDL_Renderer* renderer) const {
  Rect sprite_box = bounding_box;
  // Offset the sprite because its bouding box is not a unit rectangle.
  sprite_box.x -= (1 - bounding_box.w)/2;
  sprite_box.y -= 1 - bounding_box.h;
  SDL_Rect dst = ToSDLRect(sprite_box);
  if (facing_right) {
    right.Draw(renderer, dst);
  } else {
    left.Draw(renderer, dst);
  }
}
