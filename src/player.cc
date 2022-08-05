#include <assert.h>
#include <seabattlebreak/field.h>
#include <seabattlebreak/player.h>
#include <seabattlebreak/random.h>

namespace seabattlebreak {

void Player::input(int x, int y) {
  if (myfield_->is_ready()) {
    request_shot(Coord{x, y});
  } else {
    auto& rnd = Random::instance();
    const Coord random_direction{rotate(Coord{0, 1}, rnd.int0_3())};
    myfield_->place_ship(Coord{x, y}, random_direction);
  }
}

bool Player::is_loser() const { return myfield_->is_purged(); }

bool Player::check_shot(Coord p) { return myfield_->shoot(p); }
void Player::mark_shot(Coord p, bool is_hit) { foefield_->mark(p, is_hit); }

void Player::get_auto_shot(Coord& out_p) const{
  auto& rnd = Random::instance();
  const Coord random_start{rnd.int0_9(), rnd.int0_9()};
  Field::SpiralIterator itr(random_start, Coord{rules::kXSize, rules::kYSize});
  while (!foefield_->worth_shooting(itr.pos())) {
    if (!itr.go_next()) {
      assert(false && "Should win before trying all the cells");
    }
  }
  out_p = itr.pos();
}
bool Player::get_shot(Coord& out_p) {
  if (is_request_) {
    out_p = request_shot_pos_;
    is_request_ = false;
    return true;
  }
  return false;
}
void Player::auto_set_ships() {
  while (!myfield_->is_ready()) {
    auto& rnd = Random::instance();
    const Coord random_pos{rnd.int0_9(), rnd.int0_9()};
    const Coord random_direction{rotate(Coord{0, 1}, rnd.int0_3())};
    myfield_->place_ship(random_pos, random_direction);
  }
}
void Player::request_shot(Coord p) {
  assert(!is_request_ && "There no unsolved request.");
  is_request_ = true;
  request_shot_pos_ = p;
}

}  // namespace seabattlebreak
