#include "act.cpp"
#include "gtest/gtest.h"

using namespace olymplib;

class PreAllocatedAct : public ::testing::Test {
protected:
    static constexpr int TESTLEN = 1e7;
    long long* memory;

    PreAllocatedAct() {
        memory = new long long[TESTLEN];
        for (int i = 0; i < TESTLEN; ++i) {
            memory[i] = i;
        }
    }

    virtual ~PreAllocatedAct() {
        for (int i = 0; i < TESTLEN; ++i) {
            EXPECT_EQ(memory[i], 1LL * i * i);
        }
        delete[] memory;
    }
};

TEST_F(PreAllocatedAct, SquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; });
}

TEST_F(PreAllocatedAct, ParallelSquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; }, 2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
