#include "BSTSet.h"
#include <iostream>
#include <vector>
#include "ArrayStack.cpp"

using namespace std;

BSTSet::BSTSet()
{
    // Initialize an empty tree
    this->root = NULL;
}

BSTSet::BSTSet(const std::vector<int>& input)
{
    // If the input vector is empty initialize and empty tree
    if (input.empty()) {
        new BSTSet();
        return;
    }
    // Otherwise initialize the root node and loop through every digit of the set and add it to the binary search tree
    else {
        this->root = new TNode(input[0], NULL, NULL); // Set the first digit to the root node
        for (int i = 1; i < input.size(); i++) { // Start the index from the second digit
            this->add(input[i]);
        }
    }
    
}

// Recursive destructor for the entire binary tree
void BSTSet::deleteRec(TNode* target_node) {
    // Utilizing post-order traversal to delete every node and free up memory
    if (target_node!=NULL){
        deleteRec(target_node->left);
        deleteRec(target_node->right);
        delete(target_node);
    }
}

// Binary tree destructor
BSTSet::~BSTSet()
{
    this->deleteRec(this->root);
}

// Loop through the entire binary search tree to determine whether a node contains the input integer
bool BSTSet::isIn(int v)
{
    // Create a reader node
    TNode *current_node = this->root;
    
    // Search the branch of the tree that contains the digit
    while (current_node != NULL) {
        // Move to the left branch if the input integer is smaller
        if (v < current_node->element) {
            current_node = current_node->left;
        }
        else if (v > current_node->element) {
            current_node = current_node->right;
        }
        else {
            // We have found the desired node!
            return true;
        }
    }
    return false;
}

// Adding a new node to the binary search tree if it does not already exist
void BSTSet::add(int v)
{
    if (isIn(v)) {
        // Do not repeat nodes
    }
    else {
        TNode* new_node = new TNode(v, NULL, NULL);

        // Create reader head nodes
        TNode* current_node = this->root;
        TNode* previous_node = NULL;

        // Utilizing the same method of isIn(), move our readerhead to the end of a desired branch based on its relative size
        while (current_node != NULL) {
            previous_node = current_node;
            if (v < current_node->element) {
                    current_node = current_node->left;
            }
            else if (v > current_node->element) {
                    current_node = current_node->right;
            }
        }

        // If the previous node is NULL then we know that the tree is empty and we are adding a root node
        if (previous_node == NULL) {
            this->root = new_node;
        }
        else if (current_node == NULL) { // We have reached the edge of a branch
            // Depending on the size of the parent node, create and move the new node to either the left or right child node
            if (v < previous_node->element) {
                previous_node->left = new_node;
            }
            else {
                previous_node->right = new_node;
            }

        }       
    }

}

// Finding the smallest node in a subtree
const TNode* BSTSet::finMin(TNode* node_target) {
    if (node_target == NULL)
        return NULL;
    // Move all the way to the left of a subtree and return the smallest node
    while (node_target->left != NULL) {
        node_target = node_target->left; 
    }
    return node_target; // Return minimum 
}

// Recursive function to remove a node containing the integer 'e'
void BSTSet::removeRec(int e, TNode* &node_target)
{
    if (node_target == NULL) // If the root node is NULL the tree is empty or we are an end node
        return;

    if (e < node_target->element) {  // find the node in the tree using binary search
        removeRec(e, node_target->left);
    }
    else if (e > node_target->element) {
        removeRec(e, node_target->right);

     
    }
    // Case 3 - Delete node with 2 children (Method 1)
    else if (node_target->left != NULL && node_target->right != NULL)
    {
        // Replace the data of target node with that of the smallest node in the right subtree
        node_target->element = finMin(node_target->right)->element;
        // Delete the smallest node in the right subtree
        removeRec(node_target->element, node_target->right);
    }

    // Case 1 and 2 both handled here.  Think about how
    else
    {
        // Delete the node and adjust the parent to point to the only child
        TNode* oldNode = node_target;
        node_target = (node_target->left != NULL) ? node_target->left : node_target->right;
        delete oldNode;
    }
}

// Remove the node in a binary search tree containing the element 'v'
bool BSTSet::remove(int v)
{
    
    if (this->root == NULL) { // The tree is empty
        return false;
    }
    else if (this->isIn(v) == true) { // The tree contains the value
        this->removeRec(v, this->root); // Call the recursive remove function starting from the root
        return true;
    }
    else {
        return false;
    }
}



void BSTSet::UnionRec(TNode* node_target)
{
    // Traverse through the input tree using pre-order traversal. Add every element to this tree that is not a duplicate
    if (node_target != NULL) {
        this->add(node_target->element);
        UnionRec(node_target->left);
        UnionRec(node_target->right);        
    }
}

void BSTSet::Union(const BSTSet& s)
{
    if (s.root != NULL) { // If the tree is not empty call the recursive union function
        this->UnionRec(s.root);
    }
    
}

void BSTSet::intersectionRec(TNode* node_target, BSTSet& input_tree, BSTSet& this_tree)
{
    // Traverse through this tree object using pre-order traversal. If the input tree contains a value in this tree then add it to the copy of the empty tree representing this object

    if (node_target != NULL) {
        if (input_tree.isIn(node_target->element)) {
            this_tree.add(node_target->element);
        }
        else {
        }
        intersectionRec(node_target->left, input_tree, this_tree);
        intersectionRec(node_target->right, input_tree, this_tree);
    }
}

void BSTSet::intersection(const BSTSet& s)
{
    // If neither tree is empty create copies of the input & current trees and call the recursive intersection function
    if (s.root != NULL && this->root!=NULL ) { 
        BSTSet* input_tree = new BSTSet();
        BSTSet* copy_this_tree = new BSTSet();
        input_tree->root = s.root;
        this->intersectionRec(this->root, *input_tree, *copy_this_tree);
        this->root = copy_this_tree->root; // Set this tree back to the values inside the newly modified tree copy
    }
    else {
        this->root = NULL;
    }
 
}

void BSTSet::differenceRec(TNode* node_target) {

    // Traverse through the input tree using pre-order traversal. If both tree contain an element then remove it from this tree 
    if (node_target != NULL) { 
        if (this->isIn(node_target->element)) {
            this->remove(node_target->element);
        }
        else {
        }
        this->differenceRec(node_target->left);
        this->differenceRec(node_target->right);
    }
}


void BSTSet::difference(const BSTSet& s)
{
    if (s.root != NULL) { // If the tree is not empty call the recursive difference function
        this->differenceRec(s.root);
    }
}


int BSTSet::sizeRec(TNode* node_target) {

    // A tree has of n nodes has n-1 edges
    if (node_target == NULL) {
        return 0;
    }
    else {
        // Recursively travel the tree and count each set of children for every parent
        return sizeRec(node_target->left) + sizeRec(node_target->right) + 1;
    }
}


int BSTSet::size()
{
    // Call the recursive size function
    int size_tree = this->sizeRec(this->root);
    return size_tree; // Return the size of the tree
}

int BSTSet::heightRec(TNode* node_target) {

    // Height of a binary tree = number of layers - 1
    if (node_target == NULL) {
        return 0;
    }
    int left_traverse = this->heightRec(node_target->left);
    int right_traverse = this->heightRec(node_target->right);
    // Traverse the tree using post-order traversal. Calculate max depth of each parent node using recursion. 
    // max() returns the larger of the two values
    // if both are equal, then it returns the first one
    return max(left_traverse, right_traverse) + 1; 
}


int BSTSet::height()
{
    int height = -1;
    if (this->root == NULL) {
        return height; // If the tree is empty return a height of -1
    }
    else {
        height = this->heightRec(this->root);
    }
    return height-1; 
}

// create and use class MyStack
void BSTSet::printNonRec()
{
    // If this tree is empty return nothing
    if (this->root == NULL) {
        return;
    }

    // Create an empty arraystack
    ArrayStack<TNode*> node_stack;

    TNode* current_node = this->root;

    // While we aren't at the end of a branch and the stack isn't empty
    while (current_node != NULL || node_stack.isEmpty() == false) {

        // Push the nodes into the stack using in-order traversal
        while(current_node!=NULL){
            node_stack.push(current_node);
            current_node = current_node->left;
        }

        // If we have reached the very end of the left branch pop the last element and print it. Then move up one stack call and move to the right
        TNode* popped_item = node_stack.pop();
        cout << popped_item->element << ",";
        current_node = popped_item->right;
    }
    return;
}

// Do not modify following provided methods

void BSTSet::printBSTSet()
{
    if (root == NULL)
        std::cout << "";
    else {
        printBSTSet(root);
    }
}

void BSTSet::printBSTSet(TNode* t)
{
    if (t != NULL) {
        printBSTSet(t->left);
        std::cout << t->element << ",";
        printBSTSet(t->right);
    }
}
