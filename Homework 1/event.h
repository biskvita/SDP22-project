#pragma once

#include "interface.h"
#include <iostream>

using std::cout;
using std::endl; 
using std::cin; 

class Event : public ActionHandler
{
public:    
    void onWorkerSend(int minute, ResourceType resource) override;

    void onWorkerBack(int minute, ResourceType resource) override;

    void onClientDepart(int index, int minute, int banana, int schweppes) override;
};