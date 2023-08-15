testsExecutable: tests.o mystore.o event.o queue.o worker.o catch2.o

	g++ -o testsExecutable tests.o mystore.o event.o queue.o worker.o catch2.o

consoleAppExecutable: main.o mystore.o event.o queue.o worker.o 

	g++ -o consoleAppExecutable main.o mystore.o event.o queue.o worker.o

moretestsExecutable: moretests.o mystore.o event.o queue.o worker.o catch2.o

	g++ -o moretestsExecutable moretests.o mystore.o event.o queue.o worker.o catch2.o	 

tests.o: tests.cpp

	g++ -c tests.cpp

moretests.o: moretests.cpp

	g++ -c moretests.cpp	

main.o: main.cpp

	g++ -c main.cpp

mystore.o: mystore.cpp

	g++ -c mystore.cpp

event.o: event.cpp

	g++ -c event.cpp

queue.o: queue.cpp

	g++ -c queue.cpp

worker.o: worker.cpp

	g++ -c worker.cpp

catch2.o: catch2.cpp

	g++ -c catch2.cpp




	


