#include "mystore.h"

//How to compile:
//g++ main.cpp mystore.cpp queue.cpp event.cpp worker.cpp
//g++ tests.cpp mystore.cpp event.cpp queue.cpp worker.cpp catch2.cpp

/*
0 10 0 10
45 35 0 30
46 30 20 100
200 10 10 1

W 0 banana //0 banana 0 schweppes
0 10 0 0    //0 banana 0 schweppes
W 46 schweppes //0 banana 0 schweppes
D 60 banana   //100 banana 0 schweppes
1 60 35 0     //65 banana 0 schweppes
D 106 schweppes //65 banana 100 schweppes
2 106 30 20   //35 banana 80 schweppes
3 200 10 10   //25 banana 70 schweppes

Client 0 is between 0 and 10,  but leaves at 10 with 0 0
Client 1 is between 45 and 75, but leaves at 60 with 35 0 (wanted)
Client 2 is between 46 and 146, but leaves at 106 with  30 20 (wanted)
Client 3 is between 200 and 201, but leaves at 200 with 10 10 (wanted)

0 300 0 60
10 100 0 60

0 10 10 20
10 10 0 0

0 200 100 60
10 150 200 60

*/

int main()
{
    int W, C, min;

    do
    {
        cout << "W: ";
        cin >> W;
        cout << "C: ";
        cin >> C;

    } while (C < 0 || W < 0);
    
    Store* store = createStore();
    std::vector<Client> clients;    
    store->init(W, 0, 0);

    cout << "Store initialised..." << endl;
    
    for (size_t i = 0; i < C; i++)
    {
        Client client;

        cin >> client.arriveMinute >> client.banana >> client.schweppes >> client.maxWaitTime ;
        clients.push_back(client);
    }

    store->addClients(clients.data(), clients.size()); 
    cout << "Clients added..." << endl;

    do
    {
        cout << "Pick minutes: " ;
        cin >> min;

    } while (min < 0);

    store->advanceTo(min);
    cout << "Day ended..." << endl; 

    delete store; 

    return 0; 
}