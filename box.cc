#include "box.h"

const double kGravityAcc = 5.0;
const double kSize = 1;
const double kBuffer = 0.001;

void BoxManager::Add(Vec pos) {
  int col = pos.x + 0.5;
  SDL_assert(col >= 0 && col < columns.size());
  columns[col].push_back({pos.y, 0, false});
  std::sort(columns[col].begin(), columns[col].end(),
            [](const Box& a, const Box& b) { return a.y > b.y; });
}

Rect ToBoundingBox(int col, const Box& box) {
  return Rect{float(col), box.y, kSize, kSize};
}

void BoxManager::Update(double t, const TileMap& tilemap) {
  for (int col = 0; col < columns.size(); ++col) {
    std::vector<Box>& column = columns[col];
    // Go from bottom to top.
    std::sort(column.begin(), column.end(),
              [](const Box& a, const Box& b) { return a.y > b.y; });
    for (int i = 0; i < column.size(); ++i) {
      Box& box = column[i];
      if (!box.stopped) {
        box.y_vel += t*kGravityAcc;
        // Boxes should only fall.
        SDL_assert(box.y_vel >= 0);
        double dy = t*box.y_vel;
        if (i > 0 && box.y + kSize >= column[i - 1].y) {
          // Collided with below box.
          box.stopped = true;
          box.y = column[i-1].y - kSize;
          box.y_vel = 0;
        } else if (CollisionInfo info =
                       tilemap.YCollide(ToBoundingBox(col, box), dy);
                   info.type != TileType::NONE) {
          // Collided with tilemap.
          box.stopped = true;
          box.y += dy + info.correction;
          box.y_vel = 0;
        } else {
          box.y += dy;
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

bool BoxManager::AtPoint(Vec p) const {
  int col = (int)p.x;
  if (col < 0 || col >= columns.size()) {
    return false;
  }
  // Should always be sorted, go from bottom to top.
  for (const Box& box : columns[col]) {
    if (p.y >= box.y + kSize) {
      // We're below the box, and boxes will only get higher.
      return false;
    }
    if (p.y >= box.y) {
      // Between the top and bottom of the box, collision.
      return true;
    }
  }
  return false;
}

std::optional<double> BoxManager::XCollide(const Rect& rect, double dx) const {
  double x1 = rect.x + dx;
  double y1 = rect.y;
  double x2 = rect.x + rect.w + dx;
  double y2 = rect.y + rect.h;

  if (dx <= 0) {
    // Velocity is negative, check left side.
    if (AtPoint({x1, y1})) {
      return ceil(x1) - x1;
    } else if (AtPoint({x1, y2 - kBuffer})) {
      return ceil(x1) - x1;
    }
    return std::nullopt;
  } else {
    // Velocity is positive, check right side.
    if (AtPoint({x2 - kBuffer, y1})) {
      return floor(x2) - x2;
    } else if (AtPoint({x2 - kBuffer, y2 - kBuffer})) {
      return floor(x2) - x2;
    }
    return std::nullopt;
  }
}

std::optional<double> BoxManager::YCollide(const Rect& rect, double dy) const {
  double x1 = rect.x;
  double y1 = rect.y + dy;
  double x2 = rect.x + rect.w;
  double y2 = rect.y + rect.h + dy;

  if (dy <= 0) {
    // Velocity is negative, check top side;
    if (AtPoint({x1, y1})) {
      return ceil(y1) - y1;
    } else if (AtPoint({x2 - kBuffer, y1})) {
      return ceil(y1) - y1;
    }
    return std::nullopt;
  } else {
    // Velocity is positive, check bottom side.
    if (AtPoint({x1, y2 - kBuffer})) {
      return floor(y2) - y2;
    } else if (AtPoint({x2 - kBuffer, y2 - kBuffer})) {
      return floor(y2) - y2;
    }
    return std::nullopt;
  }
}
