#include "hero.h"

#include <iostream>

const double kHorizontalVel = 7.0;
const double kGravityAcc = 20.0;
const double kJumpVel = -15.0;

const char* ToString(JumpState j) {
  switch(j) {
    case JumpState::CAN_JUMP: return "CAN_JUMP";
    case JumpState::RECOVERING: return "RECOVERING";
    case JumpState::FALLING: return "FALLING";
    default: return "UNKNOWN JUMP STATE";
  }
}

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

  if (buttons.jump && jump_state == JumpState::CAN_JUMP) {
    jump_state = JumpState::FALLING;
    vel.y = kJumpVel;
  } else if (!buttons.jump && jump_state == JumpState::RECOVERING) {
    // You must release jump before jumping again.
    jump_state = JumpState::CAN_JUMP;
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
    if (jump_state == JumpState::FALLING) {
      // Hit a box. If we were previously falling (i.e. not already on the box),
      // we are no longer falling.
      jump_state = JumpState::RECOVERING;
    }
    vel.y = 0;
    dy += y_correction.value();
  }

  if (CollisionInfo info = tilemap.YCollide(bounding_box, dy);
      info.type == TileType::NONE) {
    if (!y_correction) {
      // No boxes hit, no ground hit.
      jump_state = JumpState::FALLING;
    }
    bounding_box.y += dy;
  } else {
    if (info.correction <= 0 && jump_state == JumpState::FALLING) {
      // We've been pushed up (hit ground). If we were previously falling (i.e.
      // not already on the ground), we are no longer falling.
      jump_state = JumpState::RECOVERING;
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
