#pragma once

#include "queue.cpp" 
#include "worker.h"

const int RESTOCK_TIME = 60;
const int RESTOCK_AMOUNT = 100;
 
class MyStore : public Store 
{
private:	
    ActionHandler* actionHandler = nullptr;
    std::vector<Client> clients; 
    int bananas = 0;
    int schweppes = 0; 
    int workerCount = 0;
    int clientsCount = 0;  

    inline void sendWorker(int i, ResourceType type, int& workerCount, Queue<Worker>& workers, int wanted);
    void twoWorkers(Client temp, Queue<Worker>& workers, int& WorkerCount, int& expectedBanana, int& expectedSchweppes, int i);
    void sending(Client temp, Queue<Worker>& workers, int& WorkerCount, int& expectedWorkerBanana, int& expectedWorkerSchweppes, 
        bool& bananaDelivery, bool& schweppesDelivery, int expectedBanana, int expectedSchweppes, int i);
    void buying(Queue<Client>& clientsWaitingAtStore, std::vector<int>& indexes, int i);   
public: 
    MyStore(); 

	void setActionHandler(ActionHandler* handler) override;
	void init(int workerCount, int startBanana, int startSchweppes) override;
	void addClients(const Client* clients, int count) override;
	void advanceTo(int minute) override;

	int getBanana() const override;
	int getSchweppes() const override;

    ~MyStore();
};