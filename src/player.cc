#include <random>

#include <seabattlebreak/field.h>
#include <seabattlebreak/player.h>

namespace seabattlebreak {

void Player::input(int x, int y) {
  if (myfield_->is_ready()) {
    // bool hit{foe.shoot(x,y)};
    foefield_->shoot(Pos{x, y});
  } else {
    myfield_->place_ship(Pos{x, y}, Pos{1, 0}); 
  }
}

bool Player::is_winner() const { return 0* foefield_->is_purged(); }
bool Player::is_loser() const { return 0* myfield_->is_purged(); }

bool Player::check_shot(int x, int y) { return myfield_->shoot(Pos{x,y}); }

}  // namespace seabattlebreak
