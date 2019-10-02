#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BSTNode.hpp"

using namespace std;
using namespace testing;

/* Initialization test starts here */
TEST(BST_NODE_TESTS, TEST_INITIALIZATION) {
    BSTNode<int> node(3);
    ASSERT_EQ(node.data, 3);
    ASSERT_EQ(node.left, nullptr);
    ASSERT_EQ(node.right, nullptr);
    ASSERT_EQ(node.parent, nullptr);
}

/* Successor test starts here */
// No Successor
TEST(BST_NODE_TESTS, TEST_SUCCESSOR) {
    BSTNode<int> node(3);
    ASSERT_EQ(node.successor(), nullptr);
}