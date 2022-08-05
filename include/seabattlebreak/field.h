#ifndef SEABATTLEBREAK_FIELD_H
#define SEABATTLEBREAK_FIELD_H

#include <seabattlebreak/rules.h>

#include <iterator>
#include <vector>

namespace seabattlebreak {

  struct Coord {
    int x;
    int y;
  };
  inline bool operator==(const Coord& p1, const Coord& p2) {
    return (p1.x == p2.x && p1.y == p2.y);
  }
  inline Coord rotate(Coord p, int n) {
    for (int i{ 0 }; i < n; ++i) {
      const int p_y = p.y;
      p.y = -p.x;
      p.x = p_y;
    }
    return p;
  }

  class Field {
  public:
    enum class CellStates { kUnknown = 0, kEmpty, kShip, kWreck };
    class SpiralIterator {
    public:
      SpiralIterator(const Coord center, const Coord border);
      bool go_next();
      Coord pos() const { return pos_; }

    private:
      bool move_forward(Coord& cur) const;
      bool is_off_the_borders(Coord p) const;
      const Coord center_;
      const Coord border_;
      const int max_search_radius_;
      Coord pos_;
    };
    Field(int x_size, int y_size, bool is_hostile);
    bool is_purged() const { return (is_ready_ && good_shipcells == 0); }
    bool is_ready() const { return is_ready_; }
    void place_ship(Coord pos, Coord direction);
    bool shoot(Coord pos);
    void mark(Coord pos, bool is_wrecked);
    CellStates cell_state(int x, int y) const { return field_[x][y]; }
    CellStates cell_state(Coord p) const { return field_[p.x][p.y]; }
    bool worth_shooting(Coord p);

 private:
  bool try_place_ship(Coord pos, int length, Coord direction);
  bool is_vacant(Coord center);
  bool is_off_the_borders(Coord pos);
  auto& cell(Coord pos) { return field_[pos.x][pos.y]; }

  const int x_size_;
  const int y_size_;
  std::vector<std::vector<CellStates>> field_{};
  int ships_numbers_[std::size(seabattlebreak::rules::kSizedShipCounts)]{};
  int good_shipcells{0};
  bool is_ready_{false};
};

}  // namespace seabattlebreak

#endif  // !SEABATTLEBREAK_FIELD_H
