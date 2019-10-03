#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include <vector>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"
using namespace std;

template <typename Data>
class BST {
  protected:
    // pointer to the root of this BST, or 0 if the BST is empty
    BSTNode<Data>* root;

    // number of Data items stored in this BST.
    unsigned int isize;

    // height of this BST.
    int iheight;

  public:
    /** Define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /** Default constructor.
     *  Initialize an empty BST.
     */
    BST() : root(0), isize(0), iheight(-1) {}

    /** Deconstructor.
     *  Deletes the pointer to the root after deleting the rest of the nodes.
     */
    virtual ~BST() { deleteAll(root); }

    /** Inserts a new BSTNode containing given Data into the tree in the correct
     *  order.
     *  @param item Data to insert into a BSTNode that will go in the tree
     *  @return True if successfully inserted. False if duplicate insertion
     *          or otherwise.
     */
    virtual bool insert(const Data& item) {
        // no root, then new node is root
        if (root == nullptr) {
            root = node;
            ++isize;
            iheight = 0;
            return true;
        }

        // create new node with item
        BSTNode<Data>* node = new BSTNode<Data>(item);
        int height = 0;  // keep track of height as we go down BST

        BSTNode<Data>* curr = root;     // start with root node
        BSTNode<Data>* prev = nullptr;  // holds previous node so we can assign

        while (curr != nullptr) {  // loop till we find a spot for new node
            prev = curr;
            ++height;
            if (item < curr->data) {  // go left
                curr = curr->left;
            } else if (curr->data < item) {  // go right
                curr = curr->right;
            } else {            // duplicate insertion
                delete (node);  // delete heap item before returning
                return false;
            }
        }
        // found spot for new node
        if (item < prev->data) {
            prev->left = node;
            node->parent = prev;
        } else if (prev->data < item) {
            prev->right = node;
            node->parent = prev;
        }

        // increase height if we added a new level
        if (iheight < height) {
            iheight = height;
        }
        ++isize;  // added a node, so increment size

        return true;
    }

    /** Find a Data item in the BST. Return iterator pointing to that item or
     *  past the last node in BST if not found.
     *  @param item Data to be found in BST
     *  @return iterator pointing to node with data or nullptr if not found.
     */
    virtual iterator find(const Data& item) const {
        BSTNode<Data>* curr = root;  // used to traverse BST

        while (curr != nullptr) {     // loop till we reach end or find item
            if (item < curr->data) {  // go left
                curr = curr->left;
            } else if (curr->data < item) {  // go right
                curr = curr->right;
            } else {  // found item
                break;
            }
        }

        return iterator(curr);
    }

    /** Returns the number of items currently in BST.
     *  @return Number of items in BST.
     */
    unsigned int size() const { return isize; }

    /** Returns the height of BST.
     *  @return Height of BST
     */
    int height() const { return iheight; }

    /** Returns if BST is empty or not with 0 items.
     *  @return True if BST is empty. False otherwise.
     */
    bool empty() const { return (isize == 0 && root == nullptr); }

    /** Return an iterator pointing to root of the BST by calling first.
     *  @return Iterator pointing to root of BST.
     */
    iterator begin() const { return BST::iterator(first(root)); }

    /** Return an iterator pointing past the last item in the BST.
     *  @return Iterator pointing to end of BST, past last time.
     */
    iterator end() const { return typename BST<Data>::iterator(0); }

    /** Perform in order traversal through BST and return the order as vector.
     *  @return Vector that contains data of BST in order from smallest to
     *          largest.
     */
    vector<Data> inorder() const {
        vector<Data> order{};
        iterator iter = begin();  // start from smallest element and iterate
        iterator endBST = end();
        while (iter != endBST) {  // stop when we reach end of BST
            order.push_back(*iter);
            ++iter;
        }
        return order;
    }

  private:
    /** Returns the smallest or first element of BST.
     *  @param root Root in BST
     *  @return BSTNode * to smallest element in BST
     */
    static BSTNode<Data>* first(BSTNode<Data>* root) {
        if (root == nullptr) {  // empty tree check
            return 0;
        }

        BSTNode<Data>* curr = root;
        while (curr->left != nullptr) {  // keep going left until cant anymore
            curr = curr->left;
        }

        return curr;
    }

    /** Deletes all the nodes below given node in BST.
     *  @param n Node to start deleting nodes from
     */
    static void deleteAll(BSTNode<Data>* n) {
        /* Pseudocode:
           if current node is null: return;
           recursively delete left sub-tree
           recursively delete right sub-tree
           delete current node
        */
        if (n == nullptr) {
            return;
        }
        deleteAll(n->left);
        deleteAll(n->right);
        delete (n);
    }
};

#endif  // BST_HPP
