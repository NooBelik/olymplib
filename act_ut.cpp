#include "act.cpp"
#include "gtest/gtest.h"

using namespace olymplib;

class PreAllocatedActCorrectness : public ::testing::Test {
protected:
    static constexpr int TESTLEN = 1e8;
    long long* memory;

    PreAllocatedActCorrectness() {
        memory = new long long[TESTLEN];
        for (int i = 0; i < TESTLEN; ++i) {
            memory[i] = i;
        }
    }

    virtual ~PreAllocatedActCorrectness() {
        for (int i = 0; i < TESTLEN; ++i) {
            EXPECT_EQ(memory[i], 1LL * i * i);
        }
        delete[] memory;
    }

};

TEST_F(PreAllocatedActCorrectness, SquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; });
}

TEST_F(PreAllocatedActCorrectness, ParallelSquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; }, 2);
}

class PreAllocatedActPerfomance : public ::testing::Test {
protected:
    static long long* memory;
    static constexpr int TESTLEN = 1e9;

    PreAllocatedActPerfomance() {
    }

    virtual ~PreAllocatedActPerfomance() {
    }

    static void SetUpTestCase() {
      memory = new long long[TESTLEN];
    }

    static void TearDownTestCase() {
      delete[] memory;
    }
};

long long* PreAllocatedActPerfomance::memory;

TEST_F(PreAllocatedActPerfomance, SquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; });
}

TEST_F(PreAllocatedActPerfomance, ParallelSquareCorrectness) {
    CallStuff(memory, memory + TESTLEN, [](long long& a) { a *= a; }, 2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
