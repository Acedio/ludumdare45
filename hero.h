#ifndef HERO_H
#define HERO_H

#include <SDL.h>

#include "buttons.h"
#include "geometry.h"
#include "sprite.h"
#include "tilemap.h"

class Hero {
 public:
  // Should this take a const TileMap&?
  void Update(double t, ButtonState buttons);
  void Draw(SDL_Renderer* renderer) const;

  Hero(const TileSet* tileset, Vec pos)
      : bounding_box{pos.x, pos.y, 1, 1},
        vel{0, 0},
        left(tileset, 3),
        right(tileset, 4),
        facing_right(true),
        holding(ObjectType::NONE) {}

 private:
  Rect bounding_box;
  // Tiles/sec
  Vec vel;

  Sprite left;
  Sprite right;

  bool facing_right;

  ObjectType holding;
};

#endif
