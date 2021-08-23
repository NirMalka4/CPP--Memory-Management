//
// Created by spl211 on 06/11/2020.
//

#include "Session.h"
#include "Agent.h"
#include "Graph.h"
#include <vector>
#include <fstream>
#include "json.hpp"
#include "string"


using json=nlohmann::json;

//Constructor
Session :: Session(const std::string& path)
    :g(std::vector<std::vector<int>>()),treeType(), agents(std::vector<Agent*>()),Q(std::vector<int>()),currCycle(0)
{
    //Initialize json parser
    std :: ifstream jsonRead(path);
    json jsonParser=json::parse(jsonRead);
    //Initialize graph g
    Graph g1(jsonParser["graph"]);
    setGraph(g1);
    //Initialize treeType
    treeType=setTreeType(jsonParser["tree"]);
    //Initialize agents
    std::vector<std::pair<std::string,int>> agentToCopy=jsonParser["agents"];
    for(const std::pair<std::string ,int>& p: agentToCopy) {
        Agent* addMe= Agent::createAgent(p.first,p.second);
        addAgent(*addMe);
        delete addMe;
        addMe= nullptr;
        //Set carried vertices
        if(p.first=="V")
            g.setCarrier(p.second);
    }//for
}

//Copy Constructor
Session ::Session(const Session &other)
    :g(other.g),treeType(other.treeType),agents(std::vector<Agent*>()),Q(other.Q),currCycle(other.currCycle){
    copyAgents(other);
}

//Destructor
Session::~Session() {
    clear();
}

//Move Constructor
Session::Session(Session &&other) noexcept :
    g(other.g),treeType(other.treeType),agents(std::vector<Agent*>()),Q(std::move(other.Q)),currCycle(other.currCycle){
    if(this!=&other)
        steal(other);
}

//Copy Assignment operator
 Session& Session:: operator= (const Session& other){
    if(this!= & other){
        clear();
        copy(other);
        copyAgents(other);
    }
    return *this;
}

//Move Assignment Operator
 Session& Session:: operator=(Session&& other) noexcept {
    if(this!=&other) {
        clear();
        copy(other);
        steal(other);
    }
    return *this;
}

//Conduct loop until the termination conditions are satisfied
void Session :: simulate() {
    //dismiss=true iff termination conditions has been satisfied
    bool dismiss = false;
    //Counter active agents in each cycle.
    int cycleLength = agents.size();
    while (!dismiss) {
        for(int i=0; i<cycleLength; i++)
            agents[i]->act(*this);
        //A new cycle is being initialized
        dismiss = g.isSatisfied();
        cycleLength = agents.size(); // set active agents for following cycle
        currCycle++;
    }//while
    //Construct output file.
    nlohmann::json j;
    j["graph"]=g.getEdges();
    j["infected"]= g.getInfectedNodes();
    std::ofstream o("output.json");
    o << j << std:: endl;
}

void Session::addAgent(const Agent &agent) {
    agents.push_back(agent.clone());
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

void Session::enqueueInfected(int vertex) {
    Q.push_back(vertex);
}

int Session::dequeueInfected() {
    int ans = Q.front();
    Q.erase(Q.begin());
    return ans;
}

TreeType Session::getTreeType() const {
    return treeType;
}

Graph &Session::getGraphReference() {
    return g;
}

//Return whether there is infected vertex in queue Q.
bool Session::containInfectedNodes() {
    return Q.empty();
}

//Return occurring cycle number
int Session::getCurrCycle() const {
    return currCycle;
}

// Utility function to set the treeType while initialize session's instance
TreeType Session::setTreeType(const std::string &s) {
    if (s == "M")
        return MaxRank;
    else if (s == "C")
        return Cycle;
    else
        return Root;
}

//Utility function to delete pointers to Agents Instances
void Session::clear() {
    for(Agent* agent : agents) {
        delete agent;
        agent=nullptr;
    }
    agents.clear();
    g.clear();
    Q.clear();
    currCycle=0;
}

//Utility function to copy other's agents.
void Session::copyAgents(const Session &other) {
    for (Agent* a : other.agents)
        agents.push_back(a->clone());
}

//Utility function to copy other session's data members,exclude agents.
void Session::copy(const Session &other) {
    g=other.g;
    treeType=other.treeType;
    Q=other.Q;
    currCycle=other.currCycle;
}

//Steal other agents allocations.
void Session::steal(Session &other) {
    for(Agent* agent: other.agents){
        agents.push_back(agent);
        agent= nullptr;
    }
    other.agents.clear();
}







