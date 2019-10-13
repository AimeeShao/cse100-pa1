#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "KDT.hpp"
#include "NaiveSearch.hpp"
#include "Point.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Test empty tree */
TEST(KdtTests, TEST_SIZE_EMPTY) {
    KDT kdt;
    // Assert size is correct in empty tree
    ASSERT_EQ(kdt.size(), 0);
}

TEST(KdtTests, TEST_HEIGHT_EMPTY) {
    KDT kdt;
    // Assert height is correct in empty tree
    ASSERT_EQ(kdt.height(), -1);
}

TEST(KdtTests, TEST_BUILD_EMPTY) {
    KDT kdt;
    vector<Point> vec;
    // test building empty vector
    kdt.build(vec);
}

TEST(KdtTests, TEST_FIND_EMPTY) {
    KDT kdt;
    Point queryPoint({5.81, 3.21});
    // Assert finding nearest neighbor of empty tree is nullptr
    ASSERT_EQ(kdt.findNearestNeighbor(queryPoint), nullptr);
}

/* Test root build */
TEST(KdtTests, TEST_SIZE_ROOT) {
    KDT kdt;
    vector<Point> vec;
    vec.emplace_back(Point({1.0, 3.2}));
    kdt.build(vec);
    // Assert size is correct after inserting root
    ASSERT_EQ(kdt.size(), 1);
}

TEST(KdtTests, TEST_HEIGHT_ROOT) {
    KDT kdt;
    vector<Point> vec;
    vec.emplace_back(Point({1.0, 3.2}));
    kdt.build(vec);
    // Assert size is correct after inserting root
    ASSERT_EQ(kdt.height(), 0);
}

/**
 * A simple test fixture from which multiple tests can
 * be written. The structure of the KDT built from this
 * may vary. See test_BST.cpp for more information on
 * test fixtures.
 */
class SmallKDTFixture : public ::testing::Test {
  protected:
    vector<Point> vec;
    KDT kdt;

  public:
    SmallKDTFixture() {
        vec.emplace_back(Point({1.0, 3.2}));
        vec.emplace_back(Point({3.2, 1.0}));
        vec.emplace_back(Point({5.7, 3.2}));
        vec.emplace_back(Point({1.8, 1.9}));
        vec.emplace_back(Point({4.4, 2.2}));
        kdt.build(vec);
    }
};

TEST_F(SmallKDTFixture, TEST_SIZE) {
    // Assert that the kd tree has the correct size
    ASSERT_EQ(kdt.size(), 5);
}

TEST_F(SmallKDTFixture, TEST_HEIGHT) {
    // Assert that the kd tree has the correct height
    ASSERT_EQ(kdt.height(), 2);
}

TEST_F(SmallKDTFixture, TEST_NEAREST_POINT) {
    // Assert nearestNeighbor is correct
    NaiveSearch naiveSearch;
    naiveSearch.build(vec);
    Point queryPoint({5.81, 3.21});
    Point* closestPoint = naiveSearch.findNearestNeighbor(queryPoint);
    ASSERT_EQ(*kdt.findNearestNeighbor(queryPoint), *closestPoint);

    // second nearest point test
    queryPoint = Point({1.0, 9.01});
    closestPoint = naiveSearch.findNearestNeighbor(queryPoint);
    ASSERT_EQ(*kdt.findNearestNeighbor(queryPoint), *closestPoint);
}

/**
 * A simple test fixture for building a large KDT.
 * Taken from Nearest Neighbor Algorithm worksheet.
 */
class LargeKDTFixture : public ::testing::Test {
  protected:
    vector<Point> vec;
    KDT kdt;

  public:
    LargeKDTFixture() {
        vec.emplace_back(Point({3, 1}));
        vec.emplace_back(Point({9, 8}));
        vec.emplace_back(Point({7, 9}));
        vec.emplace_back(Point({4, 3}));
        vec.emplace_back(Point({8, 7}));
        vec.emplace_back(Point({10, 10}));
        vec.emplace_back(Point({2, 5}));
        vec.emplace_back(Point({1, 4}));
        vec.emplace_back(Point({6, 6}));
        vec.emplace_back(Point({5, 2}));
        kdt.build(vec);
    }
};

TEST_F(LargeKDTFixture, TEST_LARGE_SIZE) {
    // Assert size of large fixture is correct
    ASSERT_EQ(kdt.size(), 10);
}

TEST_F(LargeKDTFixture, TEST_LARGE_HEIGHT) {
    // Assert height of large fixture is correct
    ASSERT_EQ(kdt.height(), 3);
}

TEST_F(LargeKDTFixture, TEST_LARGE_NEAREST_POINT) {
    // Assert nearestNeighbor is correct
    NaiveSearch naiveSearch;
    naiveSearch.build(vec);
    Point queryPoint({4, 8});
    Point* closestPoint = naiveSearch.findNearestNeighbor(queryPoint);
    ASSERT_EQ(*kdt.findNearestNeighbor(queryPoint), *closestPoint);
}

TEST_F(LargeKDTFixture, TEST_RANGE_SEARCH) {
    // Assert range search is correct
    vector<pair<double, double>> queryRegion;
    queryRegion.emplace_back(make_pair(3, 6));
    queryRegion.emplace_back(make_pair(3, 6));

    vector<Point> answer;
    answer.emplace_back(Point({4, 3}));
    answer.emplace_back(Point({6, 6}));
    kdt.rangeSearch(queryRegion);
    ASSERT_EQ(kdt.rangeSearch(queryRegion), answer);
}
