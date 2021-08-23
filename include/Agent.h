#ifndef AGENT_H_
#define AGENT_H_

#include "Session.h"
#include "Graph.h"
#include "Tree.h"
#include <vector>

class Agent{
public:
    Agent();

    // Utility function to create instance of Agent
    static  Agent* createAgent(const std::string&,int);
    virtual void act(Session& session)=0;
    // Utility function to duplicate specific Agent's type.
    virtual Agent* clone() const =0;
    //Default Destructor
    virtual ~Agent(){};
};

class ContactTracer: public Agent{
public:
    ContactTracer();

    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual ~ContactTracer(){};
};


class Virus: public Agent{
public:
    Virus(int nodeInd);

    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual ~Virus(){};
private:
    const int nodeInd;
};

#endif
