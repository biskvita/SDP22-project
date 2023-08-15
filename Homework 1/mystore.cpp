#include "mystore.h"

inline void MyStore::sendWorker(int i, ResourceType type, int& workerCount, Queue<Worker>& workers, int wanted)
{
    Worker worker(i, type, wanted); //create Worker
    actionHandler->onWorkerSend(worker.getDeparture(), worker.getResource()); //call actionHandler
    workers.push(worker); //add him to the queue
    --workerCount;  
}

MyStore::MyStore()
{
    this->setActionHandler(new Event()); 
}

void MyStore::setActionHandler(ActionHandler* handler) 
{
    actionHandler = handler;
}

void MyStore::init(int workerCount, int startBanana, int startSchweppes) 
{
    this->bananas = startBanana;
    this->workerCount = workerCount;
    this->schweppes = startSchweppes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//function that calculates what to do if a client wants both items
void MyStore::twoWorkers(Client temp, Queue<Worker>& workers, int& WorkerCount, int& expectedBanana, int& expectedSchweppes, int i)
{
    do
    {
        if (workerCount == 1 && temp.banana >= temp.schweppes) //if you have 1 worker
        {
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
            expectedBanana += RESTOCK_AMOUNT;
            break;
        }
        else if (workerCount == 1 && temp.schweppes > temp.banana) //if you have 1 worker 
        {
            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
            expectedSchweppes += RESTOCK_AMOUNT;
            break;
        }

        if (temp.banana > expectedBanana && temp.schweppes <= expectedSchweppes) //if only bananas are needed
        { 
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
            expectedBanana += RESTOCK_AMOUNT;

            if (temp.banana <= expectedBanana && temp.schweppes <= expectedSchweppes)
            {
                break; 
            }
        }
        else if (temp.schweppes > expectedSchweppes && temp.banana <= expectedBanana) //if only schweppes is needed
        {
            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
            expectedSchweppes += RESTOCK_AMOUNT;

            if (temp.banana <= expectedBanana && temp.schweppes <= expectedSchweppes)
            {
                break; 
            }
        }
        else if (temp.banana >= temp.schweppes && temp.banana > expectedBanana) //if only bananas are needed
        { 
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
            expectedBanana += RESTOCK_AMOUNT;
        }
        else if (temp.schweppes > temp.banana && temp.schweppes > expectedSchweppes) //if only bananas are needed
        { 
            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
            expectedSchweppes += RESTOCK_AMOUNT;
        }
        else //else send for both items workers 
        {
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana); 
            expectedBanana += RESTOCK_AMOUNT;

            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes); 
            expectedSchweppes += RESTOCK_AMOUNT;
        }
        
        if (workerCount < 1)
        {
            break; 
        }

    } while (temp.banana >= expectedBanana && temp.schweppes >= expectedSchweppes);
}

//main function to configure the onWorkerSend event
void MyStore::sending(Client temp, Queue<Worker>& workers, int& WorkerCount, int& expectedWorkerBanana, int& expectedWorkerSchweppes, 
    bool& bananaDelivery, bool& schweppesDelivery, int expectedBanana, int expectedSchweppes, int i)
{
    int countBananaDel = 0;
    int countSchweppesDel = 0; 
    int size = workers.size(); 
    
    expectedBanana = this->bananas + RESTOCK_AMOUNT;
    expectedSchweppes = this->schweppes + RESTOCK_AMOUNT;
    
    for (size_t i = 0; i < size; i++)
    {
        if (!workers.empty() && workers[i].getResource() == ResourceType::banana) //if there is currently a banana delivery
        {
            expectedWorkerBanana = workers[i].getDeparture() + RESTOCK_TIME; //get return time
            bananaDelivery = true; 

            for (size_t j = i+1; j < size; j++) //if there are multiple identical banana deliveries
            {
                if (size >= 2 && workers[j].getResource() == ResourceType::banana &&
                workers[i].getDeparture() == workers[j].getDeparture() && workers[i].getOriginallyWanted() == workers[j].getOriginallyWanted())
                {
                    ++countBananaDel;
                } 
            }
           
        }

        if (!workers.empty() && workers[i].getResource() == ResourceType::schweppes) //if there is currently a schweppes delivery
        {
            expectedWorkerSchweppes = workers[i].getDeparture() + RESTOCK_TIME; //get return time
            schweppesDelivery = true; 
            
            for (size_t j = i+1; j < size; j++) //if there are multiple identical banana deliveries
            {
                if (size >= 2 && workers[j].getResource() == ResourceType::schweppes &&
                workers[i].getDeparture() == workers[j].getDeparture() && workers[i].getOriginallyWanted() == workers[j].getOriginallyWanted())
                {
                    ++countSchweppesDel;
                }
            }
        }
    }

    //Calculate expected amount of bananas and schweppes for multiple identical deliveries
    if (countBananaDel > 1)
    {
        expectedBanana += (countBananaDel-1)*RESTOCK_AMOUNT;
    }
    else if (countBananaDel == 1)
    {
        expectedBanana += RESTOCK_AMOUNT; 
    }

    if (countSchweppesDel > 1)
    {
        expectedSchweppes += (countSchweppesDel-1)*RESTOCK_AMOUNT; 
    }
    else if (countSchweppesDel == 1)
    {
        expectedSchweppes += RESTOCK_AMOUNT; 
    }
    
    //If there are no deliveries at the time 
    if (workers.empty())
    {
        expectedBanana = this->bananas;
        expectedSchweppes = this->schweppes;

        if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount >= 2) //if both items are wanted
        {
            twoWorkers(temp, workers, WorkerCount, expectedBanana, expectedSchweppes, i);  
        }
        else if (temp.banana > this->bananas && workerCount >= 1 && temp.schweppes <= this->schweppes) //if bananas are wanted
        {
            do
            {
                sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
                expectedBanana += RESTOCK_AMOUNT; 

            } while (temp.banana > expectedBanana && workerCount >= 1);
        }
        else if (temp.schweppes > this->schweppes && workerCount >= 1 && temp.banana <= this->bananas) //if schweppes is wanted
        {
            do
            {
                sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
                expectedSchweppes += RESTOCK_AMOUNT; 

            } while (temp.schweppes > expectedSchweppes && workerCount >= 1);   
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount == 1 && temp.banana >= temp.schweppes) //if there is 1 worker
        {  
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount == 1 && temp.schweppes > temp.banana) //if there is 1 worker
        {
            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
        }
    }
    else //If workers are sent 
    {
        int toBeDelBanana = this->bananas;
        int toBeDelSchweppes = this->schweppes;

        //If both items are wanted
        if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount >= 2
            && (!schweppesDelivery || (schweppesDelivery && (expectedWorkerSchweppes > (temp.arriveMinute + temp.maxWaitTime) || temp.schweppes > expectedSchweppes)))
            && (!bananaDelivery || (bananaDelivery && (expectedWorkerBanana > (temp.arriveMinute + temp.maxWaitTime) || temp.banana > expectedBanana)))) 
        {   
            twoWorkers(temp, workers,  workerCount, toBeDelBanana, toBeDelSchweppes, i);
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount >= 1
            && (!schweppesDelivery || (schweppesDelivery && (expectedWorkerSchweppes > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.schweppes > expectedSchweppes))))
        {
            do //if schweppes is wanted
            {
                sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
                toBeDelSchweppes += RESTOCK_AMOUNT; 

            } while (temp.schweppes > toBeDelSchweppes && workerCount >= 1);
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount >= 1
            && (!bananaDelivery || (bananaDelivery && (expectedWorkerBanana > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.banana > expectedBanana)))) 
        {
            do //if bananas are wanted
            {
                sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
                toBeDelBanana += RESTOCK_AMOUNT; 

            } while (temp.banana > toBeDelBanana && workerCount >= 1);
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount == 1 && temp.banana >= temp.schweppes 
            && (!bananaDelivery || (bananaDelivery && (expectedWorkerBanana > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.banana > expectedBanana)))) //if you have 1 worker
        {  
            sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
        }
        else if (temp.banana > this->bananas && temp.schweppes > this->schweppes && workerCount == 1 && temp.schweppes > temp.banana
            && (!schweppesDelivery || (schweppesDelivery && (expectedWorkerSchweppes > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.schweppes > expectedSchweppes)))) //if you have 1 worker
        {
            sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes); 
        }
        else if (temp.banana > this->bananas && workerCount >= 1 && temp.schweppes <= this->schweppes 
            && (!bananaDelivery || (bananaDelivery && (expectedWorkerBanana > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.banana > expectedBanana)))) 
        {
            do //if you have the schweppes, but not the bananas
            {
                sendWorker(i, ResourceType::banana, workerCount, workers, temp.banana);
                toBeDelBanana += RESTOCK_AMOUNT; 

            } while (temp.banana > toBeDelBanana && workerCount >= 1);
        }
        else if (temp.schweppes > this->schweppes && workerCount >= 1 && temp.banana <= this->bananas 
            && (!schweppesDelivery || (schweppesDelivery && (expectedWorkerSchweppes > (temp.arriveMinute + temp.maxWaitTime) ||
                temp.schweppes > expectedSchweppes))))
        {
            do //if you have the bananas, but not the schweppes
            { 
                sendWorker(i, ResourceType::schweppes, workerCount, workers, temp.schweppes);
                toBeDelSchweppes += RESTOCK_AMOUNT; 

            } while (temp.schweppes > toBeDelSchweppes && workerCount >= 1); 
        }
    }
}

void MyStore::buying(Queue<Client>& clientsWaitingAtStore, std::vector<int>& indexes, int i)
{
    //If the first in the queue acquires the wanted products before maxWaitTime 
    if (i >= clientsWaitingAtStore.front().arriveMinute && i <= (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) && 
        clientsWaitingAtStore.front().banana <= this->bananas && clientsWaitingAtStore.front().schweppes <= this->schweppes)
    {
        actionHandler->onClientDepart(indexes.front(), i, clientsWaitingAtStore.front().banana, clientsWaitingAtStore.front().schweppes);  
        this->bananas -= clientsWaitingAtStore.front().banana;
        this->schweppes -= clientsWaitingAtStore.front().schweppes; 
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin());  
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&  
        clientsWaitingAtStore.front().banana <= this->bananas && clientsWaitingAtStore.front().schweppes <= this->schweppes)
    {
        //If maxWaitTime expires and the store has the wanted bananas and schweppes
        actionHandler->onClientDepart(indexes.front(), i, clientsWaitingAtStore.front().banana, clientsWaitingAtStore.front().schweppes);  
        this->bananas -= clientsWaitingAtStore.front().banana;
        this->schweppes -= clientsWaitingAtStore.front().schweppes;
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin());  
    }    
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        clientsWaitingAtStore.front().banana <= this->bananas && clientsWaitingAtStore.front().schweppes > this->schweppes)
    {
        //If maxWaitTime expires and the store has the wanted bananas
        actionHandler->onClientDepart(indexes.front(), i, clientsWaitingAtStore.front().banana, this->schweppes);  
        this->bananas -= clientsWaitingAtStore.front().banana;
        this->schweppes = 0;
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin());  
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        clientsWaitingAtStore.front().banana > this->bananas && clientsWaitingAtStore.front().schweppes <= this->schweppes)
    {
        //If maxWaitTime expires and the store has the wanted schweppes
        actionHandler->onClientDepart(indexes.front(), i, this->bananas, clientsWaitingAtStore.front().schweppes);  
        this->bananas = 0;
        this->schweppes -= clientsWaitingAtStore.front().schweppes;
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin()); 
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        clientsWaitingAtStore.front().banana <= this->bananas && clientsWaitingAtStore.front().schweppes == 0)
    {
        //If maxWaitTime expires and person wants only bananas
        actionHandler->onClientDepart(indexes.front(), i, clientsWaitingAtStore.front().banana, 0);  
        this->bananas -= clientsWaitingAtStore.front().banana;
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin()); 
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        clientsWaitingAtStore.front().banana == 0 && clientsWaitingAtStore.front().schweppes <= this->schweppes)
    {
        //If maxWaitTime expires and person wants only schweppes
        actionHandler->onClientDepart(indexes.front(), i, 0, clientsWaitingAtStore.front().schweppes);  
        this->schweppes -= clientsWaitingAtStore.front().schweppes;
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin()); 
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        this->bananas == 0 && this->schweppes == 0)
    {
        //If there are 0 bananas and schweppes at the store
        actionHandler->onClientDepart(indexes.front(), i, 0, 0);  
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin()); 
    }
    else if (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) &&
        clientsWaitingAtStore.front().banana > this->bananas && clientsWaitingAtStore.front().schweppes > this->schweppes)
    {
        //If there is not enough of both 
        actionHandler->onClientDepart(indexes.front(), i, this->bananas, this->schweppes);
        this->bananas = 0;
        this->schweppes = 0;   
        clientsWaitingAtStore.pop();
        indexes.erase(indexes.begin()); 
    }
}

void MyStore::addClients(const Client* clients, int count) 
{
    assert(count >= 0); 
    int size = this->clients.size();

    for (size_t i = 0; i < count; i++)
    {
        this->clients.push_back(clients[i]);
    }
    
    this->clientsCount += count; 
}

void MyStore::advanceTo(int minute) 
{
    int clientNum = 0; 
    int i = 0; 

    int expectedBanana = 0;
    int expectedSchweppes = 0;
    int expectedWorkerBanana = 0;
    int expectedWorkerSchweppes = 0;

    bool bananaDelivery;
    bool schweppesDelivery;

    Queue<Client> clientsWaitingAtStore; 
    Queue<Worker> workers;
    Queue<Client> clientsAtTime; //Clients arriving in the same moment
    std::vector<int> indexes; //indexes of clients
    
    while (i <= minute)
    {
        bool arrived = false;

        if (clients.empty() && workers.empty() && clientsWaitingAtStore.empty()) //If nothing else will happen, break 
        {
            break;
        }

        //If there is an arrival 
        if (!clients.empty() && i == clients.front().arriveMinute && !arrived) 
        {
            do
            {
                arrived = true;
                clientsAtTime.push(clients.front());
                indexes.push_back(clientNum);
                ++clientNum;
                clients.erase(clients.begin()); 

            } while (!clients.empty() && i == clients.front().arriveMinute); //There might be many coming at the same moment
        }

        //SENDING WORKERS
        if (!clientsAtTime.empty() && i == clientsAtTime.front().arriveMinute && arrived &&  
            (clientsAtTime.front().banana > this->bananas || clientsAtTime.front().schweppes > this->schweppes))
        {
            do
            {
                clientsWaitingAtStore.push(clientsAtTime.front()); //arrived client goes to the waiting queue
                Client temp = clientsAtTime.front(); 
                clientsAtTime.pop();
                arrived = false;  

                bananaDelivery = false;
                schweppesDelivery = false;
                
                sending(temp, workers, workerCount, expectedWorkerBanana, expectedWorkerSchweppes,bananaDelivery, schweppesDelivery, expectedBanana, expectedSchweppes, i);
            
            } while (!clientsAtTime.empty() && clientsAtTime.front().arriveMinute == i && 
                (clientsAtTime.front().banana > this->bananas || clientsAtTime.front().schweppes > this->schweppes));
        }
        
        //DELIVERY
        if (!workers.empty() && i == workers.front().getDeparture() + RESTOCK_TIME)
        {
            do 
            {
                actionHandler->onWorkerBack(i, workers.front().getResource());

                if (workers.front().getResource() == ResourceType::banana)
                {
                    this->bananas += RESTOCK_AMOUNT;
                }
                else
                {
                    this->schweppes += RESTOCK_AMOUNT; 
                }

                workers.pop();
                ++workerCount;

            } while (!workers.empty() && workers.front().getDeparture() + RESTOCK_TIME == i); //there might be many deliveries at the moment
        }

        //CLIENT DEPART
        //if the first in clientsAtTime have everything they need, they take it and leave
        if (!clientsAtTime.empty() && i == clientsAtTime.front().arriveMinute && arrived && 
            clientsAtTime.front().banana <= this->bananas && clientsAtTime.front().schweppes <= this->schweppes)
        {
            do
            {
                actionHandler->onClientDepart(indexes.at(indexes.size() - clientsAtTime.size()), i, clientsAtTime.front().banana, clientsAtTime.front().schweppes);  
                this->bananas -= clientsAtTime.front().banana;
                this->schweppes -= clientsAtTime.front().schweppes; 
                arrived = false; 
                indexes.erase(indexes.begin() + indexes.size() - clientsAtTime.size());
                clientsAtTime.pop();  

            } while (!clientsAtTime.empty() && clientsAtTime.front().arriveMinute == i && 
                clientsAtTime.front().banana <= this->bananas && clientsAtTime.front().schweppes <= this->schweppes); 
        }

        //CLIENT DEPART
        //for those in the queue 
        if (!clientsWaitingAtStore.empty() && !arrived)
        {
            do
            {
                buying(clientsWaitingAtStore, indexes, i); 

            } while (!clientsWaitingAtStore.empty() && (i == (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute) || 
            (i >= clientsWaitingAtStore.front().arriveMinute && clientsWaitingAtStore.front().banana <= this->bananas && 
            clientsWaitingAtStore.front().schweppes <= this->schweppes &&
            i <= (clientsWaitingAtStore.front().maxWaitTime + clientsWaitingAtStore.front().arriveMinute))));     
        }  

        ++i; 
    }
}

int MyStore::getBanana() const 
{
    return this->bananas;
}

int MyStore::getSchweppes() const 
{
    return this->schweppes;
}

MyStore::~MyStore()
{
} 

Store* createStore() 
{
	return new MyStore();
}