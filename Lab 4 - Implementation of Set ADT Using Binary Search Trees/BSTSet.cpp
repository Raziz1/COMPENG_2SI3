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
    return node_target;
}

// Recursive function to remove a node containing the integer 'e'
void BSTSet::removeRec(int e, TNode* &node_target)
{
    if (node_target == NULL)
        return;

    if (e < node_target->element)  // find the node in the tree using binary search
        removeRec(e, node_target->left);
    else if (e > node_target->element)
        removeRec(e, node_target->right);

    // Case 3 - Delete node with 2 children (Method 1)
    else if (node_target->left != NULL && node_target->right != NULL)
    {
        node_target->element = finMin(node_target->right)->element;
        removeRec(node_target->element, node_target->right);
    }

    // Case 1 and 2 both handled here.  Think about how
    else
    {
        TNode* oldNode = node_target;
        node_target = (node_target->left != NULL) ? node_target->left : node_target->right;
        delete oldNode;
    }
}


bool BSTSet::remove(int v)
{
    if (this->root == NULL) {
        return false;
    }
    else if (this->isIn(v) == true) {
        this->removeRec(v, this->root);
        return true;
    }
    else {
        return false;
    }
}

// Recursive Union function to go through all of the nodes if the input function 
void BSTSet::UnionRec(TNode* node_target)
{
    if (node_target != NULL) {
        this->add(node_target->element);
        UnionRec(node_target->left);
        UnionRec(node_target->right);        
    }
}

void BSTSet::Union(const BSTSet& s)
{
    if (s.root != NULL) {
        this->UnionRec(s.root);
    }
    
}

void BSTSet::intersectionRec(TNode* node_target, BSTSet& input_tree, BSTSet& this_tree)
{
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

    if (s.root != NULL && this->root!=NULL ) {
        BSTSet* input_tree = new BSTSet();
        BSTSet* copy_this_tree = new BSTSet();
        input_tree->root = s.root;
        this->intersectionRec(this->root, *input_tree, *copy_this_tree);
        this->root = copy_this_tree->root;
    }
    else {
        this->root = NULL;
    }
 
}

void BSTSet::differenceRec(TNode* node_target) {

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
    if (s.root != NULL) {
        this->differenceRec(s.root);
    }
}


int BSTSet::sizeRec(TNode* node_target) {
    if (node_target == NULL) {
        return 0;
    }
    else {
        return sizeRec(node_target->left) + sizeRec(node_target->right) + 1;
    }
}


int BSTSet::size()
{
    int size_tree = this->sizeRec(this->root);
	// TODO
    return size_tree; //change this after completing this function
}

int BSTSet::heightRec(TNode* node_target) {
    if (node_target == NULL) {
        return 0;
    }
    int left_traverse = this->heightRec(node_target->left);
    int right_traverse = this->heightRec(node_target->right);

    return max(left_traverse, right_traverse) + 1;
}


int BSTSet::height()
{

    int height = -1;
    if (this->root == NULL) {
        return height;
    }
    else {
        height = this->heightRec(this->root);
    }
	// TODO
    return height-1; //change this after completing this function
}

// create and use class MyStack
void BSTSet::printNonRec()
{

    if (this->root == NULL) {
        return;
    }

    ArrayStack<TNode*> node_stack;

    TNode* current_node = this->root;

    while (current_node != NULL || node_stack.isEmpty() == false) {
        while(current_node!=NULL){
            node_stack.push(current_node);
            current_node = current_node->left;
        }

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
