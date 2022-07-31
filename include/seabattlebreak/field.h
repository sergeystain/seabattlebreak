#ifndef SEABATTLEBREAK_FIELD_H
#define SEABATTLEBREAK_FIELD_H

#include <seabattlebreak/rules.h>

#include <iterator>
#include <vector>

namespace seabattlebreak {

struct Pos {
  int x;
  int y;
};
static bool operator==(const Pos& p1, const Pos& p2) {
  return (p1.x == p2.x && p1.y == p2.y);
}

class Field {
 public:
  enum class CellStates { kUnknown = 0, kEmpty, kShip, kWreck };
  class SpiralIterator {
   public:
    SpiralIterator(const Pos center, const Pos border);
    bool go_next();
    Pos pos() const { return pos_; }

   private:
    bool move_forward(Pos& cur) const;
    bool is_off_the_borders(Pos& p) const;
    const Pos center_;
    const Pos border_;
    const int max_search_radius_;
    Pos pos_;
  };
  Field(int x_size, int y_size, bool is_hostile)
      : x_size_{x_size}, y_size_{y_size} {
    field_.resize(rules::kXSize);
    for (auto& column : field_) {
      column.resize(rules::kYSize);
      for (auto& cell : column) {
        cell = is_hostile ? CellStates::kUnknown : CellStates::kEmpty;
      }
    }
  }
  bool is_purged() const { return (good_shipcells == 0); }
  bool is_ready() const { return is_ready_; }
  void place_ship(Pos pos, Pos direction);
  bool shoot(Pos pos);
  void mark(Pos pos, bool is_wrecked);
  CellStates cell_state(int x, int y) const { return field_[x][y]; }

 private:
  bool try_place_ship(Pos pos, int length, Pos direction);
  bool is_vacant(Pos center);
  bool is_off_the_borders(Pos pos);
  auto& cell(Pos pos) { return field_[pos.x][pos.y]; }
  const int x_size_;
  const int y_size_;
  std::vector<std::vector<CellStates>> field_{};
  int ships_numbers_[std::size(seabattlebreak::rules::kSizedShipCounts)]{};
  int good_shipcells{0};
  bool is_ready_{false};
};

}  // namespace seabattlebreak

#endif  // !SEABATTLEBREAK_FIELD_H
