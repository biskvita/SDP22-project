#pragma once

#include "event.h"

/////////////////////////////

class Worker
{
private:    
    ResourceType resource;
    int departure;
    int OriginallyWanted;  
public:
    Worker() = default; 
    Worker(const int& dep, const ResourceType& res, const int& wanted); 

    ResourceType getResource() const;
    int getDeparture() const;
    int getOriginallyWanted() const;  
};