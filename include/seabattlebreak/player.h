#ifndef SEABATTLEBREAK_PLAYER_H
#define SEABATTLEBREAK_PLAYER_H

#include <seabattlebreak/field.h>

namespace seabattlebreak {

class Player {
 public:
  Player() {
    myfield_ = new Field(rules::kXSize, rules::kYSize, false);
    foefield_ = new Field(rules::kXSize, rules::kYSize, true);
  }
  ~Player() {
    delete myfield_;
    delete foefield_;
  }
  void input(int x, int y);
  bool is_loser() const;
  bool check_shot(Coord p);
  void mark_shot(Coord p, bool is_hit);
  void get_auto_shot(Coord& out_p) const;
  bool get_shot(Coord& out_p);
  void auto_set_ships();
  const Field& self_field() const { return *myfield_; }
  const Field& foe_field() const { return *foefield_; }

 private:
  void request_shot(Coord p);
  bool is_request_{false};
  Coord request_shot_pos_{};
  Field* myfield_{};
  Field* foefield_{};
};

}  // namespace seabattlebreak

#endif  // !SEABATTLEBREAK_PLAYER_H
