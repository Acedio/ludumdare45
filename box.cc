#include "box.h"

const double kGravityAcc = 5.0;
const double kSize = 1;

void BoxManager::Add(Vec pos) {
  int col = pos.x + 0.5;
  SDL_assert(col >= 0 && col < columns.size());
  columns[col].push_back({pos.y, 0, false});
}

Rect ToBoundingBox(int col, const Box& box) {
  return Rect{float(col), box.y, kSize, kSize};
}

void BoxManager::Update(double t, const TileMap& tilemap) {
  for (int col = 0; col < columns.size(); ++col) {
    std::vector<Box>& column = columns[col];
    // Go from bottom to top.
    std::sort(column.begin(), column.end(),
              [](const Box& a, const Box& b) { return a.y < b.y; });
    for (int i = 0; i < column.size(); ++i) {
      Box& box = column[i];
      if (!box.stopped) {
        box.y_vel += t*kGravityAcc;
        // Boxes should only fall.
        SDL_assert(box.y_vel >= 0);
        double dy = t*box.y_vel;
        if (CollisionInfo info = tilemap.YCollide(ToBoundingBox(col, box), dy);
            info.type == TileType::NONE) {
          box.y += dy;
        } else {
          box.stopped = true;
          box.y += dy + info.correction;
          box.y_vel = 0;
        }
      }
    }
  }
}

void BoxManager::Draw(SDL_Renderer* renderer) const {
  for (int col = 0; col < columns.size(); ++col) {
    for (const Box& box : columns[col]) {
      SDL_Rect dst = ToSDLRect(ToBoundingBox(col, box));
      sprite.Draw(renderer, dst);
    }
  }
}
