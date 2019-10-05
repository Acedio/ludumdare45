#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <vector>

typedef int Tile;

struct TileMap {
  std::vector<std::vector<Tile> > map;
};

std::unique_ptr<TileMap> load();

#endif
