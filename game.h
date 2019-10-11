#ifndef GAME_H
#define GAME_H

#include "box.h"
#include "buttons.h"
#include "hero.h"
#include "object.h"
#include "tilemap.h"

class Game {
 public:
  ~Game();

  void Update(double t, ButtonState buttons);
  void Draw(SDL_Renderer* renderer) const;

  static std::unique_ptr<Game> Load(SDL_Renderer* renderer);
 private:
  Game() {}

  void LoadLevel(int level, const TileSet* tileset);

  std::unique_ptr<TileSet> tileset;
  // Owned.
  SDL_Texture* tileset_texture;

  int level = 0;

  // These are reloaded with each level load.
  std::unique_ptr<TileMap> tilemap;
  std::unique_ptr<Hero> hero;
  std::unique_ptr<BoxManager> boxes;
  std::unique_ptr<ObjectManager> objects;
};

#endif
