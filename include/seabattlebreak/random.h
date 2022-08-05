#ifndef SEABATTLEBREAK_RANDOM_H
#define SEABATTLEBREAK_RANDOM_H

namespace seabattlebreak {

class Random {  // Singleton class (private constructor/destructor)
 public:
  static Random& instance() {
    static Random the_single_instance{};
    return the_single_instance;
  }
  void reseed(const char* const seed_cstr);
  int int0_9();
  int int0_3();
 private:
  Random();
  Random(const Random&) = delete;
  Random& operator=(const Random&) = delete;
  ~Random() { delete gen_ptr_; }
  using HidenGeneratorPtr = void*;
  HidenGeneratorPtr gen_ptr_{};
};
}  // namespace seabattlebreak

#endif  // !SEABATTLEBREAK_RANDOM_H