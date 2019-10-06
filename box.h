#ifndef BOX_H
#define BOX_H

#include "geometry.h"
#include "tilemap.h"
#include "sprite.h"

struct Box {
  // Boxes only move up and down.
  double y;
  double y_vel;
  bool stopped;
};

class BoxManager {
 public:
  BoxManager(TileSet* tileset, int cols) : columns(cols), sprite(tileset, 3) {}

  // Round x to make sure that we don't skip into the wrong col.
  void Add(Vec pos);
  void Update(double t, const TileMap& tilemap);
  void Draw(SDL_Renderer* renderer) const;
 private:
  std::vector<std::vector<Box>> columns;
  Sprite sprite;
};

#endif
