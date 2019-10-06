#include "tilemap.h"

#include <iostream>

#include "log.h"

const int kTileRows = 16;
const int kTileCols = 16;
const int kTileWidth = 16;
const int kTileHeight = 16;
const Tile kEmptyTile = 0;

SDL_Rect ToSDLRect(const Rect& rect) {
  SDL_Rect sr;
  sr.x = rect.x * kTileWidth;
  sr.y = rect.y * kTileHeight;
  sr.w = kTileWidth;
  sr.h = kTileHeight;
  return sr;
}

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

TileType TileToTileType(Tile tile) {
  switch(tile) {
    case 1:
      return TileType::GROUND;
    case 2:
      return TileType::SPIKES;
    default:
      return TileType::NONE;
  }
}

ObjectType TileToObjectType(Tile tile) {
  switch(tile) {
    case 3:
      return ObjectType::BOX;
    case 4:
      return ObjectType::START;
    case 5:
      return ObjectType::EXIT;
    default:
      return ObjectType::NONE;
  }
}

std::vector<TileMapObject> extractObjects(std::vector<std::vector<Tile>>* map) {
  SDL_assert(map != nullptr);

  std::vector<TileMapObject> objects;

  Rect location;
  location.w = 1;
  location.h = 1;

  for (int row = 0; row < map->size(); ++row) {
    std::vector<Tile>& tilerow = (*map)[row];
    for (int col = 0; col < tilerow.size(); ++col) {
      ObjectType type = TileToObjectType(tilerow[col]);
      if (type != ObjectType::NONE) {
        // Clear the tile so it shows up empty.
        tilerow[col] = kEmptyTile;

        location.y = row;
        location.x = col;

        TileMapObject object;
        object.type = type;
        object.location = location;
        objects.push_back(object);
      }
    }
  }

  return objects;
}

std::unique_ptr<TileMap> TileMap::Load(const TileSet* tileset) {
  auto map = std::make_unique<TileMap>();

  map->tileset = tileset;

  // Test to verify that empty tiles parse as NONE.
  SDL_assert(TileToTileType(kEmptyTile) == TileType::NONE);

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

  map->objects = extractObjects(&map->map);

  return map;
}

std::vector<TileMapObject> TileMap::TileMapObjects() const {
  return objects;
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
