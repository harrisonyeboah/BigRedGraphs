test: graph_tests.o
	g++ -std=c++17 -o test graph_tests.o

graph_tests.o: graph_tests.cpp graph.h graph.cpp
	g++ -std=c++17 -c graph_tests.cpp

clean:
	rm -f test *.o