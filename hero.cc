#include "hero.h"

#include <iostream>

const double kHorizontalVel = 7.0;
const double kGravityAcc = 20.0;
const double kJumpVel = -15.0;
const double kGrabReach = 0.3;

const char* ToString(JumpState j) {
  switch(j) {
    case JumpState::CAN_JUMP: return "CAN_JUMP";
    case JumpState::RECOVERING: return "RECOVERING";
    case JumpState::FALLING: return "FALLING";
    default: return "UNKNOWN JUMP STATE";
  }
}

Vec Hero::HeldUpperLeft() const {
  // Y is weird because we need to remove the sprite offset to get the box to
  // match up, height-wise. See the Draw() function.
  double y = bounding_box.y - (1 - bounding_box.h);
  if (facing_right) {
    return Vec{bounding_box.x + bounding_box.w, y};
  } else {
    return Vec{bounding_box.x - 1, y};
  }
}

void Hero::UpdateGrab(ButtonState buttons, const TileMap& tilemap,
                      BoxManager* boxes) {
  SDL_assert(boxes);

  if (grab_state == GrabState::RECOVERING && !buttons.grab) {
    grab_state = GrabState::CAN_GRAB;
  } else if (grab_state == GrabState::CAN_GRAB && buttons.grab) {
    std::cout << "Attempting to grab." << std::endl;
    // try to grab a thing.
    if (jump_state == JumpState::FALLING) {
      // Can't grab while jumping.
      return;
    }
    holding = boxes->GrabAt(
        Vec{facing_right ? bounding_box.x + bounding_box.w + kGrabReach
                         : bounding_box.x - kGrabReach,
            bounding_box.y + bounding_box.h / 2.0});
    if (holding == BoxType::NONE) {
      grab_state = GrabState::RECOVERING;
    } else {
      grab_state = GrabState::GRABBING;
    }
  } else if (grab_state == GrabState::GRABBING && !buttons.grab) {
    grab_state = GrabState::CAN_RELEASE;
  } else if (grab_state == GrabState::CAN_RELEASE && buttons.grab) {
    if (jump_state == JumpState::FALLING) {
      // Must release grab button before trying to release again.
      grab_state = GrabState::GRABBING;
      return;
    }
    if (boxes->TryAdd(HeldUpperLeft(), holding)) {
      holding = BoxType::NONE;
      grab_state = GrabState::RECOVERING;
    }
  }
}

void Hero::Update(double t, ButtonState buttons, const TileMap& tilemap,
                  BoxManager* boxes) {
  SDL_assert(boxes);

  // MOVEMENT
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
  if (auto maybe_correction = boxes->XCollide(bounding_box, dx);
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
  auto y_correction = boxes->YCollide(bounding_box, dy);
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

  UpdateGrab(buttons, tilemap, boxes);
}

void Hero::Draw(SDL_Renderer* renderer) const {
  // The sprite is a unit square that should be bottom aligned and horizontally
  // centered around the bounding_box.
  Rect sprite_box{bounding_box.x - (1 - bounding_box.w) / 2,
                  bounding_box.y - (1 - bounding_box.h), 1, 1};
  SDL_Rect dst = ToSDLRect(sprite_box);
  if (facing_right) {
    right.Draw(renderer, dst);
  } else {
    left.Draw(renderer, dst);
  }
  if (holding != BoxType::NONE) {
    Vec upper_left = HeldUpperLeft();
    Rect box_sprite_box{upper_left.x, upper_left.y, 1, 1};
    left.Draw(renderer, ToSDLRect(box_sprite_box));
  }
}
