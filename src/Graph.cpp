//
// Created by spl211 on 03/11/2020.
//
#include "Session.h"
#include "Graph.h"
#include "Tree.h"
#include <utility>
#include <vector>
#include <queue>
#include <unordered_map>

//Upon initializing, all vertices are assigned to the healthy component.
Graph::Graph(std::vector<std::vector<int>> matrix)
    :V(matrix.size()),edges(std::move(matrix)),infected(std::vector<int>()),status(std::vector<int>(V,1))
    ,infectedComponent(std::unordered_map<int,int>()),healthyComponent(std::unordered_map<int,int>()),carriedComponent(std::unordered_map<int,int>()){
    for(int i=0; i<V; i++){
        healthyComponent.insert(std::make_pair(i,i));
    }
}
Graph::~Graph(){
    clear();
}

//Copy Constructor
Graph::Graph(const Graph &other)
    :V(other.edges.size()),edges(other.edges), infected(other.infected), status(other.status),
    infectedComponent(other.infectedComponent),healthyComponent(other.healthyComponent),carriedComponent(other.carriedComponent){}

//Copy Assignment Operator
Graph& Graph::operator=(const Graph &other) {
    if(this!=&other) {
        clear();
        copy(other);
    }
    return *this;
}
//Move Constructor
Graph::Graph(Graph&& other) noexcept
    : V(other.edges.size()),edges(std::move(other.edges)),infected(std::move(other.infected)),status(std::move(other.status))
    ,infectedComponent(std::move(other.infectedComponent)), healthyComponent(std::move(other.healthyComponent))
    ,carriedComponent(std::move(other.carriedComponent)){
    other.clear();
}

//Move Assignment operator
Graph& Graph::operator=(Graph &&other) noexcept {
    clear();
    steal(other);
    other.clear();
    return *this;
}

void Graph::infectNode(int nodeInd){
    if(!isInfected(nodeInd)) {
        // Update vertex v status to infected(status[v]=3)
        status[nodeInd]= 3;
        //Move vertex nodeInd from the carried component to the infected component.
        //Therefore, nodeInd shall be added to the infected nodes vector.
        carriedComponent.erase(nodeInd);
        infectedComponent.insert(std::make_pair(nodeInd,nodeInd));
        infected.push_back((nodeInd));
    }
}

bool Graph ::isInfected(int nodeInd){
    return status[nodeInd]==3;
}
//Set vertex as carrier
void Graph::setCarrier(int v) {
    if(!isCarried(v)) {
        // Update vertex v status to carrier(status[v]=2)
        status[v] = 2;
        //Move vertex v from the healthy component to the carried component.
        healthyComponent.erase(v);
        carriedComponent.insert(std::make_pair(v,v));
    }
}
//Return whether vertex v is carried
bool Graph:: isCarried(int v){
   return status[v]==2;
}
//Return whether vertex v is healthy
bool Graph::isHealthy(int v) {
    return status[v]==1;
}
//Isolate vertex via remove all edges incident to it.
void Graph::isolateNode(int v) {
    for(int u : getNeighbors(v)) {
        edges[v][u]=0;
        edges[u][v] = 0;
    }
}

//Return neighbors of vertex v sorted in ascending order
std:: vector<int> Graph::getNeighbors(int w) {
    std:: vector<int> neighbors;
    for(int i=0; i<V ;i++) {
        if (edges[w][i]==1)
            neighbors.push_back(i);
    }
    return neighbors;
}
//Conduct BFS and set the BFS tree simultaneously.
void Graph::BFS(const Session& session, Tree* root) {

        //Vertex Vi has been approached iff visited[i]==false
        std:: vector<bool> visited(V,false);
        std::queue<Tree*> q;

        q.push(root);
        visited[root->getNode()] = true;
        int childIndex=0; //Point to the last discovered child of parent v during the BFS scan.

        while (!q.empty()) {

            Tree* treeParent = q.front();
            q.pop();
            childIndex=0;

            // Discover all unvisited vertices adjacent of v ,mark them visited and set them as v children.
            for (int i: getNeighbors(treeParent->getNode())) {
                if (!visited[i]) {
                    Tree* child=Tree::createTree(session,i);
                    treeParent->addChild(*child);
                    q.push(treeParent->getChildren()[childIndex]);
                    childIndex++;
                    delete child;
                    child= nullptr;
                    visited[i] = true;
                }//if
            }//for
        }//while
}
//Return whether termination condition has been satisfied.
bool Graph ::isSatisfied() {
    //Observe that if exists carried vertex at the graph, termination conditions has not been satisfied
    return carriedComponent.empty();
}
//Return infected nodes.
const std::vector<int>& Graph::getInfectedNodes() const {
    return infected;
}
//Return adjacency matrix.
const std::vector<std::vector<int>>& Graph::getEdges() const {
    return edges;
}

//Utility function to copy other graph data members
void Graph::copy(const Graph &other) {
    V=other.V;
    edges = other.edges;
    infected = other.infected;
    status=other.status;
    infectedComponent=other.infectedComponent;
    healthyComponent=other.healthyComponent;
    carriedComponent=other.carriedComponent;
}
//Utility function to steal other instance's resources
void Graph::steal(Graph& other) {
    V=other.V;
    edges=std::move(other.edges);
    infected=std::move(other.infected);
    status= std:: move(other.status);
    infectedComponent=std::move(other.infectedComponent);
    healthyComponent= std::move(other.healthyComponent);
    carriedComponent= std::move(other.carriedComponent);
}
//Utility function to clear data members allocations.
void Graph::clear() {
    V=-1;
    edges.clear();
    infected.clear();
    status.clear();
    infectedComponent.clear();
    healthyComponent.clear();
    carriedComponent.clear();
}






