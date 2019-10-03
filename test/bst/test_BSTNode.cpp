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

TEST(BST_NODE_TESTS, TEST_INITIALIZATION_HEAP) {
    BSTNode<int>* node = new BSTNode<int>(3);
    ASSERT_EQ(node->data, 3);
    // assert node on heap works
    delete (node);
}

/* Successor test starts here */
// No Successor
TEST(BST_NODE_TESTS, TEST_SUCCESSOR) {
    BSTNode<int> node(3);
    ASSERT_EQ(node.successor(), nullptr);
}

/**
 * Builds the following BST with nodes only:
 *         4
 *       /  \
 *      2    6
 *     / \   /
 *    1   3 5
 */
class BSTNodeFixture : public ::testing::Test {
  protected:
    BSTNode<int> node1, node2, node3, node4, node5, node6;

  public:
    BSTNodeFixture()
        : node1(1), node2(2), node3(3), node4(4), node5(5), node6(6) {}
    // Set the left, right, parent pointers for the nodes to make the tree
    void SetUp() {
        node4.left = &node2;
        node4.right = &node6;
        node2.parent = &node4;
        node2.left = &node1;
        node2.right = &node3;
        node1.parent = &node2;
        node3.parent = &node2;
        node6.parent = &node4;
        node6.left = &node5;
        node5.parent = &node6;
    }
};

TEST_F(BSTNodeFixture, PARENT_TEST) {
    // first parent is successor
    ASSERT_EQ(node1.successor(), &node2);
}

TEST_F(BSTNodeFixture, RIGHT_CHILD_TEST) {
    // right child is successor
    ASSERT_EQ(node2.successor(), &node3);
}

TEST_F(BSTNodeFixture, GO_UP_TEST) {
    // node is right child with no children
    ASSERT_EQ(node3.successor(), &node4);
}

TEST_F(BSTNodeFixture, GO_DOWN_TEST) {
    // node has right child with a left child
    ASSERT_EQ(node4.successor(), &node5);
}

TEST_F(BSTNodeFixture, LAST_NODE_TEST) {
    // no successor for largest node
    ASSERT_EQ(node6.successor(), nullptr);
}