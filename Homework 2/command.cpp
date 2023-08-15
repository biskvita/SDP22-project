#include "command.h"

bool Command::checkName(std::string str)
{
    int size = str.size();

    for (size_t i = 0; i < size; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
        {
            return false; 
        }
    }
    
    return true; 
}

void Command::turnInLowerCase(string& str)
{
    int size = str.length();
    for (size_t i = 0; i < size; ++i)
    {
        std::string convert(1, tolower(str[i]));
        str.replace(i, 1, convert);
    }
}

void Command::getArguments(const string& input)
{
	size_t i = 0;
    size_t size = input.size(); 

    while (i < size)
    {
        if (i < size && input[i] != ' ')
        {
            std::string tempStr = "";
            while (i < size && input[i] != ' ')
            {
                tempStr.push_back(input.at(i));
                ++i;
            }

            storage.push_back(tempStr);
        }
        else
        {
            ++i; 
        }
    } 
}

//! Main function for interacting with the user
void Command::execute() 
{
    std::string input; 

    // Arguments 
    std::string first;
    std::string second;
    std::string third;
    std::string fourth; 
	
	do
    {
        storage.clear();

        // Menu type 
        cout << "##############################" << endl;
        cout << "Choose what to do with a file:" << endl << ">";
        std::getline(cin, input);

        cout << "##############################" << endl;

        getArguments(input);

        // Get arguments
        first = storage[0];
        second = storage[1];

        if (storage.size() > 2)
        {
            third = storage[2];
        }
        else
        {
            third = ""; 
        }

        if (storage.size() > 3)
        {
            fourth = storage[3];
        }
        else
        {
            fourth = ""; 
        }
        
        turnInLowerCase(first); 
        
        // Cases
        if (first.compare("help") == 0)
        {
            cout << "---------List of commands---------" << endl;
            cout << "load /name/ /file/" << endl;
            cout << "save /name/ /file/" << endl;
            cout << "find /where/ /who/" << endl;
            cout << "num_subordinates /where/ /who/" << endl;
            cout << "num_employees /where/" << endl;
            cout << "manager /where/ /who/" << endl;
            cout << "overloaded /where/" << endl;
            cout << "join /where1/ /where2/ /name/" << endl;
            cout << "fire /where/ /who/" << endl;
            cout << "hire /where/ /who/ /boss/" << endl;
            cout << "salary /where/ /who/" << endl;
            cout << "incorporate /where/" << endl;
            cout << "modernize /where/" << endl;
            cout << "----------------------------------" << endl;
        }
        else if (first.compare("load") == 0)
        {
            if (second.size() > 0 && !checkName(second))
            {
                cout << "Wrong name" << endl;
                return;
            }

            if (third.size() > 0)
            {
                Hierarchy* littleHierarchy = new Hierarchy(third); 
                littleHierarchy->setName(second);
                hierarchies.push_back(littleHierarchy);
            }
            else
            {
                std::string temp;

                while (std::getline(std::cin, temp) && (temp != "^Z" || temp != "^D"))
                {
                    third += temp;
                    third += '\n';
                }
                
                Hierarchy* littleHierarchy = new Hierarchy(third); 
                littleHierarchy->setName(second);
                hierarchies.push_back(littleHierarchy);
                cin.clear();  
            } 

            cout << second << " successfully loaded!" << endl;
        }
        else if (first.compare("save") == 0) 
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    if (third.size() > 0)
                    {
                        hierarchies[i]->save(third);
                    }
                    else
                    {
                        cout << endl << hierarchies[i]->print() << endl;
                    } 

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("find") == 0) 
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    if (hierarchies[i]->find(third))
                    {
                        cout << third << " is employed in " << second << endl;
                    }
                    else
                    {
                        cout << "there is no " << third << " in " << second << endl;
                    }

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("num_subordinates") == 0) 
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    int num = hierarchies[i]->num_subordinates(third);

                    if (num > 0)
                    {
                        cout << third << " has " << num << " subordinates " << endl;
                    }
                    else
                    {
                        cout << third << " has no subordinates " << endl;
                    }

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }  
        }
        else if (first.compare("num_employees") == 0) 
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    int num = hierarchies[i]->num_employees();

                    if (num > 0)
                    {
                        cout << second << " has " << num << " employees " << endl;
                    }
                    else
                    {
                        cout << second << " has no employees " << endl;
                    }

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            } 
        }
        else if (first.compare("manager") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    std::string str = hierarchies[i]->manager(third); 

                    if (str != "")
                    {
                        cout << "the manager of " << third << " is " << str << endl;
                    }
                    else
                    {
                        cout << "there is no " << third << " in " << second << endl;
                    }

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }

            }
        }
        else if (first.compare("overloaded") == 0) 
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    int num = hierarchies[i]->num_overloaded();

                    if (num > 0)
                    {
                        cout << num << " overloaded employees in " << second << endl;
                    }
                    else
                    {
                        cout << "no overloaded employees in " << second << endl;
                    }

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("join") == 0)  
        {
            int i = 0;
            int j = 0;
            int size = hierarchies.size(); 
            
            for (size_t i = 0; i < size; i++)
            {
                for (size_t j = 1; j < size; j++)
                {
                    if (second == hierarchies[i]->getName() && third == hierarchies[j]->getName())
                    {
                        Hierarchy* joined = new Hierarchy(hierarchies[i]->join(*hierarchies[j]));
                        joined->setName(fourth);
                        hierarchies.push_back(joined);
                    }
                    else if (second == hierarchies[j]->getName() && third == hierarchies[i]->getName())
                    {
                        Hierarchy* joined = new Hierarchy(hierarchies[j]->join(*hierarchies[i]));
                        joined->setName(fourth);
                        hierarchies.push_back(joined);
                    }
                }
            }
            
            cout << fourth << " created" << endl;
        }
        else if (first.compare("fire") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    hierarchies[i]->fire(third); 
                    cout << third << " was fired" << endl;

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }  
        }
        else if (first.compare("hire") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    hierarchies[i]->hire(third, fourth); 
                    cout << third << " was hired" << endl;

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            } 
        }
        else if (first.compare("salary") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    int num = hierarchies[i]->getSalary(third);
                    cout << "The salary of " << third << " is " << num << " BGN" << endl;

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("incorporate") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    hierarchies[i]->incorporate();
                    cout << second << " incorporated" << endl;

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("modernize") == 0)  
        {
            int size = hierarchies.size(); 
            for (size_t i = 0; i < size; i++)
            {
                if (second == hierarchies[i]->getName())
                {
                    hierarchies[i]->modernize();
                    cout << second << " modernized" << endl;

                    break;
                }
                else if (i == size - 1)
                {
                    cout << second << " is an unknown office" << endl;
                }
            }
        }
        else if (first.compare("exit") == 0) 
        {
            cout << "You have exited the program" << endl;
        }   
        else 
        {
            cout << "Wrong command!" << endl;
        }
         
    } while (first.compare("exit") != 0);
}

Command::~Command()
{
    int size = hierarchies.size();

    for (size_t i = 0; i < size; i++)
    {
        delete hierarchies[i];
    }
}
