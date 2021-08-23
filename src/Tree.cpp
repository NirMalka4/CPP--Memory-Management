#include "Tree.h"
#include <vector>


//Constructor
Tree::Tree(int rootLabel):node(rootLabel),children(std:: vector<Tree*>()) {}

//Destructor
Tree::~Tree()  {
    node=-1;
    clear();
}

//Copy Constructor
Tree::Tree(const Tree &other): node(other.node),children(std:: vector<Tree*>()) {
    copyTrees(other);
}

//Move Constructor
Tree::Tree(Tree &&other) noexcept : node(other.node),children(std::vector<Tree*>()) {
    if(this!=&other)
        steal(other);
}

//Copy Assignment operator
Tree& Tree::operator=(const Tree &other) {
    if(this!=&other){
        clear();
        node=other.node;
        copyTrees(other);
    }
    return *this;
}

//Move Assignment Operator
Tree& Tree::operator=(Tree &&other)  noexcept {
    if(this!=&other) {
        clear();
        node = other.node;
        steal(other);
    }
    return *this;
}

//Utility function used for allocate Tree pointer according to it's type
Tree* Tree::createTree(const Session& session, int rootLabel) {
    const TreeType t=session.getTreeType();
    if(t==MaxRank)
        return new MaxRankTree(rootLabel);
    else if(t==Cycle)
        return new CycleTree(rootLabel,session.getCurrCycle());
    else
        return new RootTree(rootLabel);
}

//Add subtree to current Tree
void Tree::addChild(const Tree &child) {
    children.push_back(child.clone());
}

//Return Tree root
int Tree::getNode() const {
    return node;
}

//Return number of descendents
int Tree::getNumOfChildren() const {
    return children.size();
}

//Return children vector
const std::vector<Tree*>& Tree::getChildren() const {
    return children;
}

MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {}

//Duplicate instance
Tree* MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

int MaxRankTree::traceTree()
{
    //Conduct level order traversal on the BFS tree
    //Hence either the vertex with most descendants or the shallowest vertex(closest to the root) or the left-most node
    // will be returned.
    int output=node;
    int max_rank= children.size();
    std::queue<Tree*> q;
    q.push(this);
    while(!q.empty()) {
        Tree* parentTree = q.front();
        q.pop();
        for (Tree* treeDescendant: parentTree->getChildren()) {
            if (treeDescendant->getNumOfChildren()>  max_rank) {
                output = treeDescendant->getNode();
                max_rank = treeDescendant->getNumOfChildren();
            }
            q.push(treeDescendant);
        }
    }
    return output;
}


CycleTree ::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel),currCycle(currCycle){}

//Duplicate instance
Tree* CycleTree::clone() const {
    return new CycleTree(*this);
}

int CycleTree::traceTree()
{
    //Observe that the left-most child is the first vertex which is being approached from the dequeued vertex
    //Hence, as long as counter>0, we descending the BFS tree's levels by picking the first descendant of each vertex;
    int counter=currCycle;
    int output=node;
    std::queue<Tree*> q;
    q.push(this);
    while((counter>0) & !q.empty()) {
        Tree* parentTree = q.front();
        q.pop();
        for (Tree *treeDescendant: parentTree->getChildren()) {
            output=treeDescendant->getNode();
            q.push(treeDescendant);
            break;
        }//for
        counter--;
    }//while
    return output;
}

RootTree::RootTree(int rootLabel) : Tree(rootLabel){}

//Duplicate instance
Tree* RootTree::clone() const {
    return new RootTree(*this);
}

int RootTree::traceTree() {
    return node;
}

//Utility function to delete Tree's pointers allocations on the heap.
void Tree::clear() {
    for(Tree* tree : children){
        delete tree;
        tree= nullptr;
    }
    children.clear();
}

//Utility function to copy Tree's from other instance.
void Tree::copyTrees(const Tree& other) {
    for(Tree* t: other.children)
        children.push_back(t->clone());
}

//Steal other instance children
void Tree::steal(Tree &other) {
    for(Tree* child: other.children){
        children.push_back(child);
        child= nullptr;
    }
    other.children.clear();
}

















