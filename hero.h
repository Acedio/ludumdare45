#ifndef HERO_H
#define HERO_H

#include <SDL.h>

#include "box.h"
#include "buttons.h"
#include "geometry.h"
#include "sprite.h"
#include "tilemap.h"

enum class JumpState {
  RECOVERING,
  CAN_JUMP,
  FALLING,
};

class Hero {
 public:
  void Update(double t, ButtonState buttons, const TileMap& tilemap, const
              BoxManager& boxes);
  void Draw(SDL_Renderer* renderer) const;

  Hero(const TileSet* tileset, Vec pos)
      : bounding_box{pos.x, pos.y, 0.8, 0.9},
        vel{0, 0},
        left(tileset, 3),
        right(tileset, 4) {};

 private:
  Rect bounding_box;
  // Tiles/sec
  Vec vel;

  Sprite left;
  Sprite right;

  bool facing_right = true;
  JumpState jump_state = JumpState::RECOVERING;

  // ObjectType holding;
};

#endif
