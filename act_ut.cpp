#include "act.cpp"
#include "gtest/gtest.h"

using namespace olymplib;

TEST(SIMPLE_ACT, SQUARE) {
    constexpr int TESTLEN = 1e8;
    long long* a = new long long[TESTLEN];
    for (int i = 0; i < TESTLEN; ++i) {
        a[i] = i;
    }
    CallStuff(a, a + TESTLEN, [](long long& a) { a *= a; });
    
    for (int i = 0; i < TESTLEN; ++i) {
        EXPECT_EQ(a[i], 1LL * i * i);
    }
    delete[] a;
}

TEST(PARALLEL_ACT, SQUARE) {
    constexpr int TESTLEN = 1e8;
    long long* a = new long long[TESTLEN];
    for (int i = 0; i < TESTLEN; ++i) {
        a[i] = i;
    }
    CallStuff(a, a + TESTLEN, [](long long& a) { a *= a; }, 4);
    
    for (int i = 0; i < TESTLEN; ++i) {
        EXPECT_EQ(a[i], 1LL * i * i);
    }
    delete[] a;
}

