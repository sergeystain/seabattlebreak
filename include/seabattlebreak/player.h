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
  bool is_winner() const;
  bool is_loser() const;
  bool check_shot(int x, int y);

  const Field& self_field() const { return *myfield_; }
  const Field& foe_field() const { return *foefield_; }

 private:
  Field* myfield_;
  Field* foefield_;
};

}  // namespace seabattlebreak

#endif  // !SEABATTLEBREAK_PLAYER_H
