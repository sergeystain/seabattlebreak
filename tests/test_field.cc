#include <gtest/gtest.h>
#include <seabattlebreak/field.h>
#include <seabattlebreak/random.h>

namespace {
using namespace seabattlebreak;

TEST(SpiralIterator, NumberOfIterations) {
  Field::SpiralIterator i(Coord{9, 9}, Coord{10, 10});
  int count{0};
  while (i.go_next()) {
    ++count;
  }
  EXPECT_EQ(count, 99);
}
TEST(SpiralIterator, RightCircleSequence) {
  Field::SpiralIterator i(Coord{5, 5}, Coord{10, 10});
  const std::vector<Coord> seq{{6, 6}, {6, 5}, {6, 4}, {5, 4}, {4, 4},
                               {4, 5}, {4, 6}, {5, 6}, {7, 7}, {7, 6}};
  for (const Coord p : seq) {
    i.go_next();
    EXPECT_EQ(p, i.pos());
  }
}
TEST(SpiralIterator, RightSequenceInTheCorner) {
  Field::SpiralIterator i(Coord{9, 9}, Coord{10, 10});
  const std::vector<Coord> seq{{9, 8}, {8, 8}, {8, 9}, {9, 7},
                               {8, 7}, {7, 7}, {7, 8}, {7, 9}};
  for (const Coord p : seq) {
    i.go_next();
    EXPECT_EQ(p, i.pos());
  }
}

TEST(Field, FillWithShips) {
  Field f(10, 10, false);
  while (!f.is_ready()) {
    f.place_ship(Coord{5, 5}, Coord{0, 1});
  }
}

TEST(Random, DeterminedBySeedAndReseedable) {
  Random& rnd{Random::instance()};
  rnd.reseed("abc_seed");
  const std::vector<int> seq{0, 2, 4, 1, 1, 0, 3, 7, 6, 8, 4};
  for (const int& n : seq) {
    const int generated{rnd.int0_9()};
    EXPECT_EQ(n, generated);
  }
  rnd.reseed("abc_seed2");
  const std::vector<int> seq2{1, 8, 3, 9, 0, 5, 4, 6, 9, 4, 6};
  for (const int& n : seq2) {
    const int generated{rnd.int0_9()};
    EXPECT_EQ(n, generated);
  }
}
TEST(Random, SeededSingletoneDeterminedButNotRepeting) {
  Random& rnd{Random::instance()};
  rnd.reseed("abc_seed");
  {
    Random& rnd1{Random::instance()};
    const std::vector<int> seq{0, 2, 4, 1, 1, 0, 3, 7, 6, 8, 4};
    for (const int& n : seq) {
      const int generated{rnd1.int0_9()};
      EXPECT_EQ(n, generated);
    }
  }
  {
    // singletone shoud give the same instance
    Random& rnd2{Random::instance()};
    // and the seeded sequence continues
    const std::vector<int> seq2{3, 7, 1, 3, 6, 0, 8, 1, 1, 0, 9};
    for (const int& n : seq2) {
      const int generated{rnd2.int0_9()};
      EXPECT_EQ(n, generated);
    }
  }
}

}  // namespace
