//
// Created by spl211 on 07/11/2020.
//

#include "Session.h"
#include "Graph.h"
#include "Tree.h"
#include <vector>
#include "string"
#include "Agent.h"


Agent ::Agent()= default;

// Utility function to create instance of Agent
Agent* Agent::createAgent(const std::string& s,int i)
{
    if(s=="V")
        return new Virus(i);
    else
        return new ContactTracer();
}

ContactTracer ::ContactTracer() = default;

void ContactTracer :: act(Session &session) {
    if(!session.containInfectedNodes()) {
        Tree* t=Tree::createTree(session,session.dequeueInfected());
        session.getGraphReference().BFS(session,t);
        int v = t->traceTree();
        session.getGraphReference().isolateNode(v);
        delete t;
        t= nullptr;
        }//if
    }

// Utility function to duplicate specific Agent's type.
Agent* ContactTracer::clone() const{
    return new ContactTracer(*this);
}


Virus :: Virus(int nodeInd): nodeInd(nodeInd){}

void Virus::act(Session &session)
{
    Graph& g= session.getGraphReference();
    //Infect the node it occupies, thus update status from Carrier to Infected, insert it(node) to infected nodes queue.
    if(g.isCarried(nodeInd)) {
        g.infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    //Spread to virus-free node.
    for(int v : g.getNeighbors(nodeInd)) {
        if (g.isHealthy(v)) {
            g.setCarrier(v);
            Virus* addMe= new Virus(v);
            session.addAgent(*addMe);
            delete addMe;
            addMe= nullptr;
            break;
        }
    }
}

// Utility function to duplicate specific Agent's type.
Agent* Virus ::clone() const {
    return new Virus(*this);
}










