#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "Session.h"
#include "Graph.h"
#include "Agent.h"
#include <queue>


class Session;

class Tree{
public:
    explicit Tree(int rootLabel); // Constructor
    Tree(const Tree& other); // Copy CRT
    virtual~ Tree();// Destructor
    Tree(Tree&& other) noexcept ; // Move Constructor
    Tree& operator=(const Tree& other);// Copy Assignment operator
    Tree& operator= (Tree&& other) noexcept ;// Move Assignment operator
    void addChild(const Tree& child);

    //Utility function used for allocate Tree pointer according to it's type
    static Tree* createTree(const Session&,int);
    //Define how the contact tracer affect the graph.
    virtual int traceTree()=0;
    // Utility function to duplicate Tree instance.
    virtual Tree* clone() const=0 ;
    //Return Tree root
    int getNode() const;
    //Return number of descendents
    int getNumOfChildren() const;
    //Return children vector
    const std::vector<Tree*>& getChildren() const;

protected:
    int node;
    std::vector<Tree*> children;

private:
    //Utility function to delete Tree's pointers allocations on the heap.
    void clear();
    //Utility function to copy Tree's from other instance.
    void copyTrees(const Tree& other);
    //Steal other instance children
    void steal(Tree& other);

};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    int traceTree() override;
    virtual Tree* clone() const;

private:
    int currCycle;
};

class MaxRankTree: public Tree {
public:
    explicit MaxRankTree(int rootLabel);

    int traceTree() override;
    virtual Tree *clone() const;
};

class RootTree: public Tree{
public:
    explicit RootTree(int rootLabel);
    int traceTree() override;
    virtual Tree* clone() const;


};

#endif
