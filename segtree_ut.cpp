#include "segtree.cpp"
#include "gtest/gtest.h"
#include <random>
#include <vector>
#include <iostream>

using namespace olymplib;
using namespace std;

vector <int> arr;
struct test_node : public internal_tree_node {
		int mn;
		test_node(int index) {
				mn = arr[index];
		}
};

struct test_summer {
		test_node operator() (const test_node& l, const test_node& r) {
				test_node t(l);
				t.mn += r.mn;
				return t;
		}
};

struct test_query : public internal_tree_query {
		int place;
		test_query() {};
		test_query(int l, int r, int x)
				:internal_tree_query(l,r)
				, place(x) 
		{};
};

struct test_updater {
		test_node& operator()(test_node& t, const test_query& q) {
				t.mn = q.place * (t.r - t.l + 1);
				return t;
		}
};
 
class SimpleTest : public ::testing::Test {
protected:      
    static constexpr int TESTSIZE = 1000;
    SimpleTest() {
        arr.resize(TESTSIZE);
	      srand(1488); //TODO(noobgam) fix this trash
        for (int i = 0; i < TESTSIZE; ++i) {
            arr[i] = roll(0, 1000);
        }
        // You can do set-up work for each test here.
    }

    int roll(int mn, int mx) {
	return rand() % (mx - mn + 1) + mn;
    }
};

TEST_F(SimpleTest, Correctness) {
   int n = 500;
   segment_tree<test_node, test_query, test_summer, test_updater> t(1, n);
   for (int i = 0; i < TESTSIZE; ++i) {
       int l = roll(1, n);
       int r = roll(l, n);
       int check = 0;
       for (int i = l; i <= r; ++i) {
           check += arr[i];
       }
       EXPECT_EQ(check, t.ask(l,r).mn);
   } 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
