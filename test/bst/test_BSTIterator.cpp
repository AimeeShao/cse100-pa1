#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"

using namespace std;
using namespace testing;

/* Testing incrementing*/
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_INCREMENT) {
    BSTNode<int> node(3);
    BSTIterator<int> iter(&node);

    ASSERT_NE(iter, nullptr);

    iter++;

    ASSERT_EQ(iter, nullptr);
}

/* Test equality */
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_EQUALITY) {
    BSTNode<int> node1(3);
    BSTIterator<int> iter1(&node1);
    BSTIterator<int> iter2(&node1);

    ASSERT_EQ(iter1 == iter2, true);

    BSTNode<int> node2(3);
    iter2 = BSTIterator<int>(&node2);
    ASSERT_EQ(iter1 == iter2, false);
}

/* Inquality test */
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_INEQUALITY) {
    BSTNode<int> node1(5);
    BSTIterator<int> iter1(&node1);
    BSTIterator<int> iter2(&node1);

    ASSERT_EQ(iter1 != iter2, false);

    BSTNode<int> node2(5);
    iter2 = BSTIterator<int>(&node2);
    ASSERT_EQ(iter1 != iter2, true);
}

/* Dereference test */
TEST(BST_ITERATOR_TEST, TEST_DEREFERENCE) {
    BSTNode<int> node(1);
    BSTIterator<int> iter(&node);

    ASSERT_EQ(*iter, node.data);
}