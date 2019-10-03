#ifndef BSTNODE_HPP
#define BSTNODE_HPP
#include <iomanip>
#include <iostream>
using namespace std;

template <typename Data>

class BSTNode {
  public:
    BSTNode<Data>* left;
    BSTNode<Data>* right;
    BSTNode<Data>* parent;
    Data const data;  // the const Data in this node.

    /** Constructor.
     * Initialize a BSTNode with given Data, with no parent and no children.
     * @param d Data/element of this node.
     */
    BSTNode(const Data& d) : data(d) { left = right = parent = nullptr; }

    /** Returns the successor of this BSTNode.
     * The successor is the node with the smallest element that is larger than
     * this node's.
     * @return Successor node or nullptr
     */
    BSTNode<Data>* successor() {
        // has a right subtree, successor is leftmost node in right subtree
        if (right != nullptr) {
            BSTNode<Data>* current = right;
            // traverse right subtree;
            while (current->left != nullptr) {
                current = current->left;
            }
            return current;
        } else {  // no right child
            BSTNode<Data>* current = this;
            // go up until current is a left child
            while (current->parent != nullptr) {
                if (current->parent->left == current) {
                    return current
                        ->parent;  // if left child, parent is successor
                } else {           // keep going up
                    current = current->parent;
                }
            }

            // never found parent successor, no successor
            if (current->parent == nullptr) return 0;

            return current;
        }
    }
};

/** DO NOT CHANGE THIS METHOD
 *  Overload operator<< to print a BSTNode's fields to an ostream.
 */
template <typename Data>
ostream& operator<<(ostream& stm, const BSTNode<Data>& n) {
    stm << '[';
    stm << setw(10) << &n;                  // address of the BSTNode
    stm << "; p:" << setw(10) << n.parent;  // address of its parent
    stm << "; l:" << setw(10) << n.left;    // address of its left child
    stm << "; r:" << setw(10) << n.right;   // address of its right child
    stm << "; d:" << n.data;                // its data field
    stm << ']';
    return stm;
}

#endif  // BSTNODE_HPP
