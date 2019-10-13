#ifndef KDT_HPP
#define KDT_HPP

#include <math.h>     // pow, abs
#include <algorithm>  // sort, max, min
#include <limits>     // numeric_limits<type>::max()
#include <vector>     // vector<typename>
#include "Point.hpp"

using namespace std;

class KDT {
  private:
    /** Inner class which defines a KD tree node */
    class KDNode {
      public:
        KDNode* left;
        KDNode* right;
        Point point;

        KDNode(Point point) : point(point) {}
    };

    // root of KD tree
    KDNode* root;

    // number of dimension of data points
    unsigned int numDim;

    // smallest squared distance to query point so far
    double threshold;

    unsigned int isize;
    int iheight;

    // current nearest neighbor
    Point nearestNeighbor;

    // Extra Credit: smallest bounding box containing all points
    vector<pair<double, double>> boundingBox;

    // Extra Credit: result container
    // (Set during searching, and clear before another search)
    vector<Point> pointsInRange;

  public:
    /** Constructor of KD tree */
    KDT()
        : root(0),
          numDim(0),
          threshold(numeric_limits<double>::max()),
          isize(0),
          iheight(-1) {}

    /** Destructor of KD tree */
    virtual ~KDT() { deleteAll(root); }

    /** Builds a balanced KD tree with points as input.
     *  @param points Vector of points to put into the KD tree.
     */
    void build(vector<Point>& points) {
        // check if vector is empty
        if (points.empty() || points.size() == 0) {
            return;
        }
        // Set numDim based on first point
        numDim = points[0].numDim;

        // sort points based on dim[0]
        std::sort(points.begin(), points.end(), CompareValueAt(0));

        // set median as root
        int start = 0;
        int end = points.size();
        int medianIndex = floor((start + end) / 2);
        root = new KDNode(points[medianIndex]);

        // increase tracking variables
        isize += 1;
        unsigned int height = 0;
        iheight = height;

        // recursively build sub trees
        root->left =
            buildSubtree(points, start, medianIndex, 1 % numDim, height);
        root->right =
            buildSubtree(points, medianIndex + 1, end, 1 % numDim, height);

        // extra credit - bounding box not used however.
        // set boundingBox as smallest box containing all points
        for (unsigned int i = 0; i < numDim; i++) {
            double lowerBound = numeric_limits<double>::max();
            double upperBound = numeric_limits<double>::min();
            for (Point p : points) {
                if (p.valueAt(i) < lowerBound) lowerBound = p.valueAt(i);
                if (p.valueAt(i) > upperBound) upperBound = p.valueAt(i);
            }
            // add boundary pair to boundingBox
            boundingBox.emplace_back(make_pair(lowerBound, upperBound));
        }
    }

    /** Returns a pointer to the nearest neighbor of a given query point in the
     *  KD tree. If KD tree is empty, return nullptr.
     *  @param queryPoint Query point to find the nearest neighbor of
     *  @return Pointer to nearest neighbor point of given query point. If tree
     *          is empty, return nullptr.
     */
    Point* findNearestNeighbor(Point& queryPoint) {
        // check if tree is empty
        if (isize == 0) {
            return nullptr;
        }

        threshold = numeric_limits<double>::max();  // reset threshold

        // call helper function to find nearest neighbor and set threshold
        findNNHelper(root, queryPoint, 0);

        return &nearestNeighbor;
    }

    /** Extra credit */
    /** Returns a vector containing all points inside query region.
     *  @param queryRegion The query region to perform region search
     *  @return Vector of all points inside query region
     */
    vector<Point> rangeSearch(vector<pair<double, double>>& queryRegion) {
        // reset pointsInRange before new search
        pointsInRange = {};
        // call helper function
        rangeSearchHelper(root, boundingBox, queryRegion, 0);
        return pointsInRange;
    }

    /** Returns the size of the KDT. This is the number of items in the tree.
     *  @return Size of KDT
     */
    unsigned int size() const { return isize; }

    /** Returns the height of the KDT. This is how many levels the tree has.
     *  Note that empty KDT has height -1 and KDT with one node has height 0.
     *  @return Height of KDT
     */
    int height() const { return iheight; }

  private:
    /** Helper method to recursively build the subtrees of KD tree.
     *  @param points Vector of all data points to insert into the KD tree
     *  @param start Inclusive start index of points to insert into subtree
     *  @param end Exclusive end index of points to insert into subtree
     *  @param curDim Current dimension when building subtree
     *  @param height Current height during building subtree
     *  @return root node of this subtree
     */
    KDNode* buildSubtree(vector<Point>& points, unsigned int start,
                         unsigned int end, unsigned int curDim, int height) {
        // base case
        if (start == end) {
            return nullptr;
        }

        // sort for this subtree
        std::sort(points.begin() + start, points.begin() + end,
                  CompareValueAt(curDim));

        // set median as new node
        int medianIndex = floor((start + end) / 2);
        KDNode* current = new KDNode(points[medianIndex]);

        // increase tracking variables
        isize += 1;
        height += 1;
        // check if we should increase height
        if (height > iheight) {
            iheight = height;
        }

        // recursively build sub trees
        current->left = buildSubtree(points, start, medianIndex,
                                     (curDim + 1) % numDim, height);
        current->right = buildSubtree(points, medianIndex + 1, end,
                                      (curDim + 1) % numDim, height);

        return current;
    }

    /** Helper method to recursively find the nearest neighbor of query
     *  point.
     *  @param node Pointer points to the current KD node being checked
     *  @param queryPoint The given query point
     *  @param curDim The current dimension being checked
     */
    void findNNHelper(KDNode* node, Point& queryPoint, unsigned int curDim) {
        // base case
        if (node == nullptr) {
            return;
        }

        // values of curDim to compare
        int nodeVal = node->point.valueAt(curDim);
        int queryVal = queryPoint.valueAt(curDim);

        unsigned int nextDim = (curDim + 1) % numDim;  // next dimension

        // if query larger than or equal to node, go right first
        if (nodeVal <= queryVal) {
            findNNHelper(node->right, queryPoint,
                         nextDim);  // right

            // if curDim difference squared < threshold, go left
            if (std::pow(nodeVal - queryVal, 2) < threshold) {
                findNNHelper(node->left, queryPoint, nextDim);
            }
        } else {  // go left first
            findNNHelper(node->left, queryPoint,
                         nextDim);  // right

            // if curDim difference squared < threshold, go left
            if (std::pow(nodeVal - queryVal, 2) < threshold) {
                findNNHelper(node->right, queryPoint, nextDim);
            }
        }

        // update threshold and nearestNeighbor for current node if needed
        node->point.setDistToQuery(queryPoint);
        if (node->point.distToQuery < threshold) {
            threshold = node->point.distToQuery;
            nearestNeighbor = node->point;
        }
    }

    /** Extra credit */
    /** Helper method to find all points inside the query region.
     *  @param node Pointer points to current KD Node being checked
     *  @param currBB The current bounding box that contains all points in this
     *                node and subtree
     *  @param queryRegion Query region to perform range search
     *  @param curDim Current dimension being checked
     */
    void rangeSearchHelper(KDNode* node, vector<pair<double, double>>& curBB,
                           vector<pair<double, double>>& queryRegion,
                           unsigned int curDim) {
        // base case
        if (node == nullptr) {
            return;
        }

        int nodeValue = node->point.valueAt(curDim);  // value of node at curDim
        unsigned int nextDim = (curDim + 1) % numDim;  // next dimension

        // if curDim node value < queryLower, go right
        if (nodeValue < queryRegion[curDim].first) {
            rangeSearchHelper(node->right, curBB, queryRegion, nextDim);

        } else if (queryRegion[curDim].second < nodeValue) {
            // if queryUpper < curDim node value, go left
            rangeSearchHelper(node->left, curBB, queryRegion, nextDim);

        } else {  // nodeValue is between (inclusive) range, go both left right
            rangeSearchHelper(node->right, curBB, queryRegion, nextDim);
            rangeSearchHelper(node->left, curBB, queryRegion, nextDim);

            bool inRange = true;  // if current node's point is in region

            // check if other dimensions fall in query region
            for (unsigned int dim = nextDim; dim != curDim;
                 dim = (dim + 1) % numDim) {
                int curVal = node->point.valueAt(dim);
                if (curVal < queryRegion[dim].first ||
                    queryRegion[dim].second < curVal) {  // if out of range
                    inRange = false;
                    break;
                }
            }

            // add node if in region
            if (inRange) {
                pointsInRange.emplace_back(node->point);
            }
        }
    }

    /** Deletes every node in the KDT.
     *  @param n Node to delete subtree of and the node itself.
     */
    static void deleteAll(KDNode* n) {
        // base case
        if (n == nullptr) {
            return;
        }

        deleteAll(n->left);
        deleteAll(n->right);
        delete (n);
    }

    // Add your own helper methods here
};
#endif  // KDT_HPP
