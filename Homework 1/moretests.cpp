#define CATCH_CONFIG_MAIN

#include "interface.h"
#include "catch2.hpp"
#include <iostream>

#include <vector>

const int RESTOCK_TIME = 60;
const int RESTOCK_AMOUNT = 100;

typedef std::vector<Client> ClientList;

struct StoreEvent 
{
	enum Type 
    {
		WorkerSend, WorkerBack, ClientDepart
	};

	Type type;
	int minute;

	struct Worker 
    {
		ResourceType resource;
	} worker;

	struct Client 
    {
		int index;
		int banana;
		int schweppes;
	} client;
};

struct TestStore : ActionHandler 
{
	Store* impl = nullptr;
	std::vector<StoreEvent> log;

	TestStore() : impl(createStore()) 
    {
		impl->setActionHandler(this);
	}

	~TestStore() 
    {
		delete impl;
	}

	TestStore(const TestStore &) = delete;
	TestStore &operator=(const TestStore &) = delete;

	void init(int workerCount, int startBanana, int startSchweppes) 
    {
		impl->init(workerCount, startBanana, startSchweppes);
	}

	void advanceTo(int minute) 
    {
		impl->advanceTo(minute);
	}

	int getBanana() const 
    {
		return impl->getBanana();
	}

	int getSchweppes() const 
    {
		return impl->getSchweppes();
	}

	void addClients(const ClientList &clients) 
    {
		impl->addClients(clients.data(), clients.size());
	}

	void addClients(const Client &single) 
    {
		impl->addClients(&single, 1);
	}

	void onWorkerSend(int minute, ResourceType resource) override 
    {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerSend;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onWorkerBack(int minute, ResourceType resource) override 
    {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerBack;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onClientDepart(int index, int minute, int banana, int schweppes) override 
    {
		StoreEvent ev;
		ev.type = StoreEvent::ClientDepart;
		ev.minute = minute;
		ev.client.banana = banana;
		ev.client.schweppes = schweppes;
		ev.client.index = index;
		log.push_back(ev);
	}
};

#define LastEvent() (store.log.back())

TEST_CASE("Client wanting both items, no workers") 
{
	TestStore store;
	store.init(0, 0, 0);

    SECTION("1") 
    {
		store.addClients(Client{0, 200, 100, 61});
	    store.advanceTo(1);
        REQUIRE(store.log.size() == 0);
	}
    
    SECTION("60") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(60);
        REQUIRE(store.log.size() == 1);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().minute == 60);
		REQUIRE(LastEvent().client.banana == 0);
	 	REQUIRE(LastEvent().client.schweppes == 0);
	}
	
}

TEST_CASE("Client wanting both items, 1 worker") 
{
	TestStore store;
	store.init(1, 0, 0);

    SECTION("1") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(1);
        REQUIRE(store.log.size() == 1);
		REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
	}	
}

TEST_CASE("Client wanting both items, 2 workers") 
{
	TestStore store;
	store.init(2, 0, 0);

    SECTION("1") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(1);
        REQUIRE(store.log.size() == 2);
		REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
	}
    
    SECTION("60") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(60);
        REQUIRE(store.log.size() == 5);
		REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[3].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[4].type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().minute == 60);
		REQUIRE(LastEvent().client.banana == 200);
	 	REQUIRE(LastEvent().client.schweppes == 0);
	}

	SECTION("60") 
    {
		store.addClients(Client{0, 100, 200, 60});
	    store.advanceTo(60);
        REQUIRE(store.log.size() == 5);
		REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[3].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[4].type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().minute == 60);
		REQUIRE(LastEvent().client.banana == 0);
	 	REQUIRE(LastEvent().client.schweppes == 200);
	}

	SECTION("60 with equal banans and schweppes") 
    {
		store.addClients(Client{0, 400, 400, 60});
	    store.advanceTo(60);
        REQUIRE(store.log.size() == 5);
		REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[3].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[4].type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().minute == 60);
		REQUIRE(LastEvent().client.banana == 200);
	 	REQUIRE(LastEvent().client.schweppes == 0);
	}	
}

TEST_CASE("Client wanting both items, 5 workers") 
{
	TestStore store;
	store.init(5, 0, 0);

    SECTION("1") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(1);
        REQUIRE(store.log.size() == 3);
		REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerSend);
	}
    
    SECTION("60") 
    {
		store.addClients(Client{0, 200, 100, 60});
	    store.advanceTo(60);
        REQUIRE(store.log.size() == 7);
	    REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[3].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[4].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[5].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[6].type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().minute == 60);
		REQUIRE(LastEvent().client.banana == 200);
	 	REQUIRE(LastEvent().client.schweppes == 100);
	}	
}

TEST_CASE("Clients wanting both items, 5 workers") 
{
	TestStore store;
	store.init(5, 0, 0);

    SECTION("10") 
    {
		store.addClients(Client{0, 200, 100, 60});
		store.addClients(Client{10, 150, 200, 60});
	    store.advanceTo(10);
        REQUIRE(store.log.size() == 5);
	    REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[3].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[4].type == StoreEvent::WorkerSend);	
	}
    
    SECTION("70") 
    {
		store.addClients(Client{0, 200, 100, 60});
		store.addClients(Client{10, 150, 200, 60});
	    store.advanceTo(70);
        REQUIRE(store.log.size() == 12);
	    REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[2].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[3].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[4].type == StoreEvent::WorkerSend);
		REQUIRE(store.log[5].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[6].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[7].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[8].type == StoreEvent::ClientDepart);	
		REQUIRE(store.log[9].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[10].type == StoreEvent::WorkerBack);
		REQUIRE(store.log[11].type == StoreEvent::ClientDepart);
	}	
}
	