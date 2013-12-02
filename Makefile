all: node.o train.o test.o
	g++ -o test test.o node.o
	g++ -o train train.o node.o

train: train.o node.o
	g++ -o train train.o node.o
	
test: test.o node.o
	g++ -o test test.o node.o

train.o: train.cpp
	g++ -c -g train.cpp

test.o: test.cpp
	g++ -c -g test.cpp

node.o: node.cpp node.h
	g++ -c -g node.cpp

clean:
	rm -f *.out *.o *.stackdump *~