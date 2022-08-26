#include <assert.h>
#include <seabattlebreak/player.h>
#include <seabattlebreak/rules.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

using seabattlebreak::Field;
using seabattlebreak::rules::kXSize;
using seabattlebreak::rules::kYSize;

static char state_to_char(Field::CellState state) {
  switch (state) {
    case Field::CellState::kUnknown:
      return 0xfa;
    case Field::CellState::kEmpty:
      return '*';
    case Field::CellState::kShip:
      return 0xdb;
    case Field::CellState::kWreck:
      return 0xb0;
    default:
      assert(false && "Don't try to print unknown state");
      return ' ';
  }
}

static void print_fields(seabattlebreak::Player& p) {
  // Form fields header and border strings
  static_assert(kXSize < ('z' - 'a' + 1) && "Field width fits alphabet");
  constexpr std::string_view kHeaderStr{"abcdefghijklmnopqrstuvwxyz", kXSize};
  // Form fields rows strings
  std::string self_rows[kYSize]{};
  std::string foe_rows[kYSize]{};
  for (int y{0}; y < kYSize; ++y) {
    for (int x{0}; x < kXSize; ++x) {
      self_rows[y] += state_to_char(p.self_field().cell_state(x, y));
      foe_rows[y] += state_to_char(p.foe_field().cell_state(x, y));
    }
  }
  // Print fields
  std::cout << '\n';
  std::cout << "   " << ' ' << kHeaderStr << "   "
            << "   " << ' ' << kHeaderStr << " \n";
  for (int y{kYSize - 1}; y >= 0; --y) {
    std::cout << std::setw(3) << y << "|" << self_rows[y] << "|  "
              << std::setw(3) << y << '|' << foe_rows[y] << "|\n";
  }
  std::cout << "   " << ' ' << kHeaderStr << "   "
            << "   " << ' ' << kHeaderStr << " \n";
}

int main(int argc, char** argv) {
  seabattlebreak::Player user{};
  seabattlebreak::Player comp{};

  comp.auto_set_ships();

  while (true) {
    print_fields(user);

    if (user.is_loser() && comp.is_loser()) {
      std::cout << "Standoff?!! Is it really?";
      break;
    }
    if (comp.is_loser()) {
      std::cout << "You won. Congrats!!!";
      break;
    }
    if (user.is_loser()) {
      std::cout << "You lost! Better luck next time.";
      break;
    }

    char x_inp{};
    int y_inp{};
    std::cin >> x_inp;
    std::cin >> y_inp;
    const int x{std::tolower(x_inp) - 'a'};
    const int y{y_inp};
    if (x > kXSize || y > kYSize || x < 0 || y < 0) {
      std::cout << "Outside the lines!\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    user.input(x, y);

    seabattlebreak::Coord user_shot{}, comp_shot{};
    if (user.get_shot(user_shot)) {
      const bool user_made_hit{comp.check_shot(user_shot)};
      user.mark_shot(user_shot, user_made_hit);

      comp.get_auto_shot(comp_shot);
      const bool comp_made_hit{user.check_shot(comp_shot)};
      comp.mark_shot(comp_shot, comp_made_hit);
    }
  }
  return 0;
}
