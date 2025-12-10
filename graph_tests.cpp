// *******************************************
//  CS 271 Graph Project:  Test File
// *******************************************

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <climits>
#include <tuple>
#include "graph.h"
using namespace std;

// helper function to create a graph from a file
// first line: number of vertices (v) and number of edges (e)
// next v lines: vertex key (int) and data (string)
// next e lines: edge from vertex1 to vertex2 with weight
Graph<int, string> createGraphFromFile(const string& filename)
{
    Graph<int, string> g;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return g;
    }
    int v, e;
    infile >> v >> e;
    for (int i = 0; i < v; ++i) {
        int key;
        string data;
        infile >> key >> data;
        g.insertVertex(key, "12");
    }
    for (int i = 0; i < e; ++i) {
        int from, to;
        double weight;
        infile >> from >> to >> weight;
    }
    return g;
}

Graph<int, std::tuple<double,double>> createBigRed(const string& filename)
{
    using coordinates = std::tuple<double, double>;
    Graph<int, coordinates> g;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: couldn't open the file." << endl;
        return g;
    }

    int v, e;
    infile >> v >> e;  

    for (int i = 0; i < v; ++i) {
        int key;
        double x, y;
        infile >> key >> x >> y;
        g.insertVertex(key, make_tuple(x, y));
    }

    
    string introLine;
    getline(infile, introLine);
    
    string line;
    int from, to;
    double weight;
    string name;
    int count = 1;
    int lineNumber = 456;

    for (int i = 0; i < e; ++i) {

        do {
            if (!getline(infile, line)) {
                break;
            }
        } while (line.empty());

        stringstream ss(line);

        ss >> from >> to >> weight;

        getline(ss, name);
    
        if (!name.empty() && name[0] == ' ')
            name.erase(0, 1);

        try {
            if (name[1] != ' ') {
                g.insertEdge(from, to, weight, name.substr(1));            
            }
            else {
                g.insertEdge(from, to, weight, "");
            }


            
        } catch (const std::exception& err) {
            cout << "There is an error" << endl;
        }

        count++;
        lineNumber++;
        
    }
    return g;
}

/*
void test_DFS()
{
   Graph<int, string> g = createGraphFromFile("example.txt");
   auto start = chrono::high_resolution_clock::now();
   g.DFS();
   auto end = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsed = end - start;
   cout << "DFS took " << elapsed.count() << " seconds." << endl;
}

void test_topologicalSort()
{
   // create graph from file example.txt
   Graph<int, string> g = createGraphFromFile("example.txt");
   string topSortResult = g.topologicalSort();
   cout << "Topological Sort Result: " << topSortResult << endl;
   if (topSortResult != "0->1->2->3") {
       cout << "Topological sort result is incorrect. Expected: 0->1->2->3 but got: " << topSortResult << endl;
   }
}

void test_BFS()
{
   try{
       Graph<int, string> g = createGraphFromFile("example.txt");
       auto start = chrono::high_resolution_clock::now();
       g.BFS(0);
       auto end = chrono::high_resolution_clock::now();
       chrono::duration<double> elapsed = end - start;
       cout << "BFS took " << elapsed.count() << " seconds from source 0." << endl;
   }
   catch (std::exception& e) {
       cerr << "Error testing BFS: " << e.what() << endl;
       return;
   }
}

void test_shortestPath()
{
    try{
        Graph<int, string> g = createGraphFromFile("example.txt");
        string path = g.shortestPath(0, 3);
        cout << "Shortest path from 0 to 3: " << path << endl;
        if (path != "0->2->3") {
            cout << "Shortest path result is incorrect. Expected: 0->2->3 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}

void test_asAdjMatrix()
{
    try{
        Graph<int, string> g = createGraphFromFile("example.txt");
        int** adjMatrix = g.asAdjMatrix();
        if (adjMatrix == nullptr) {
            cout << "Error: adjacency matrix is null." << endl;
            return;
        }
        // Convert adjMatrix to string for comparison
        string actualString;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (adjMatrix[i][j] == INT_MAX)
                    actualString += "inf ";
                else
                actualString += to_string(adjMatrix[i][j]) + " ";
            }
            actualString.pop_back(); // remove trailing space
            actualString += "\n";
        }
        cout << "Adjacency Matrix:" << endl;
        cout << actualString << endl;

        int expectedMatrix[4][4] = {
            {INT_MAX, 1, 2, INT_MAX},
            {INT_MAX, INT_MAX, 3, INT_MAX},
            {INT_MAX, INT_MAX, INT_MAX, 4},
            {INT_MAX, INT_MAX, INT_MAX, INT_MAX}
        };
        
        string expectedString ="inf 1 2 inf\ninf inf 3 inf\ninf inf inf 4\ninf inf inf inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

// ==========================================
// disconnected.txt
// ==========================================

void test_disconnected_DFS() {
   Graph<int, string> g = createGraphFromFile("disconnected.txt");
   auto start = chrono::high_resolution_clock::now();
   g.DFS();
   auto end = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsed = end - start;
   cout << "DFS (Disconnected) took " << elapsed.count() << " seconds." << endl;
}

void test_disconnected_BFS() {
   try{
       Graph<int, string> g = createGraphFromFile("disconnected.txt");
       auto start = chrono::high_resolution_clock::now();
       g.BFS(0);
       auto end = chrono::high_resolution_clock::now();
       chrono::duration<double> elapsed = end - start;
       cout << "BFS (Disconnected) took " << elapsed.count() << " seconds from source 0." << endl;
   }
   catch (std::exception& e) {
       cerr << "Error testing BFS: " << e.what() << endl;
   }
}

void test_disconnected_shortestPath() {
    try{
        Graph<int, string> g = createGraphFromFile("disconnected.txt");
        string path = g.shortestPath(0, 3);
        cout << "Shortest path from 0 to 3 (Disconnected): '" << path << "'" << endl;
        
        // Expected: Empty string (no path)
        if (path != "") {
            cout << "Shortest path result is incorrect. Expected: '' (empty) but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}

void test_disconnected_asAdjMatrix() {
    try{
        Graph<int, string> g = createGraphFromFile("disconnected.txt");
        int** adjMatrix = g.asAdjMatrix();
        if (adjMatrix == nullptr) {
            cout << "Error: adjacency matrix is null." << endl;
            return;
        }
        string actualString;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (adjMatrix[i][j] == INT_MAX)
                    actualString += "inf ";
                else
                actualString += to_string(adjMatrix[i][j]) + " ";
            }
            actualString.pop_back();
            actualString += "\n";
        }
        cout << "Adjacency Matrix (Disconnected):" << endl;
        cout << actualString << endl;

        // 0->1 (10), 2->3 (20), Rest INF
        string expectedString ="inf 10 inf inf\ninf inf inf inf\ninf inf inf 20\ninf inf inf inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

// ==========================================
// cycle.txt
// ==========================================

void test_cycle_DFS() {
   Graph<int, string> g = createGraphFromFile("cycle.txt");
   auto start = chrono::high_resolution_clock::now();
   g.DFS();
   auto end = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsed = end - start;
   cout << "DFS (Cycle) took " << elapsed.count() << " seconds." << endl;
}

void test_cycle_BFS() {
   try{
       Graph<int, string> g = createGraphFromFile("cycle.txt");
       auto start = chrono::high_resolution_clock::now();
       g.BFS(0);
       auto end = chrono::high_resolution_clock::now();
       chrono::duration<double> elapsed = end - start;
       cout << "BFS (Cycle) took " << elapsed.count() << " seconds from source 0." << endl;
   }
   catch (std::exception& e) {
       cerr << "Error testing BFS: " << e.what() << endl;
   }
}

void test_cycle_shortestPath() {
    try{
        Graph<int, string> g = createGraphFromFile("cycle.txt");
        string path = g.shortestPath(0, 2);
        cout << "Shortest path from 0 to 2 (Cycle): " << path << endl;
        
        // Expected: 0->1->2
        if (path != "0->1->2") {
            cout << "Shortest path result is incorrect. Expected: 0->1->2 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}

void test_cycle_asAdjMatrix() {
    try{
        Graph<int, string> g = createGraphFromFile("cycle.txt");
        int** adjMatrix = g.asAdjMatrix();
        if (adjMatrix == nullptr) {
            cout << "Error: adjacency matrix is null." << endl;
            return;
        }
        string actualString;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (adjMatrix[i][j] == INT_MAX)
                    actualString += "inf ";
                else
                actualString += to_string(adjMatrix[i][j]) + " ";
            }
            actualString.pop_back();
            actualString += "\n";
        }
        cout << "Adjacency Matrix (Cycle):" << endl;
        cout << actualString << endl;

        // 0->1, 1->2, 2->0
        string expectedString ="inf 1 inf\ninf inf 1\n1 inf inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

// ==========================================
// other.txt
// ==========================================
void otherTestBFS() {
    try{
       Graph<int, string> g = createGraphFromFile("other.txt");
       auto start = chrono::high_resolution_clock::now();
       g.BFS(0);
       auto end = chrono::high_resolution_clock::now();
       chrono::duration<double> elapsed = end - start;
       cout << "BFS took " << elapsed.count() << " seconds from source 0." << endl;
   }
   catch (std::exception& e) {
       cerr << "Error testing BFS: " << e.what() << endl;
       return;
   }

}


void otherTestAdj() {
    try{
        Graph<int, string> g = createGraphFromFile("other.txt");
        int** adjMatrix = g.asAdjMatrix();
        if (adjMatrix == nullptr) {
            cout << "Error: adjacency matrix is null." << endl;
            return;
        }
        // Convert adjMatrix to string for comparison
        string actualString;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (adjMatrix[i][j] == INT_MAX)
                    actualString += "inf ";
                else
                actualString += to_string(adjMatrix[i][j]) + " ";
            }
            actualString.pop_back(); // remove trailing space
            actualString += "\n";
        }
        cout << "Adjacency Matrix:" << endl;
        cout << actualString << endl;

    int expectedMatrix[6][6] = {
        {INT_MAX, 3,       1,       4,       INT_MAX, INT_MAX},
        {INT_MAX, INT_MAX, 2,       INT_MAX, 5,       INT_MAX},
        {INT_MAX, INT_MAX, INT_MAX, 2,       INT_MAX, 7      },
        {INT_MAX, INT_MAX, INT_MAX, INT_MAX, 1,       3      },
        {6,        INT_MAX, INT_MAX, INT_MAX, INT_MAX, 2      },
        {INT_MAX, 4,        INT_MAX, INT_MAX, INT_MAX, INT_MAX}
    };

        
        string expectedString =
        "inf 3 1 4 inf inf\n"
        "inf inf 2 inf 5 inf\n"
        "inf inf inf 2 inf 7\n"
        "inf inf inf inf 1 3\n"
        "6 inf inf inf inf 2\n"
        "inf 4 inf inf inf inf\n";


        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }  

    // Now I will try with an empty graph
    try {
        Graph<int, string> g;  

        int** adjMatrix = g.asAdjMatrix();

        if (g.size() != 0) {
             cout << "Error: Empty graph should have size 0." << endl;
        }
        string actualString = "";   
        string expectedString = "";

        if (actualString != expectedString) {
            cout << "Adjacency matrix not correct. This was the expected:\n"
                << expectedString << "\nbut instead I got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "There's an error for testing: " << e.what() << endl;
    }

    
}


void otherTestShortestPath() {
    try {
        Graph<int, string> g = createGraphFromFile("other.txt");
        g.BFS(0);
        string path = g.shortestPath(0, 5);

        cout << "Shortest path from 0 to 5: " << path << endl;

    if (path != "0->2->5") {
        cout << "Shortest path result is incorrect. Expected: 0->2->5 but got: " << path << endl;
    }
    } 
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}
*/

int main()
{
    /*
    Graph<int, string> g = createGraphFromFile("example.txt");
    cout << "Graph created from file:" << endl;
    cout << g.toString() << endl;

    test_DFS();
    test_topologicalSort();
    test_BFS();
    test_shortestPath();
    test_asAdjMatrix();

    cout << "\n=== TESTS (Disconnected) ===" << endl;
    test_disconnected_DFS();
    test_disconnected_BFS();
    test_disconnected_shortestPath();
    test_disconnected_asAdjMatrix();
    
    cout << "\n=== TESTS (Cycle) ===" << endl;
    test_cycle_DFS();
    test_cycle_BFS();
    test_cycle_shortestPath();
    test_cycle_asAdjMatrix();

    cout << "\n=== TESTS (other) ===" << endl;
    otherTestBFS();
    otherTestShortestPath();
    otherTestAdj();
    cout << "Testing completed" << endl;
    */

    // This is based on my project this is now for the new project.

    Graph<int, std::tuple<double, double>> bigRedGraph = createBigRed("denison.txt");
    int startDistance = 0;
    cout << " This is regular unweighted " << endl;
    cout << bigRedGraph.shortestPath(44137, 73704, startDistance);
    cout << " This was the total distance traveled: " << startDistance << endl;
    cout << bigRedGraph.dijsktra(44137, 73704, startDistance);
    return 0;
}
