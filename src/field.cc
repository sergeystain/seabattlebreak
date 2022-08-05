#include <assert.h>
#include <seabattlebreak/field.h>
#include <seabattlebreak/rules.h>

#include <iterator>
#include <vector>

namespace seabattlebreak {

static Coord operator-(const Coord& p1, const Coord& p2) {
  return Coord{p1.x - p2.x, p1.y - p2.y};
}
static Coord operator+(const Coord& p1, const Coord& p2) {
  return Coord{p1.x + p2.x, p1.y + p2.y};
}
static Coord operator*(const Coord& p, const int& m) {
  return Coord{p.x * m, p.y * m};
}
static Coord operator*(const int& m, const Coord& p) {
  return Coord{p.x * m, p.y * m};
}

static int max(const int& a, const int& b) { return a > b ? a : b; }
static int abs(const int& a) { return a > 0 ? a : -a; }

/// <summary>
/// ////////
/// </summary>
/// <param name="center"></param>
/// <param name="border"></param>
Field::SpiralIterator::SpiralIterator(const Coord center, const Coord border)
    : center_{center},
      border_{border},
      pos_{center},
      max_search_radius_{max(max(center.x, border.x - center.x - 1),
                             max(center.y, border.y - center.y - 1))} {}
bool Field::SpiralIterator::go_next() {
  Coord next{pos_};
  do {
    if (!move_forward(next)) {
      return false;
    }
  } while (is_off_the_borders(next));
  pos_ = next;
  return true;
}
bool Field::SpiralIterator::move_forward(Coord& cur_pos) const {
  Coord relative_pos = cur_pos - center_;
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
/// <summary>
/// //////////////////////////////////////////////
/// </summary>
/// <param name="x_size"></param>
/// <param name="y_size"></param>
/// <param name="is_hostile"></param>
Field::Field(int x_size, int y_size, bool is_hostile)
    : x_size_{x_size}, y_size_{y_size} {
  field_.resize(rules::kXSize);
  for (auto& column : field_) {
    column.resize(rules::kYSize);
    for (auto& cell : column) {
      cell = is_hostile ? CellStates::kUnknown : CellStates::kEmpty;
    }
  }
}

bool Field::SpiralIterator::is_off_the_borders(Coord p) const {
  return (p.x < 0 || p.x >= border_.x || p.y < 0 || p.y >= border_.y);
}

void Field::place_ship(Coord pos, Coord direction) {
  int i{0};
  while (ships_numbers_[i] == rules::kSizedShipCounts[i]) {
    ++i;
  }
  assert(i < std::size(ships_numbers_) && "Not trying to put more ships.");
  const int length = i + 1;
  SpiralIterator itr{pos, Coord{rules::kXSize, rules::kYSize}};
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

bool Field::shoot(Coord pos) {
  const bool is_hit{(cell(pos) == CellStates::kShip)};
  cell(pos) = is_hit ? CellStates::kWreck : CellStates::kEmpty;
  good_shipcells -= is_hit;
  return is_hit;
}

void Field::mark(Coord pos, bool is_hit) {
  cell(pos) = is_hit ? CellStates::kWreck : CellStates::kEmpty;
}

bool Field::worth_shooting(Coord p) {
  if (is_off_the_borders(p)) {
    return false;
  }
  if (cell_state(p) == CellStates::kEmpty || cell_state(p) == CellStates::kWreck) {
    return false;
  }
  const Coord diag_direction{1, 1};
  for (int i{0}; i < 4; i++) {
    const Coord diag_sibling{p + rotate(diag_direction, i)};
    if (is_off_the_borders(diag_sibling)) {
      continue;
    }
    const auto sibling = cell_state(diag_sibling);
    if (sibling == CellStates::kWreck) {
      return false;
    }
  }
  return true;
}

bool Field::try_place_ship(Coord pos, int length, Coord direction) {
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
bool Field::is_vacant(Coord pos) {
  if (is_off_the_borders(pos)) {
    return false;
  }
  for (int x{-1}; x <= 1; ++x) {
    for (int y{-1}; y <= 1; ++y) {
      const Coord nearby = pos + Coord{x,y};
      if (is_off_the_borders(nearby)) {
        continue;
      } else if (cell_state(nearby) != CellStates::kEmpty) {
        return false;
      }
    }
  }
  return true;
}
bool Field::is_off_the_borders(Coord pos) {
  return (pos.x < 0 || pos.y < 0 || pos.x >= x_size_ || pos.y >= y_size_);
}
}  // namespace seabattlebreak