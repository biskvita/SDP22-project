#include <iostream>
#include <vector>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

struct Node
{
    std::string value = nullptr;
    Node* parent = nullptr;
    std::vector<Node*> kids; 
    int height = 0;

    Node(std::string name) : value(name) {}
};