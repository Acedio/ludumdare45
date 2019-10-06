#include "hero.h"

#include <iostream>

const double kHorizontalVel = 5.0;
const double kGravityAcc = 5.0;

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

  double dx = t*vel.x;
  CollisionInfo info = tilemap.XCollide(bounding_box, dx);
  if (info.type != TileType::GROUND) {
    bounding_box.x += dx;
  } else {
    bounding_box.x += dx + info.correction;
    vel.x = 0;
  }

  vel.y += t*kGravityAcc;
  double dy = t*vel.y;
  info = tilemap.YCollide(bounding_box, dy);
  if (info.type != TileType::GROUND) {
    bounding_box.y += dy;
  } else {
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
