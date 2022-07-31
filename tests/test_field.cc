#include <gtest/gtest.h>
#include <seabattlebreak/field.h>

namespace {
using namespace seabattlebreak;

TEST(SpiralIterator, NumberOfIterations) {
  Field::SpiralIterator i(Pos{9, 9}, Pos{10, 10});
  int count{0};
  while (i.go_next()) {
    ++count;
  }
  EXPECT_EQ(count, 99);
}
TEST(SpiralIterator, RightCircleSequence) {
  Field::SpiralIterator i(Pos{5, 5}, Pos{10, 10});
  const std::vector<Pos> seq{{6, 6}, {6, 5}, {6, 4}, {5, 4}, {4, 4},
                             {4, 5}, {4, 6}, {5, 6}, {7, 7}, {7, 6}};
  for (const Pos p : seq) { 
    i.go_next(); 
    EXPECT_EQ(p, i.pos());
  }
 }
TEST(SpiralIterator, RightSequenceInTheCorner) {
  Field::SpiralIterator i(Pos{9, 9}, Pos{10, 10});
  const std::vector<Pos> seq{{9, 8}, {8, 8}, {8, 9}, {9, 7},
                             {8, 7}, {7, 7}, {7, 8}, {7, 9}};
  for (const Pos p : seq) {
    i.go_next();
    EXPECT_EQ(p, i.pos());
  }
}

TEST(Field, FillWithShips) { 
  Field f(10, 10, false);
  while (! f.is_ready()) {
    f.place_ship(Pos{5, 5}, Pos{0, 1});
  }
}

}  // namespace
