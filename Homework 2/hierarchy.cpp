#include "interface.h"

#include <fstream>
#include <queue>

bool Hierarchy::checkFile(const string& str)
{
    std::string check = ".txt";
    std::string check2 = ".data";

    if (str.substr(str.length()-4, 4) != check && str.substr(str.length()-5, 5) != check2) // Return if it is not the wanted 
    {
        return false; 
    }

    return true; 
}

bool Hierarchy::correctLine(const string& line) //function that checks if a row of a file is valid
{
    size_t size = line.size();
    int countTire = 0;
    int countSpace = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (!isalnum(line[i]) && line[i] != '-' && line[i] != ' ')
        {
            return false; 
        }

        if (line[i] == '-')
        {
            ++countTire;
        }

        if (line[i] == ' ')
        {
            ++countSpace;
        }
    }

    if (countTire != 1)
    {
        return false; 
    }
	
    return true; 
}

std::string Hierarchy::ignoreSpaces(std::string& str) // function that removes unnecessary spaces
{
    size_t size = str.length();

    for (size_t i = 0; i < size; ++i)
    {
        if (str.at(i) == ' ')
        {
            str.erase(str.begin() + i);
            --size;
        }
    }

	return str; 
}

void Hierarchy::read(const string& line) // function that reads a file by rows and stores the data into the tree
{
    std::queue<Node*> row; 

    if (correctLine(line))
    {
        int pos = line.find("-");
        std::string first;
        std::string second;

        if (line[pos+1] == ' ' && line[pos-1] == ' ' && line[pos-2] != ' ' && line[pos+2] != ' ') 
        {
            first = line.substr(0, pos-1);
            second = line.substr(pos+2); 
        }
        else if (line[pos+1] != ' ' && line[pos-1] != ' ')
        {
            first = line.substr(0, pos);
            second = line.substr(pos+1);
        }
        else
        {
            int pos2 = line.find(" ");

            if (pos2 < pos)
            {
                first = line.substr(0, pos2);
            }
            else
            {
                first = line.substr(0, pos);
            }
            
            line.substr(pos);
            second = line.substr(pos+1);

            if (second[0] == ' ')
            {
                second = second.substr(1); 
            }

            if (second.back() == ' ')
            {
                pos2 = second.find(" ");
                second = second.substr(0, pos2);
            }
        }

        while(first.back() == ' ') //if there are still unnecessary spaces
        {
            ignoreSpaces(first);
        }

        while(second.back() == ' ')
        {
            ignoreSpaces(second); 
        }

        Node* fst = new Node(first);  //create nodes
        Node* snd = new Node(second);
        
        row.push(fst);
        row.push(snd); 
    }
    else
    {
        throw std::invalid_argument("Problem while reading data");
    }

    if (row.front()->value == "Uspeshnia") //if front is boss
    {
        Node* temp = row.back(); 
        temp->parent = this->boss;
        temp->height = 1;
        this->boss->kids.push_back(temp);
        ++this->employeesCount;
        this->level = 1; 
    }
    else
    { 
        if (!find(row.front()->value)) //check if the front exists
        {
            throw std::invalid_argument("No person");
        }

        Node* keep = getPerson(row.front()->value, this->boss); //create a copy of the front
        Node* temp = row.back(); //the new node
        temp->parent = keep;
        temp->height = keep->height + 1; 
        insert(keep->value, this->boss, temp); 
        ++this->employeesCount;

        if (temp->height > this->level) //check the height
        {
            this->level = temp->height; 
        } 
    }
}

void Hierarchy::copy(Node*& other, Node*& current) //copy function for copy constructor
{
    current = other;
    int size = other->kids.size();

    for (size_t i = 0; i < size; i++)
    {
        copy(other->kids[i], current->kids[i]);
    }
}

Node* Hierarchy::getPerson(const string& name, Node* current) const //get a person from the tree via name
{
    if (name == current->value)
    {
        return current; 
    }

    int size = current->kids.size(); 

    for (size_t i = 0; i < size; i++) //search through kids
    {
        if (current->kids[i]->value == name)
        {
            return current->kids[i]; 
        }
    }

    for (size_t i = 0; i < size; i++) //search through the kids' kids
    {
        Node* temp = getPerson(name, current->kids[i]);

        if (temp != nullptr)
        {
            return temp; 
        }
    }
    
    return nullptr; 
}

void Hierarchy::insert(const string& parent, Node*& current, Node* wanted) //insert wanted into current's kids
{
    if (current->value == parent) //if it is the correct one
    {
        current->kids.push_back(wanted);
        return; 
    }

    int size = current->kids.size();

    for (size_t i = 0; i < size; i++) //search through kids 
    {
        insert(parent, current->kids[i], wanted); 
    }   
}

int Hierarchy::neprekiPodchineni(Node* wanted, Node* current) const 
{
    int final = 0;
    int size = current->kids.size();

    if (wanted == current)
    {
        for (size_t i = 0; i < size; i++)
        {
            int temp = neprekiPodchineni(wanted, current->kids[i]);
            final += temp;
        }
    }
    else if (size > 0)
    {
        int size2 = size; 
        for (size_t i = 0; i < size; i++)
        {
            int temp = neprekiPodchineni(wanted, current->kids[i]);
            size2 += temp; 
        }

        return size2; 
    }
    else 
    {
        return size; 
    }
    
    return final; 
}

void Hierarchy::remove(const string& name, Node*& current) //delete a person
{
    int size = current->kids.size();

    if (current->value == name) //if we get to the correct one
    {
        int parentSize = current->parent->kids.size(); 
        Node* temp = current; //create copy of it

        for (size_t i = 0; i < parentSize; i++)
        {
            if (current->parent->kids[i]->value == current->value) //erase itself from its parent's kids vector
            {
                current->parent->kids.erase(current->parent->kids.begin()+i);
                break;
            }
        }
        
        Node* keepParent = current->parent; //copy of the parent
        
        for (size_t i = 0; i < size; i++) //change the kids' parent from current to current's parent
        {
            temp->kids[i]->parent = keepParent;
            keepParent->kids.push_back(temp->kids[i]);
        }
        
        delete temp; //delete
        --this->employeesCount;

        return;  
    }

    for (size_t i = 0; i < size; i++) //search thorugh kids 
    {
        remove(name, current->kids[i]);
    }
}

void Hierarchy::remove(const string& name, Node*& current) const //const version for join func, but it does not change the kids' parent from current to current's parent
{
    int size = current->kids.size();

    if (current->value == name)
    {
        int parentSize = current->parent->kids.size();
        Node* temp = current; 

        for (size_t i = 0; i < parentSize; i++)
        {
            if (current->parent->kids[i]->value == current->value)
            {
                current->parent->kids.erase(current->parent->kids.begin()+i);
                break;
            }
        }
        
        delete temp;
        return;  
    }

    for (size_t i = 0; i < size; i++)
    {
        remove(name, current->kids[i]);
    }
}

void Hierarchy::setHeight(Node*& current, int level, int& max) //update the height of a changed tree
{
    current->height = level;
    int size = current->kids.size(); 

    if (level > max)
    {
        max = level;
    }

    for (size_t i = 0; i < size; i++)
    {
        setHeight(current->kids[i], level+1, max); 
    }
}

void Hierarchy::setHeight(Node*& current, int level, int& max) const //const version
{
    current->height = level;
    int size = current->kids.size(); 

    if (level > max)
    {
        max = level;
    }

    for (size_t i = 0; i < size; i++)
    {
        setHeight(current->kids[i], level+1, max); 
    }
}

void Hierarchy::changeManager(const string& parent, Node*& current, Node*& wanted)
{
    if (current->value == parent)
    {
        current->kids.push_back(wanted); //make wanted one of current's kids
        int parentSize = wanted->parent->kids.size();
        Node* keep = current; //create a copy
        
        for (size_t i = 0; i < parentSize; i++)
        {
            if (wanted->parent->kids[i] == wanted) //erase itself from its parent's kids
            {
                wanted->parent->kids.erase(wanted->parent->kids.begin()+i);
                break;
            }
        }

        wanted->parent = keep; //set new parent
        wanted->height = keep->height + 1; //set new height
        return;  
    }

    int size = current->kids.size();

    for (size_t i = 0; i < size; i++) //search through kids 
    {
        changeManager(parent, current->kids[i], wanted); 
    }
}

void Hierarchy::changeManager(const string& parent, Node*& current, Node*& wanted) const //const version 
{
    if (current->value == parent)
    {
        current->kids.push_back(wanted);
        int parentSize = wanted->parent->kids.size();
        Node* keep = current;
        
        for (size_t i = 0; i < parentSize; i++)
        {
            if (wanted->parent->kids[i] == wanted)
            {
                wanted->parent->kids.erase(wanted->parent->kids.begin()+i);
                break;
            }
        }

        wanted->parent = keep;
        wanted->height = keep->height + 1;
        return;  
    }

    int size = current->kids.size();

    for (size_t i = 0; i < size; i++)
    {
        changeManager(parent, current->kids[i], wanted); 
    }
}

void Hierarchy::search(int level, int& count, Node* current) const //function for num_overloaded
{
    int nepreki = neprekiPodchineni(current, current);
    int preki = num_subordinates(current->value);

    if (preki + nepreki > level) //if someone's preki and nepreki > 20
    {
        ++count; 
    }

    int size = current->kids.size();

    for (size_t i = 0; i < size; i++) //search through kids 
    {
        search(level, count, current->kids[i]);
    }
}

Node* Hierarchy::getPersonMaxSalary(Node* current)
{
    int size = current->kids.size();

    if (size == 0)
    {
        return nullptr;
    }

    int maxSalary = getSalary(current->kids[0]->value); //set the first kid to be maxSalary
    Node* keep = current->kids[0];

    for (size_t i = 0; i < size; i++)
    {
        int curr = getSalary(current->kids[i]->value);

        if (curr > maxSalary) //compare and change if needed
        {
            maxSalary = curr;
            keep = current->kids[i];
        }
    }

    for (size_t i = 0; i < size; i++)
    {
        if (getSalary(current->kids[i]->value) == maxSalary && current->kids[i] != keep) //if there are two people with the same salary
        {
            if (current->kids[i]->value < keep->value) //get the person with the shorter name 
            {
               keep = current->kids[i];
            }
        }
    }
    
    return keep; 
}

void Hierarchy::getPeopleAtLevel(Node* current, int level, std::vector<Node*>& people) //function that fills a vector with people at wanted level
{
    int size = current->kids.size();

    if (current->height == level)
    {
        people.push_back(current);
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            getPeopleAtLevel(current->kids[i], level, people);
        }
    }
}

void Hierarchy::getPeopleAtLevel2(Node* current, int level, std::vector<Node*>& people) const //const version that puts the people in lexicogrpahical order
{
    int size = current->kids.size();

    if (current->height == level)
    {
        size_t x = 0;
        int count = people.size();
        
        for (; x < count; x++)
        {
            if (current->value < people[x]->value)
            {
                break;
            }
        }
        
        people.emplace(people.begin()+x, current);
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            getPeopleAtLevel2(current->kids[i], level, people);
        }
    }
}

void Hierarchy::incorporater(int& level)
{ 
    std::vector<Node*> peopleCurrentLevel; //vector with the almost lowest level
    getPeopleAtLevel(this->boss, level, peopleCurrentLevel);
    int size = peopleCurrentLevel.size();

    for (size_t i = 0; i < size; i++)
    {
        int size2 = peopleCurrentLevel[i]->kids.size();

        if (size2 > 1) //if someone's kids are >=2
        {
            Node* keep = getPersonMaxSalary(peopleCurrentLevel[i]); //get the one with the max salary
            std::vector<Node*> peopleCurrentLevelKids;

            for (size_t j = 0; j < size2; j++) //extract this person's all kids 
            {
                peopleCurrentLevelKids.push_back(peopleCurrentLevel[i]->kids[j]);
            }

            for (size_t j = 0; j < size2; j++) //change their manager 
            {
                if (peopleCurrentLevelKids[j] != keep) //make his siblings his own subordinates
                {
                    Node* temp = peopleCurrentLevelKids[j];
                    changeManager(keep->value, this->boss, temp);
                }
            }
        }
    }

    if (level == 0) //if we get to the boss
    {
        return; 
    }
    
    --level; //go to next level 
    incorporater(level); 
}

void Hierarchy::modernizer(int& level)
{
    if (level % 2 == 0) //if the level is divided by 2, skip 
    {
        --level;
        modernizer(level);
    }
    
    std::vector<Node*> peopleCurrentLevel;
    getPeopleAtLevel(this->boss, level, peopleCurrentLevel);
    int size = peopleCurrentLevel.size();

    for (size_t i = 0; i < size; i++)
    {
        if (peopleCurrentLevel[i]->kids.size() > 0) //if someone at this level has kids, remove him (remove will manage the kids' new parent)
        {
            remove(peopleCurrentLevel[i]->value, this->boss);
        }
    }
    
    if (level == 1) //if get to the boss's kids 
    {
        return; 
    }
    
    --level; //change level
    modernizer(level); 
}

void Hierarchy::printer(int& level, std::string& str) const
{
    std::vector<Node*> peopleCurrentLevel; //get people from the second highest level
    getPeopleAtLevel2(this->boss, level, peopleCurrentLevel);
    int size = peopleCurrentLevel.size();
    string add;

    for (size_t i = 0; i < size; i++)
    { 
        std::vector<Node*> peopleCurrentLevelKids;
        getPeopleAtLevel2(peopleCurrentLevel[i], level+1, peopleCurrentLevelKids); //get this person's kids in lexicographical order
        int size2 = peopleCurrentLevelKids.size();

        for (size_t j = 0; j < size2; j++) //add to str
        {
            add = peopleCurrentLevel[i]->value + "-" + peopleCurrentLevelKids[j]->value  + "\n";
            str += add; 
        }
    }

    if (level == longest_chain() ) //if we get to the bottom, return 
    {
        return;
    }

    ++level; //change level
    printer(level, str);
}

string Hierarchy::joiner(Node*& current, Hierarchy& original, Hierarchy& copy) const
{
    int size = current->kids.size();
    Node* originalPerson = new Node(""); //new node for duplicates

    for (size_t i = 0; i < size; i++)
    {
        Node* temp = new Node(current->kids[i]->value); //create a copy for each of current's kids

        if (find(temp->value)) //if there is the same person in the other tree
        {
            originalPerson = getPerson(temp->value, original.boss); 
        }
        
        if (current->value == "Uspehsnia") //if current is the boss, add him as his kids
        {
            temp->parent = original.boss;
            temp->height = 1; 
            original.boss->kids.push_back(temp);
            ++original.employeesCount;
        }
        else
        {
            string manager = current->kids[i]->parent->value; 

            Node* keep = getPerson(manager, original.boss);
            temp->parent = keep;
            temp->height = keep->height + 1; 
            keep->kids.push_back(temp);
            ++original.employeesCount;
        }

        if (originalPerson->value == temp->value) //if we have duplicates
        {
            checkJoining(originalPerson, copy.boss); //check if there are contradicting employees 
            duplicates(originalPerson, temp, original.employeesCount, copy.employeesCount, original.boss, copy.boss); 
        }   
    }

    for (size_t i = 0; i < size; i++) //search through kids 
    {
        joiner(current->kids[i], original, copy);
    }

    return original.print();
}

void Hierarchy::duplicates(Node*& current1, Node*& current2, int& count1, int& count2, Node* boss1, Node* boss2) const
{
    if ((current1->parent == current2->parent && current1->value < current2->value) ||
        (current1->parent != current2->parent && current1->parent->height < current2->parent->height) || 
        (current1->parent != current2->parent && current1->parent->height == current2->parent->height && current1->parent->value < current2->parent->value))
    {
        int size = current2->kids.size();

        for (size_t i = 0; i < size; i++) //change the manager of current2's kids to the other duplicate
        {
            Node* temp = current2->kids[i];
            changeManager(current1->value, boss2, temp);   
        }

        remove(current2->value, current2); //remove current2
        --count2;
    }
    else if ((current1->parent == current2->parent && current1->value > current2->value) ||
        (current1->parent != current2->parent && current1->parent->height > current2->parent->height) ||
        (current1->parent != current2->parent && current1->parent->height == current2->parent->height && current1->parent->value > current2->parent->value))
    {
        int size = current1->kids.size();

        for (size_t i = 0; i < size; i++) //change the manager of current1's kids to the other duplicate
        {
            Node* temp = current1->kids[i];
            changeManager(current2->value, boss1, temp); 
        }

        remove(current1->value, current1); //remove current1
        --count1;
    }
}

void Hierarchy::checkJoining(Node*& current, Node*& otherBoss) const
{ 
    Node* copy = getPerson(current->value, otherBoss); //get the person in the other tree

    if (current->value != "Uspeshnia" && copy)
    {
        if (getPerson(current->parent->value, copy)) //search for current's parent through copy's kids 
        {
            throw std::invalid_argument("Contradicting employees");
        }
    }
    else
    {
        checkJoining(current->parent, otherBoss); //change current to his manager
    }
}

void Hierarchy::deleteEverything(Node* current)
{
    int size = current->kids.size();

    for (size_t i = 0; i < size; i++)
    {
        deleteEverything(current->kids[i]);
    }

    delete current;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
{
    this->boss = r.boss;
    r.boss = nullptr;
    this->employeesCount = r.employeesCount;
    r.employeesCount = 0;
    this->level = r.level;
    r.level = 0;
    this->name = r.name;
    r.name = "";
}

Hierarchy::Hierarchy(const Hierarchy& r)
{
    this->boss = r.boss;
    this->employeesCount = r.employeesCount;
    this->level = r.level;  
    this->name = r.name; 

    Node* cpy = r.boss;
    copy(cpy, this->boss);
}

Hierarchy::Hierarchy(const string& data)
{ 
    if (data != "") //it is not empty
    {
        this->boss = new Node("Uspeshnia");
    }
    
    if (data != "" && checkFile(data)) //if we have a file
    {
        std::ifstream file(data, std::ios::in);

        if (!file.is_open()) //Throw exception if the file cannot be opened 
        {
            throw std::invalid_argument("Problem while reading data");
        }
        
        std::string line; 

        while(getline(file, line)) 
        {
            read(line); 
        }
        
        file.close();
    }
    else //if we have a string
    {
        if (data == "") //if it is empty
        {
            this->level = -1;
            this->employeesCount = -1;
        }
        else
        {
            string copy = data; //copy string

            while (!copy.empty())
            {
                int pos = copy.find("\n"); //extract each row
                string toRead = copy.substr(0, pos);
                toRead = ignoreSpaces(toRead); 
                
                read(toRead);
                copy = copy.substr(pos+1); //cut off the row
            }
        }
    }
    
}

Hierarchy::~Hierarchy() noexcept
{
    if (this->boss)
    {
        deleteEverything(this->boss);
    }
}

string Hierarchy::print() const
{
    if (this->level == -1) //if it is empty
    {
        return "";
    }
    
    int level = 0;
    std::string str = "";
    printer(level, str);

    return str;
}

int Hierarchy::longest_chain() const
{
    return this->level + 1; 
}

bool Hierarchy::find(const string& name) const
{
    if (this->boss == nullptr) //if it is empty
    {
        return false;
    }

    if (getPerson(name, this->boss) != nullptr)
    {
        return true; 
    }
    
    return false; 
}

int Hierarchy::num_employees() const
{
    return this->employeesCount + 1; 
}

int Hierarchy::num_overloaded(int level) const
{
    if (num_employees() < level || this->boss == nullptr)
    {
        return 0; 
    }

    int count = 0;
    search(level, count, this->boss);

    return count;  
}

string Hierarchy::manager(const string& name) const
{
    if (name == this->boss->value || this->boss == nullptr) //if we search for the boss or it is empty
    {
        return "";
    }
    
    Node* temp = getPerson(name, this->boss);
    
    if (temp != nullptr)
    {
        return temp->parent->value;
    }

    return nullptr; 
}

int Hierarchy::num_subordinates(const string& name) const
{
    if (this->boss == nullptr) //if it is empty
    {
        return -1;
    }

    Node* temp = getPerson(name, this->boss);
    
    if (temp != nullptr)
    {
        return temp->kids.size(); 
    }

    return -1;
}

unsigned long Hierarchy::getSalary(const string& who) const
{  
    if (this->boss == nullptr) //if it is empty
    {
        return -1;
    }

    Node* temp = getPerson(who, this->boss);
    
    if (temp != nullptr)
    {
        unsigned long int nepreki = neprekiPodchineni(temp, temp);
        return temp->kids.size()*500 + nepreki*50;
    }

    return -1; 
}

bool Hierarchy::fire(const string& who)
{
    if (!find(who) || who == this->boss->value || this->boss == nullptr)
    {
        return false; 
    }

    remove(who, this->boss);

    return true;  
}

bool Hierarchy::hire(const string& who, const string& boss)
{
    if (!find(boss) || who == this->boss->value || this->boss == nullptr)
    {
        return false; 
    }

    if (!find(who)) //if it is a new person
    {
        Node* temp = new Node(who);
        Node* keep = getPerson(boss, this->boss);
        temp->value = who; 
        temp->parent = keep;
        temp->height = keep->height + 1; 
        insert(keep->value, this->boss, temp); 
        ++this->employeesCount;

        if (temp->height > this->level)
        {
            this->level = temp->height; 
        }
    }
    else
    {
        Node* temp = getPerson(who, this->boss);
        changeManager(boss, this->boss, temp);

        if (temp->height > this->level)
        {
            this->level = temp->height; 
        }
    }
    
    return true;
}

void Hierarchy::incorporate()
{
    int level = longest_chain() - 1; //start level
    int max = 0;

    incorporater(level);
    setHeight(this->boss, 0, max); //update tree
    this->level = max; 
}

void Hierarchy::modernize()
{
    int level = longest_chain() - 1; //start level 
    int max = 0;
    
    modernizer(level);
    setHeight(this->boss, 0, max); //update tree
    this->level = max;  
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
    int max = 0;
    Hierarchy original(print()); //create a copy of *this 
    Hierarchy cpy(right.print());  //create a copy of right
    
    if (this->boss == nullptr) //if *this is empty
    {
        return cpy;
    }
    else if (right.boss == nullptr) //if right is empty
    {
        return original; 
    }
    else if (original.print() == cpy.print()) //if they are equal hierarchies
    {
        return original;
    }
    
    string str = joiner(cpy.boss, original, cpy);
    
    Hierarchy joined(str);
    setHeight(joined.boss, 0, max);
    joined.level = max;

    return joined;
}

void Hierarchy::save(const string& file)
{
    std::ofstream outFile(file, std::ios::trunc); 

	if (!outFile.is_open()) 
	{
		throw std::invalid_argument("Cannot open file"); 
	}

    outFile << this->print(); 
	outFile.close(); 

	cout << "File is saved!" << endl;
}

void Hierarchy::setName(const string& str)
{
    this->name = str;
}

string Hierarchy::getName() const
{
    return this->name;
}