//=================================================================
// CS 271 - Project 6
// Graph.cpp
// Fall 2025
// This is the implementation file for the Graph class
// TO-DO: Implement BFS, shortestPath, and asAdjMatrix methods
//=================================================================
#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <list>
#include <tuple>
#include <queue>
#include <climits>
#include "graph.h"
#include <functional>
using namespace std;
//=================================================================
// Default constructor
// Creates an empty graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::Graph ( )
{
    numV = 0;
    numE = 0;
    numOfDuplicates = 0;
}

//=================================================================
// Constructor
// Creates a graph with given vertices and edges
// Parameters:  keys - vector of vertex keys
//              data - vector of vertex data
//              edges - vector of edges as tuples(vertex1, vertex2, weight)
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::Graph ( vector<K> keys, vector<D> data, vector<tuple<K,K,int>> edges )
{
    numV = 0;
    numE = 0;
    numOfDuplicates = 0;
    for (int i = 0; i < keys.size(); i++)
        insertVertex(keys[i], data[i]);
    for (int j = 0; j < edges.size(); j++)
    {
        K v1 = get<0>(edges[j]);
        K v2 = get<1>(edges[j]);
        int w = get<2>(edges[j]);
        insertEdge(v1, v2, w);
    }
}

//=================================================================
// Destructor
// Destroys the graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::~Graph ( ) {}

//=================================================================
// isEdge
// Checks if there is an edge between two vertices
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
// Returns:     true if edge exists in graph, false otherwise
//=================================================================
template <class K, class D>
bool Graph<K,D>::isEdge ( K v1, K v2 ) const{
    if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end())
        throw invalid_argument("Error in isEdge: One or both vertices not found.");
    // get the adjacency list of v1
    const list<tuple<K, int>>& adj = vertices[v1].adj;
    // check if v2 is in the adjacency list
    for (const auto& edge : adj) {
        if (get<0>(edge) == v2)
            return true;
    }
    return false;
 }

//=================================================================
// getWeight
// Returns the weight of the edge between two vertices
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
// Returns:     weight of the edge if it exists, INT_MAX otherwise
//=================================================================
template <class K, class D>
int Graph<K,D>::getWeight ( K v1, K v2 ) const {
    if (!isEdge(v1, v2))
        return INT_MAX;
    // get the adjacency list of v1
    const list<tuple<K, int>>& adj = vertices[v1].adj;
    for (const auto& edge : adj) {
        if (get<0>(edge) == v2) // first element in tuple is the adjacent vertex key
            return get<1>(edge); // second element is the weight
    }
    return INT_MAX;
}

//=================================================================
// insertEdge
// Inserts an edge between two vertices with a given weight
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
//              w  - weight of the edge
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::insertEdge ( K v1, K v2, double w, string streetName )
{
    if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end()) {
        cout << v1 << "and " << v2 << "and" << numE << endl;
        throw invalid_argument("Error in insertEdge: One or both vertices not found.");
    }  
    // if edge already exists, update weight
    for (auto& edge : vertices[v1].adj) {
        if (get<0>(edge) == v2) {
            edge = make_tuple(v2, w);
            numOfDuplicates++;
            return;
        }
    }
    if (streetName.length() > 5) {
        vertices[v2].isStreet = true;
        vertices[v2].streetName = streetName;
    } else {
        vertices[v2].isStreet = false;
        vertices[v2].streetName = " ";
    }
    
    // otherwise, add new edge to adjacency list
    vertices[v1].adj.push_back(make_tuple(v2, w));
    numE++;

}

//=================================================================
// insertVertex
// Inserts a vertex with a given key and data
// Parameters:  key  - key of the vertex
//              data - data associated with the vertex
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::insertVertex ( K key, D data )
{
    if (vertices.find(key) != vertices.end()){
        // vertex already exists, so just update data
        vertices[key].data = data;
    }
    else {
        VertexInfo<K,D> newVertex;
        newVertex.data = data;
        vertices[key] = newVertex;
        numV++;
    }
}

//=================================================================
// toString
// Represents the graph as a string, each line includes the 
//      adjacency list for a vertex
// Parameters:  none
// Returns:     string representation of the graph
//=================================================================
template <class K, class D>
string Graph<K,D>::toString ( ) const
{
    stringstream ss;
    for (const auto& pair : vertices) {
        ss << pair.first << ": ";
        for (const auto& edge : pair.second.adj) {
            ss << "(" << get<0>(edge) << ", weight: " << get<1>(edge) << ") ";
        }
        ss << endl;
    }
    return ss.str();
}

//=================================================================
// DFSVisit
// Helper function for Depth-First Search
// Parameters:  u - the current vertex
//              time - the current time counter
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::DFSVisit ( K u, int& time )
{
    time++;
    vertices[u].d_time = time;
    vertices[u].color = 'g';
    for (const auto& edge : vertices[u].adj) {
        K v = get<0>(edge);
        if (vertices[v].color == 'w') {
            vertices[v].pre = &u;
            DFSVisit(v, time);
        }
    }
    vertices[u].color = 'b';
    time++;
    vertices[u].f_time = time;
}

//=================================================================
// DFS
// Performs Depth-First Search on the graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::DFS ( )
{
    // set all vertices to initial state (white, no predecessor)
    // in vertices map, .first is key, .second is VertexInfo
    for (auto& u : vertices) {
        u.second.color = 'w';
        u.second.pre = nullptr;
    }
    int time = 0; // initialize time counter
    // visit each vertex
    for (auto& u : vertices) {
        if (u.second.color == 'w') {
            DFSVisit(u.first, time);
        }
    }
}

//=================================================================
// topologicalSort
// Returns a string representing the topological sort of the graph
//  (keys in decreasing finishing time order)
//   Main idea: visit each vertex only after all dependencies are
//   complete (i.e., all vertices along path to it have been visited)
//   Requires graph to be a DAG (Directed Acyclic Graph)
// Parameters:  none
// Returns:     string representation of the topological sort
//=================================================================
template <class K, class D>
string Graph<K,D>::topologicalSort( ) //Sort by Decreasing Finish
{
    DFS();

    // get all the finishing times, store in a priority queue with respective keys
    // priority determined by finishing time (max has highest priority)
    priority_queue<pair<int, K>> finishTimes;
    for (const auto& pair : vertices) {
        finishTimes.push({pair.second.f_time, pair.first});
    }
    // build the result string
    // only include "->" between keys, no trailing arrow
    stringstream ss;
    while (!finishTimes.empty()) {
        ss << finishTimes.top().second;
        finishTimes.pop();
        if (!finishTimes.empty()) {
            ss << "->";
        }
    }
    return ss.str();
}
//=================================================================
// BFS
// Performs Breadth-First Search starting from the given source vertex
// Parameters:  source - the starting vertex for BFS
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::BFS ( K source )
{
    // TO-DO: Implement BFS algorithm
    for (auto& pair : vertices){
        pair.second.color = 'w'; // map
        pair.second.d = INT_MAX;
        pair.second.pre = nullptr;
    }
    vertices[source].color = 'g';
    vertices[source].d = 0;
    vertices[source].pre = nullptr;
    queue<K> myQueue;
    myQueue.push(source);
    while(!myQueue.empty()){
        K u = myQueue.front(); 
        myQueue.pop();
        for(auto& edge: vertices[u].adj){
            K v = get<0>(edge); //tuple
            
            if (vertices[v].color == 'w' ){
                vertices[v].color = 'g';
                vertices[v].d = vertices[u].d + 1;

                auto it = vertices.find(u);
                vertices[v].pre = const_cast<K*>(&(it->first));
                myQueue.push(v);

            }
        }
        vertices[u].color = 'b';
    }
    return;
}

//=================================================================
// shortestPath
// Finds the shortest path between two vertices using BFS results
// Parameters:  s - source vertex key
//              d - destination vertex key
// Returns:     string representation of the shortest path
//=================================================================
template <class K, class D>
string Graph<K,D>::shortestPath ( K s, K d, int& totalDistance)
{
    // TO-DO: Implement shortest path retrieval using BFS results
   // 1. Run BFS to fill 'pre' pointers
    BFS(s);
    if (vertices[d].d == INT_MAX) {
        return ""; 
    }
    function<void(K, stringstream&)> printPath = 
    [&](K v, stringstream& ss) -> void {
        if (v == s) {
            ss << "(" << get<0>(vertices[v].data) << "," << get<1>(vertices[v].data) << ") at " << vertices[v].streetName << endl;
        } else if (vertices[v].pre == nullptr){
            return;
        } else {
            printPath(*vertices[v].pre, ss);
            totalDistance = totalDistance + 1;
            ss << "(" << get<0>(vertices[v].data) << "," << get<1>(vertices[v].data) << ") at " << vertices[v].streetName << endl;
        }
    };
    stringstream ss;
    printPath(d, ss);
    return ss.str();
}

//=================================================================
// asAdjMatrix
// Returns the adjacency matrix representation of the graph
//   smallest key value corresponds to row/column 0, etc.
//   use weight value for edges, INT_MAX for no edge
// Parameters:  none
// Returns:     2D array (matrix) of edge weights
//=================================================================
template <class K, class D>
int** Graph<K,D>::asAdjMatrix() const
{
    int n = vertices.size();
    
    int** matrix = new int*[n]; // one element -> 1 row -> 1 element in a row
    // 1. Allocate 2D Array
    for(int i = 0; i < n; i++){
        matrix[i] = new int[n]; // create an dynamic array with n element (column)
        for(int j = 0; j < n; j ++){
            matrix[i][j] = INT_MAX;
        }
    }

    // 2. Map Keys to Indices in Row and Column of a key(0 to n-1)
    // Because map is sorted, iterating gives us alphabetical/numerical order
    map<K,int> keyToIndex;
    int index = 0; 
    for(const auto& pair : vertices){ 
        keyToIndex[pair.first] = index;
        index++;
    }

    int rowIndex = 0; 
    // 3. Fill Matrix
    for(const auto& pair: vertices){
        for(auto& edge : pair.second.adj){
            int colIndex = keyToIndex[get<0>(edge)];
            matrix[rowIndex][colIndex] = get<1>(edge);
        }
        rowIndex++; 

    }
    return matrix;
}

template <class K, class D>
void Graph<K,D>::changeVertexStatus (K key) {
    vertices[key].isStreet = true;
}


template <class K, class D>
string Graph<K,D>::dijsktra(K s, K d, int& totalDistance) {
    // This was my initialize process
    for (auto& [key, value] : vertices) {
        value.d = INT_MAX;
        value.pre = nullptr;
    }
    // I will make my sources distance = 0
    vertices[s].d = 0;
    priority_queue<pair<int, K>, vector<pair<int, K>>, greater<pair<int, K>>> pq;

    pq.push({0, s});

    while (!pq.empty()) {
        auto [dist, u] = pq.top();
        pq.pop();

        for (auto& [v, w] : vertices[u].adj) {
            if (vertices[u].d + w < vertices[v].d) {
                vertices[v].d = vertices[u].d + w;
                auto it = vertices.find(u);
                vertices[v].pre = const_cast<K*>(&(it->first));
                pq.push({vertices[v].d, v});
            }
        }
    }

    return dijsktraShortestHelper(s, d, totalDistance);
    
}

template <class K, class D>
string Graph<K,D>::dijsktraShortestHelper(K s, K d, int& totalDistance) {
    vector<tuple<double, double>> path; 


    if (vertices[d].d == INT_MAX) {
        cout << "There is no path " << endl;
        return "";
    }

totalDistance = 0; 
int timesRan = 0;
stringstream ss;

function<void(K)> printPath = [&](K v) {
    if (v == s) {
        if (vertices[v].isStreet == true || timesRan == 0) {
            ss << "(" << get<0>(vertices[v].data) << "," << get<1>(vertices[v].data) << ") at " << vertices[v].streetName << endl;
            path.push_back(vertices[v].data);
        }
        timesRan++;
    } 
    else if (vertices[v].pre == nullptr) {
        cout << "Our pre equals null ptr" << endl;
        return;
    } 
    else {
        printPath(*vertices[v].pre);

        for (auto& [neighbor, w] : vertices[*vertices[v].pre].adj) {
            if (neighbor == v) {
                totalDistance += w;
                break;
            }
        }
        if (vertices[v].isStreet == true) {
            path.push_back(vertices[v].data);
            ss << "(" << get<0>(vertices[v].data) << "," << get<1>(vertices[v].data) << ") at " << vertices[v].streetName << endl;
        }

        timesRan++;
    }
};
    printPath(d); 
    
    string myString = "";
    for (auto& coord : path) {
        myString += "(" + to_string(get<0>(coord)) + "," + to_string(get<1>(coord)) + ")\n";
    }
    myString += "With a total distance of " + std::to_string(totalDistance) + "\n";    // The string stream does not work so I did path instead.
    ss << "Total distance at " << totalDistance << endl;
    return ss.str(); 
}


template <class K, class D>
string Graph<K,D>::turnMyTupleToString(const tuple<double, double>& myCoordinates) {
    std::stringstream ss;
    ss << "(" << std::get<0>(myCoordinates) << ", " << std::get<1>(myCoordinates) << ")";
    return ss.str();
}






































