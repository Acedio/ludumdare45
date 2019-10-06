#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <vector>

#include <SDL.h>

typedef int Tile;

struct TileSet {
  // Assumes 16x16 tiles.
  SDL_Texture* tex;
  int tile_w;
  int tile_h;
};

void drawTile(const TileSet& tileset, SDL_Renderer* renderer, Tile tile, int x, int y);

struct TileMap {
  std::vector<std::vector<Tile>> map;
  std::unique_ptr<TileSet> tileset;
};

std::unique_ptr<TileMap> load(SDL_Texture* tileset_texture);

// Draw the map, no viewport.
void drawMap(const TileMap& tilemap, SDL_Renderer* renderer);

#endif
