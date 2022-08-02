#include <assert.h>
#include <seabattlebreak/random.h>

#include <random>
#include <string>

namespace seabattlebreak {

typedef std::mt19937 GeneratorType;

Random::Random() {
  std::random_device rd;
  gen_ptr_ = new GeneratorType(rd());
}

void Random::reseed(const char* const seed_cstr) {
  assert(seed_cstr && gen_ptr_ &&
         "Seed is provided and generator is already exist.");
  std::string str(seed_cstr);
  std::seed_seq seed_seq(begin(str), end(str));
  delete gen_ptr_;
  gen_ptr_ = new GeneratorType(seed_seq);
}
int Random::int0_9() {
  static std::uniform_int_distribution<int> dist(0, 9);
  return dist(*static_cast<GeneratorType*>(gen_ptr_));
}

}  // namespace seabattlebreak