#ifndef GRAPH_H_
#define GRAPH_H_

#include "Session.h"
#include "Agent.h"
#include "Tree.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Session;
class Tree;

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);//Constructor
    ~Graph(); // Destructor
    Graph (const Graph &other); // Copy Constructor
    Graph(Graph&& other) noexcept ; // Move Constructor
    Graph& operator=(const Graph& other);// Copy Assignment operator
    Graph& operator= (Graph&& other) noexcept ;// Move Assignment operator

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    //Remove all edges incident to vertex v
    void isolateNode(int v);
    //Return vertex v neighbors.
    std:: vector<int> getNeighbors(int v);
    //Return whether each connected component is either fully infected or healthy.
    bool isSatisfied();
    //Utility function, conduct BFS scan
    void BFS(const Session&,Tree*);
    //Return const reference of the infected vector.
    const std:: vector<int> &getInfectedNodes() const;
    //Mark vertex as carrier
    void setCarrier(int);
    //Return whether the vertex is a carrier.
    bool isCarried(int);
    //Return whether a vertex is healthy
    bool isHealthy(int);
    //Return const reference of the adjacency matrix
    const std::vector<std::vector<int>>& getEdges() const;
    //Utility function to clear data members allocations.
    void clear();

private:
    int V;
    std::vector<std::vector<int>> edges;
    //Enumerate infected vertices at the graph.
    std:: vector<int> infected;
    //Mark healthy vertex Vi with key 1, carrier with key 2, infected with key 3.
    std:: vector<int> status;
    //Container of the infected vertices at the graph.
    std::unordered_map<int,int> infectedComponent;
    //Container of the healthy vertices at the graph.
    std::unordered_map<int,int> healthyComponent;
    //Container of the carried vertices at the graph.
    std::unordered_map<int,int> carriedComponent;
    //Utility function to copy other graph data members
    void copy(const Graph& other);
    //Utility function to steal other instance's resources
    void steal(Graph& other);


};

#endif
