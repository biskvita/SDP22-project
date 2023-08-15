#include "command.h"

using std::cout;
using std::cin;
using std::endl; 

//g++ tests.cpp hierarchy.cpp catch2.cpp

int main(int argc, char* argv[])
{
    string lozenec =
    "Uspeshnia - Gosho \n"
    "Uspeshnia - Misho \n"
    "Gosho     - Pesho \n"
    "Gosho     - Dancho\n"
    "Pesho     - Alex  \n"
    "Dancho    - Boris \n"
    "Dancho    - Kamen \n"
    "Uspeshnia - Slavi \n"
    "Slavi     - Slav1 \n"
    "Slavi     - Slav2 \n"
    "Slav1     - Mecho \n"
    "Mecho     - Q12Adl\n";

    // const string loz_new =
    // " Uspeshnia   -  MishoPetrov \n"
    // " MishoPetrov -  Misho       \n"
    // " MishoPetrov -  Slav        \n";

    const string loz_new2 =
    " Uspeshnia   -  MishoPetrov \n"
    " Uspeshnia   -  Mecho       \n"
    " MishoPetrov -  Misho       \n"
    " MishoPetrov -  Slav        \n"
    " Mecho       -  Slav1       \n"
    " Misho       -  zzzzzzzzzzzz\n";

    // const string large =
    // "Uspeshnia - 1\n"
    // "1-2\n1-3\n1-4\n1-5\n1-6\n1-7\n"
    // "2-21\n2-22\n2-23\n2-24\n2-25\n2-26\n2-27\n2-28\n"
    // "3-31\n3-32\n3-33\n3-34\n3-35\n3-36\n3-37\n3-38\n3-39\n";

    Command command;
    //command.execute();

    Hierarchy* proba = new Hierarchy(lozenec);
    Hierarchy* proba2 = new Hierarchy(loz_new2);
    Hierarchy* proba3 = new Hierarchy(proba->join(*proba2));
    cout << proba3->print() << endl;

    delete proba;
    delete proba2;
    delete proba3; 
    
    return 0; 
}