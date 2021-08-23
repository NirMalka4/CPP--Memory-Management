#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Agent.h"
#include "Graph.h"
#include "Tree.h"
#include "json.hpp"
#include "queue"

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    explicit Session(const std::string& path); //Constructor
    Session(const Session& other); // Copy CRT
    ~Session();// Destructor
    Session(Session&& other) noexcept; // Move Constructor
    Session& operator=(const Session& other);// Copy Assignment operator
    Session& operator= (Session&& other) noexcept;// Move Assignment operator
    
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    // Return reference to the current graph
    Graph& getGraphReference() ;
    //Return cycle number
    int getCurrCycle() const;
    //Return whether there is infected vertex in queue Q.
    bool containInfectedNodes();


private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    // Queue contains infected vertices
    std::vector<int> Q;
    //Counter cycle number
    int currCycle;

    // Utility function to set the treeType while initialize session's instance
    static TreeType setTreeType(const std::string& s);
    //Utility function to delete agent's pointers allocations on the heap.
    void clear();
    //Utility function to copy agent's from other instance.
    void copyAgents(const Session& other);
    //Utility function to copy other session's data members,exclude agents.
    void copy(const Session& other);
    //Steal other agents allocations.
    void steal(Session& other);
};

#endif
