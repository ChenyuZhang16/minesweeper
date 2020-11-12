execute: main.o functions.o
	g++ -Wall -g main.o functions.o -o execute

main.o: main.cpp functions.h
	g++ -Wall -g -c main.cpp -o main.o

functions.o: functions.cpp functions.h
	g++ -Wall -g -c functions.cpp -o functions.o

clean:
	rm *.o execute
