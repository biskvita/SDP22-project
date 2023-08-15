#pragma once

#include "interface.h" 
 
class Command
{
private:
    std::vector<string> storage; 
    std::vector<Hierarchy*> hierarchies; 

    bool checkName(std::string str); // Function for validating name 
    void turnInLowerCase(string& str); // Function for turning input into lower case

public:
    Command() = default; 
    Command(const Command& other) = delete; 
    Command& operator=(const Command& other) = delete;

    void getArguments(const string& input); 
    void execute(); // Function for interface

    ~Command(); 
};