#include <assert.h>
#include <seabattlebreak/field.h>
#include <seabattlebreak/rules.h>

#include <iterator>
#include <vector>

namespace seabattlebreak {

static Pos operator-(const Pos& p1, const Pos& p2) {
  return Pos{p1.x - p2.x, p1.y - p2.y};
}
static Pos operator+(const Pos& p1, const Pos& p2) {
  return Pos{p1.x + p2.x, p1.y + p2.y};
}
static Pos operator*(const int& m, const Pos& p) {
  return Pos{p.x * m, p.y * m};
}

static int max(const int& a, const int& b) { return a > b ? a : b; }
static int abs(const int& a) { return a > 0 ? a : -a; }

Field::SpiralIterator::SpiralIterator(const Pos center, const Pos border)
    : center_{center},
      border_{border},
      pos_{center},
      max_search_radius_{max(max(center.x, border.x - center.x - 1),
                             max(center.y, border.y - center.y - 1))} {}
bool Field::SpiralIterator::go_next() {
  Pos next{pos_};
  do {
    if (!move_forward(next)) {
      return false;
    }
  } while (is_off_the_borders(next));
  pos_ = next;
  return true;
}
bool Field::SpiralIterator::move_forward(Pos& cur_pos) const {
  Pos relative_pos = cur_pos - center_;
  const int radius{max(abs(relative_pos.x), abs(relative_pos.y))};
  if (relative_pos.x == radius && relative_pos.y > -radius) {
    --relative_pos.y;
  } else if (relative_pos.y == -radius && relative_pos.x > -radius) {
    --relative_pos.x;
  } else if (relative_pos.x == -radius && relative_pos.y < radius) {
    ++relative_pos.y;
  } else if (relative_pos.y == radius && relative_pos.x < radius) {
    ++relative_pos.x;
  }
  if (relative_pos.x == relative_pos.y && relative_pos.x >= 0) {
    ++relative_pos.x;
    ++relative_pos.y;
  }
  if (radius > max_search_radius_) {
    return false;
  }
  cur_pos = relative_pos + center_;
  return true;
}
bool Field::SpiralIterator::is_off_the_borders(Pos& p) const {
  return (p.x < 0 || p.x >= border_.x || p.y < 0 || p.y >= border_.y);
}

void Field::place_ship(Pos pos, Pos direction) {
  int i{0};
  while (ships_numbers_[i] == rules::kSizedShipCounts[i]) {
    ++i;
  }
  assert(i < std::size(ships_numbers_) && "Not trying to put more ships.");
  const int length = i + 1;
  SpiralIterator itr{pos, Pos{rules::kXSize, rules::kYSize}};
  while (!try_place_ship(itr.pos(), length, direction)) {
    if (!itr.go_next()) {
      assert(false &&
             "Should be able to place a ship until all cells are went over");
    }
  }
  ++(ships_numbers_[i]);
  good_shipcells += length; 

  if (i == std::size(ships_numbers_) - 1 && ships_numbers_[i] == rules::kSizedShipCounts[i]) {
    is_ready_ = true;
    for (auto& column : field_) {
      for (auto& cell : column) {
        if(cell == CellStates::kEmpty) cell = CellStates::kUnknown;
      }
    }
  }
}

bool Field::shoot(Pos pos) {
  const bool is_hit{(cell(pos) == CellStates::kShip)};
  cell(pos) = is_hit ? CellStates::kWreck : CellStates::kEmpty;
  good_shipcells -= is_hit;
  return is_hit;
}

void Field::mark(Pos pos, bool is_hit) {
  cell(pos) = is_hit ? CellStates::kWreck : CellStates::kEmpty;
}

bool Field::try_place_ship(Pos pos, int length, Pos direction) {
  assert(abs(direction.x) + abs(direction.y) == 1 &&
         "Direction vector length == 1");
  for (int i{0}; i < length; ++i) {
    if (!is_vacant(pos + i * direction)) {
      return false;
    }
  }
  for (int i{0}; i < length; ++i) {
    cell(pos + i * direction) = CellStates::kShip;
  }
  return true;
}
bool Field::is_vacant(Pos pos) {
  if (is_off_the_borders(pos)) {
    return false;
  }
  for (int x{-1}; x <= 1; ++x) {
    for (int y{-1}; y <= 1; ++y) {
      const Pos nearby = pos + Pos{x,y};
      if (is_off_the_borders(nearby)) {
        continue;
      } else if (cell(nearby) != CellStates::kEmpty) {
        return false;
      }
    }
  }
  return true;
}
bool Field::is_off_the_borders(Pos pos) {
  return (pos.x < 0 || pos.y < 0 || pos.x >= x_size_ || pos.y >= y_size_);
}
}  // namespace seabattlebreak