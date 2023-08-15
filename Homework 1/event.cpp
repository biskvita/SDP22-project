#include "event.h"

void Event::onWorkerSend(int minute, ResourceType resource) 
{
    if (resource == ResourceType::banana)
    {
        cout << "W "<< minute << " banana" << endl;
    }
    else
    {
        cout << "W "<< minute << " schweppes" << endl;
    }
}

void Event::onWorkerBack(int minute, ResourceType resource) 
{
    if (resource == ResourceType::banana)
    {
        cout << "D "<< minute << " banana" << endl;
    }
    else
    {
        cout << "D "<< minute << " schweppes" << endl;
    }  
}

void Event::onClientDepart(int index, int minute, int banana, int schweppes)
{
    cout << index << " " << minute << " " << banana << " " << schweppes << endl;
}