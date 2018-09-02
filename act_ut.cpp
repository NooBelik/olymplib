#include "act.cpp"
#include "gtest/gtest.h"

using namespace olymplib;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
