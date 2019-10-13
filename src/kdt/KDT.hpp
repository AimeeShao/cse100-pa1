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
        vector<Point> currBB;
        rangeSearchHelper(root, currBB, queryRegion, 0);
        return currBB;
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

        int nextDim = (curDim + 1) % numDim;  // next dimension to compare

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
        int nextDim = (curDim + 1) % numDim;          // next dimension

        // if curDim node value < queryLower, go right
        if (nodeValue < queryRegion[curDim][0]) {
            rangeSearchHelper(node->right, currBB, queryRegion, nextDim);
        }
        // if queryUpper < curDim node value, go left
        if (queryRegion[curDim][1] < nodeValue) {
            rangeSearchHelper(node->left, currBB, queryRegion, nextDim);
        }
        // if curDim node value is in between (inclusive) range, then go both
        if (queryRegion[curDim][0] < nodeValue &&
            nodeValue < queryRegion[curDim][1]) {
            rangeSearchHelper(node->right, currBB, queryRegion, nextDim);
            rangeSearchHelper(node->left, currBB, queryRegion, nextDim);

            // check if other dimensions falls in query region
            for (int i = nextDim; i != curDim; i = (i + 1) % nextDim) {
            }
        }

        /** Deletes every node in the KDT.
         *  @param n Node to delete subtree of and the node itself.
         */
        static void deleteAll(KDNode * n) {
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
