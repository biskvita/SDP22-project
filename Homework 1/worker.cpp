#include "worker.h"

Worker::Worker(const int& dep, const ResourceType& res, const int& wanted) : resource(res), departure(dep), OriginallyWanted(wanted)
{
}

ResourceType Worker::getResource() const
{
    return this->resource;
}
    
int Worker::getDeparture() const
{
    return this->departure;
}

int Worker::getOriginallyWanted() const
{
    return this->OriginallyWanted;
}
