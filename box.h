#ifndef BOX_H
#define BOX_H

#include "geometry.h"
#include "tilemap.h"

class Box {
 public:
  Box(Vec pos) : pos{pos.x, pos.y, 1, 1}, vel{0, 0} {}
  void Update(double t, const TileMap& tilemap);
  void Draw(SDL_Renderer* renderer) const;
 private:
  Rect pos;
  Vec vel;
};

#endif
