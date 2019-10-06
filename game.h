#ifndef GAME_H
#define GAME_H

#include "tilemap.h"
#include "buttons.h"
#include "hero.h"

class Game {
 public:
  ~Game();

  void Update(double t, ButtonState buttons);
  void Draw(SDL_Renderer* renderer) const;

  static std::unique_ptr<Game> Load(SDL_Renderer* renderer);
 private:
  Game() {}

  std::unique_ptr<TileSet> tileset;
  std::unique_ptr<TileMap> tilemap;
  std::unique_ptr<Hero> hero;
  // Owned.
  SDL_Texture* tileset_texture;
};

#endif
