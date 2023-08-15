#pragma once
#include <string>
#include "node.h"

using std::string;

class Hierarchy
{
private:
    string name = "";
    Node* boss = nullptr;
    int employeesCount = 0;
    int level = 0;

    bool checkFile(const string& str);
    bool correctLine(const string& line);
    std::string ignoreSpaces(std::string& str);
    void read(const string& line); 
    void copy(Node*& other, Node*& current);
    Node* getPerson(const string& name, Node* current) const;
    void insert(const string& parent, Node*& current, Node* wanted); 
    int neprekiPodchineni(Node* wanted, Node* current) const;
    void remove(const string& name, Node*& current);
    void remove(const string& name, Node*& current) const; 
    void changeManager(const string& parent, Node*& current, Node*& wanted);
    void changeManager(const string& parent, Node*& current, Node*& wanted) const;
    void search(int level, int& count, Node* current) const; 
    Node* getPersonMaxSalary(Node* current);
    void setHeight(Node*& current, int level, int& max); 
    void setHeight(Node*& current, int level, int& max) const; 
    void getPeopleAtLevel(Node* current, int level, std::vector<Node*>& people); 
    void getPeopleAtLevel2(Node* current, int level, std::vector<Node*>& people) const;
    void incorporater(int& level);
    void modernizer(int& level); 
    void printer(int& level, std::string& str) const; 
    string joiner(Node*& current, Hierarchy& original, Hierarchy& cpy) const;
    void duplicates(Node*& current1, Node*& current2, int& count1, int& count2, Node* boss1, Node* boss2) const;
    void checkJoining(Node*& current, Node*& otherBoss) const;
    void deleteEverything(Node* current);
    
public:
    Hierarchy(Hierarchy&& r) noexcept; 
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data); 
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const; 
    bool find(const string& name) const; 
    int num_employees() const; 
    int num_overloaded(int level = 20) const; 

    string manager(const string& name) const; 
    int num_subordinates(const string& name) const; 
    unsigned long getSalary(const string& who) const; 

    bool fire(const string& who); 
    bool hire(const string& who, const string& boss); 

    void incorporate(); 
    void modernize(); 

    Hierarchy join(const Hierarchy& right) const;

    void save(const string& file); 
    void setName(const string& str);
    string getName() const;
};