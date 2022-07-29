#include <seabattlebreak\field.h>

#include <gtest/gtest.h>

namespace {

	TEST(Field, Empty) {
		seabattlebreak::Field f{};

		EXPECT_TRUE(f.isEmpty());

	}
}// namespace
