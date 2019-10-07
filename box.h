#ifndef BOX_H
#define BOX_H

#include <optional>

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

  std::optional<double> XCollide(const Rect& rect, double dx) const;
  std::optional<double> YCollide(const Rect& rect, double dy) const;

 private:
  bool AtPoint(Vec pos) const;

  // Each column should be sorted from bottom (largest Y) to top.
  std::vector<std::vector<Box>> columns;
  Sprite sprite;
};

#endif
