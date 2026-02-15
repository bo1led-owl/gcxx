#include <cassert>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <set>

#include "avl.hpp"

#define TEST(a, b, content) void a##_##b() content
#define ASSERT_TRUE(a) assert(a);
#define ASSERT_FALSE(a) assert(!a);
#define ASSERT_EQ(a, b) assert(a == b);
#define ASSERT_NEQ(a, b) assert(a != b);

TEST(AvlTest, LinearInsertion, {
    AVL a;
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.insert(90 - 1 - i));
    }
});

TEST(AvlTest, ReversedLinearInsertion, {
    AVL a;
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.insert(i));
    }
});

TEST(AvlTest, LinearReinsertion, {
    AVL a;
    ASSERT_TRUE(a.insert(10));
    for (int i = 0; i < 90; i++) {
        ASSERT_FALSE(a.insert(10));
    }
});

TEST(AvlTest, LinearDeletion, {
    AVL a;
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.insert(i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.remove(i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_FALSE(a.remove(i));
    }
});

TEST(AvlTest, ReversedLinearDeletion, {
    AVL a;
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.insert(i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.remove(90 - 1 - i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_FALSE(a.remove(90 - 1 - i));
    }
});

TEST(AvlTest, LinearFind, {
    AVL a;
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.insert(i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_EQ(a.find(i), i);
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_TRUE(a.remove(i));
    }
    for (int i = 0; i < 90; i++) {
        ASSERT_EQ(a.find(i), INT_MIN);
    }
});

TEST(AvlTest, MinMaxSingleElement, {
    AVL a;
    a.insert(1);
    ASSERT_EQ(a.min(), 1);
    ASSERT_EQ(a.max(), 1);
});

TEST(AvlTest, MinMaxAfterOperations, {
    AVL a;
    a.insert(5);
    a.insert(3);
    a.insert(7);
    a.insert(2);
    a.insert(4);
    ASSERT_EQ(a.min(), 2);
    ASSERT_EQ(a.max(), 7);
    a.remove(2);
    ASSERT_EQ(a.min(), 3);
    a.remove(7);
    ASSERT_EQ(a.max(), 5);
});

TEST(AvlTest, SelectSingle, {
    AVL a;
    a.insert(1);
    ASSERT_EQ(a.select(0), 1);
    ASSERT_EQ(a.select(2), INT_MIN);
});

TEST(AvlTest, RankSingle, {
    AVL a;
    a.insert(1);
    ASSERT_EQ(a.rank(0), 0);
    ASSERT_EQ(a.rank(1), 0);
    ASSERT_EQ(a.rank(2), 1);
});

TEST(AvlTest, SetFuzz, {
    AVL a;
    std::set<int> s;
    std::srand(std::time(0));

    for (int i = 0; i < 2048; ++i) {
        int op = rand() % 3;
        int val = rand() % 90;
        if (op == 0) {
            bool inserted = a.insert(val);
            ASSERT_EQ(inserted, s.insert(val).second);
        } else if (op == 1) {
            bool removed = a.remove(val);
            ASSERT_EQ(removed, s.erase(val) == 1);
        } else {
            int found = a.find(val);
            auto it = s.find(val);
            if (it != s.end()) {
                ASSERT_EQ(found, val);
            } else {
                ASSERT_EQ(found, INT_MIN);
            }
        }
        ASSERT_TRUE(a.verify());
    }
});

int main(void) {
    AvlTest_LinearInsertion();
    AvlTest_LinearDeletion();
    AvlTest_ReversedLinearDeletion();
    AvlTest_ReversedLinearInsertion();
    AvlTest_MinMaxAfterOperations();
    AvlTest_MinMaxSingleElement();
    AvlTest_SelectSingle();
    AvlTest_SetFuzz();
}
