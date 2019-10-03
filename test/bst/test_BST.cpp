#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BST.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty BST test starts here */

TEST(BSTTests, EMPTY_TREE_HEIGHT_TEST) {
    BST<int> bst;
    ASSERT_EQ(bst.height(), -1);
}

TEST(BSTTests, EMPTY_TREE_EMPTY_TEST) {
    BST<int> bst;
    ASSERT_TRUE(bst.empty());
}

TEST(BSTTests, EMPTY_TREE_INSERT_TEST) {
    BST<int> bst;
    // assert insert works
    ASSERT_TRUE(bst.insert(3));
}

TEST(BSTTests, EMPTY_TREE_INORDER_TEST) {
    BST<int> bst;
    // assert inorder/begin works
    ASSERT_EQ(bst.inorder(), vector<int>{});
}

TEST(BSTTests, EMPTY_TREE_FIND_TEST) {
    BST<int> bst;
    // assert find works
    ASSERT_EQ(bst.find(3), bst.end());
}

TEST(BSTTests, EMPTY_TREE_BEGIN_TEST) {
    BST<int> bst;
    // assert begin works with empty tree
    ASSERT_EQ(bst.begin(), bst.end());
}

TEST(BSTTests, EMPTY_TREE_DELETE_TEST) {
    BST<int>* bst = new BST<int>();
    // confirm deconstruct empty tree works
    delete (bst);
}

TEST(BSTTests, DELETE_BST_TEST) {
    BST<int>* bst = new BST<int>();
    vector<int> input{5, 2, 4, 3, 8, 7, 10};
    insertIntoBST(input, *bst);
    // assert deconstructor and deleteAll method works correctly
    delete (bst);
}

/* Small BST test starts here */

/**
 * A simple test fixture from which multiple tests
 * can be created. All fixture tests (denoted by the
 * TEST_F macro) can access the protected members of this
 * fixture. Its data is reset after every test.
 *
 * Builds the following BST:
 *         3
 *        / \
 *       1   4
 *      /     \
 *    -33     100
 */
class SmallBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    SmallBSTFixture() {
        // initialization code here
        vector<int> input{3, 4, 1, 100, -33};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

TEST_F(SmallBSTFixture, SMALL_SIZE_TEST) {
    // assert that the small BST has the correct size
    ASSERT_EQ(bst.size(), 5);
}

TEST_F(SmallBSTFixture, SMALL_INSERT_DUPLICATES_TEST) {
    // assert failed duplicate insertion
    ASSERT_FALSE(bst.insert(3));
}

TEST_F(SmallBSTFixture, INSERT_TEST) {
    // assert true insertion
    ASSERT_TRUE(bst.insert(2));
}

TEST_F(SmallBSTFixture, NOT_EMPTY_TEST) {
    // assert BST is not empty
    ASSERT_FALSE(bst.empty());
}

TEST_F(SmallBSTFixture, BEGIN_TEST) {
    // test that iterator returned by bst.begin is correct
    ASSERT_EQ(*bst.begin(), -33);
}

TEST_F(SmallBSTFixture, SMALL_HEIGHT_TEST) {
    // assert correct height
    ASSERT_EQ(bst.height(), 2);

    // add two more nodes to see change in height
    bst.insert(2);
    ASSERT_EQ(bst.height(), 2);

    bst.insert(-1);
    ASSERT_EQ(bst.height(), 3);
}

TEST_F(SmallBSTFixture, INORDER_TEST) {
    // assert inorder function works
    bst.insert(2);
    vector<int> order{-33, 1, 2, 3, 4, 100};
    ASSERT_EQ(bst.inorder(), order);
}

TEST_F(SmallBSTFixture, FIND_DATA_TEST) {
    // assert data found works
    ASSERT_EQ(*bst.find(3), 3);
    ASSERT_EQ(*bst.find(100), 100);
}

TEST_F(SmallBSTFixture, FIND_NONE_TEST) {
    // assert find works correctly when data not found
    ASSERT_EQ(bst.find(0), bst.end());
}