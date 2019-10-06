#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <vector>

#include <SDL.h>

#include "geometry.h"

SDL_Rect ToSDLRect(const Rect& rect);

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

enum class TileType {
  NONE,
  GROUND,
  SPIKES
};

TileType TileToTileType(Tile tile);

enum class ObjectType {
  NONE,
  BOX,
  START,
  EXIT
};

struct TileMapObject {
  ObjectType type;
  // The coordinates and size of the object in tilespace.
  Rect location;
};

struct CollisionInfo {
  TileType tile;

  // A vector of directions the colliding rect could move to not collide with
  // the TileMap anymore. The caller should choose which direction to move,
  // either is fine. The lower magnitude should probably be preferred.
  Vec correction;
};

class TileMap {
 public:
  // Draw the map, no viewport.
  void Draw(SDL_Renderer* renderer) const;

  std::vector<TileMapObject> TileMapObjects() const;

  // TODO: Should this take a velocity vector as well to inform deltas?
  CollisionInfo Collide(const Rect& rect) const;

  static std::unique_ptr<TileMap> Load(const TileSet* tileset);

 private:
  std::vector<std::vector<Tile>> map;
  const TileSet* tileset;
  std::vector<TileMapObject> objects;
};

#endif
