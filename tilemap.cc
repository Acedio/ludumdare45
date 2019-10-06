#include "tilemap.h"

#include <iostream>

#include "log.h"

const int kTileRows = 16;
const int kTileCols = 16;
const int kTileWidth = 16;
const int kTileHeight = 16;

void TileSet::DrawTile(SDL_Renderer* renderer, Tile tile,
                       const SDL_Rect& dst) const {
  SDL_assert(tile >= 0 && tile < kTileRows*kTileCols);

  int tile_x = tile % kTileCols;
  int tile_y = tile / kTileCols;

  SDL_Rect src;
  src.x = tile_x * kTileWidth;
  src.y = tile_y * kTileHeight;
  src.w = kTileWidth;
  src.h = kTileHeight;

  SDL_RenderCopy(renderer, tex, &src, &dst);
}

std::unique_ptr<TileMap> TileMap::Load(SDL_Texture *tileset_texture) {
  auto map = std::make_unique<TileMap>();

  map->tileset = std::make_unique<TileSet>(tileset_texture);

  // TODO: Add validation.
  map->map = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 5, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };

  return map;
}

void TileMap::Draw(SDL_Renderer* renderer) const {
  SDL_Rect dst;
  dst.w = kTileWidth;
  dst.h = kTileHeight;

  for (int row = 0; row < map.size(); ++row) {
    const std::vector<Tile>& tilerow = map[row];
    for (int col = 0; col < tilerow.size(); ++col) {
      dst.y = row * kTileHeight;
      dst.x = col * kTileWidth;
      tileset->DrawTile(renderer, tilerow[col], dst);
    }
  }
}
