#include "hero.h"

#include <iostream>

const double kHorizontalVel = 7.0;
const double kGravityAcc = 20.0;
const double kJumpVel = -15.0;

void Hero::Update(double t, ButtonState buttons, const TileMap& tilemap,
                  const BoxManager& boxes) {
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
  if (auto maybe_correction = boxes.XCollide(bounding_box, dx);
      maybe_correction) {
    // TODO: What if a box collision response yields a tilemap collision?
    // Currently giving tilemap priority.
    dx += maybe_correction.value();
    vel.x = 0;
  }

  if (CollisionInfo info = tilemap.XCollide(bounding_box, dx);
      info.type == TileType::NONE) {
    bounding_box.x += dx;
  } else {
    // Tilemap collision.
    bounding_box.x += dx + info.correction;
    vel.x = 0;
  }

  vel.y += t*kGravityAcc;
  double dy = t*vel.y;
  auto y_correction = boxes.YCollide(bounding_box, dy);
  if (y_correction) {
    // Hit a box, no longer falling.
    falling = false;
    vel.y = 0;
    dy += y_correction.value();
  }

  if (CollisionInfo info = tilemap.YCollide(bounding_box, dy);
      info.type == TileType::NONE) {
    if (!y_correction) {
      // No boxes hit, no ground hit.
      falling = true;
    }
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
