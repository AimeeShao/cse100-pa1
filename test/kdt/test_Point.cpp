#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Point.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

TEST(PointTests, TEST_EQUALS) {
    vector<double> p1Values{3, 4, 5, 6};
    Point p1(p1Values);

    vector<double> p2Values{3, 4, 5};
    Point p2(p2Values);

    // Assert two non-equal points are not equal.
    ASSERT_NE(p1, p2);
}

TEST(PointTests, TEST_EQUALS_OPERATOR) {
    vector<double> p1Values{3, 4, 5};
    Point p1(p1Values);

    vector<double> p2Values{3, 4, 6};
    Point p2(p2Values);

    // Assert two non-equal points are not equal with == operator.
    ASSERT_FALSE(p1 == p2);
}

TEST(PointTests, TEST_DISTANCE) {
    vector<double> p1Values{3, 4, 5, 6};
    vector<double> p2Values{3, 4, 5, 7};

    Point p1(p1Values);
    Point p2(p2Values);

    p1.setDistToQuery(p2);
    // Assert distance to query is set correctly
    ASSERT_DOUBLE_EQ(p1.distToQuery, 1.0);
}

TEST(PointTests, TEST_OUTPUT_OPERATOR) {
    vector<double> pValues{3, 4, 5, 6};
    Point p(pValues);
    // Assert output works correctly
    cerr << "TEST_OUTPUT_OPERATOR: " << p << endl;
}