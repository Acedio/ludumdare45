#include "hero.h"

#include "tilemap.h"

const double kHorizontalVel = 5.0;

void Hero::Update(double t, ButtonState buttons) {
  if (buttons.left == buttons.right) {
    vel.x = 0;
  } else if (buttons.left) {
    facing_right = false;
    vel.x = -kHorizontalVel;
  } else if (buttons.right) {
    facing_right = true;
    vel.x = kHorizontalVel;
  }

  bounding_box.x += t*vel.x;
}

void Hero::Draw(SDL_Renderer* renderer) const {
  SDL_Rect dst = ToSDLRect(bounding_box);
  if (facing_right) {
    right.Draw(renderer, dst);
  } else {
    left.Draw(renderer, dst);
  }
}
