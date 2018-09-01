#include "act.cpp"
#include "gtest/gtest.h"

using namespace olymplib;

TEST(SIMPLE_ACT, SQUARE) {
    constexpr int TESTLEN = 10;
    int* a = new int[TESTLEN];
    for (int i = 0; i < TESTLEN; ++i) {
        a[i] = i;
    }
    CallStuff(a, a + TESTLEN, [](int& a) { a *= a; });
    
    for (int i = 0; i < TESTLEN; ++i) {
        EXPECT_EQ(a[i], i * i);
    }
    delete[] a;
}
