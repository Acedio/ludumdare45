#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <vector>

#include <SDL.h>

typedef int Tile;

class TileSet {
 public:
  TileSet(SDL_Texture* tex) : tex(tex) {}
  void DrawTile(SDL_Renderer* renderer, Tile tile,
                const SDL_Rect& dst) const;

 private:
  // Assumes 16x16 tilemap.
  SDL_Texture* tex;
};

class TileMap {
 public:
  // Draw the map, no viewport.
  void Draw(SDL_Renderer* renderer) const;

  static std::unique_ptr<TileMap> Load(SDL_Texture* tileset_texture);

 private:
  std::vector<std::vector<Tile>> map;
  std::unique_ptr<TileSet> tileset;
};

#endif
