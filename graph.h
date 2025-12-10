//=================================================================
// CS 271 - Project 6
// Graph.h
// Fall 2025
// This is the declaration file for the Graph class
//=================================================================

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <list>
#include <tuple>
using namespace std;

template <class K, class D>
struct VertexInfo
{
    D                     data;
    list<tuple<K, int>>    adj; // adjacency list

    // attributes filled in during BFS/DFS
    int                      d; // distance from source
    K*                      pre; // predecessor in search
    char                 color; // 'w', 'g', 'b'
    
    int                 d_time; // discovery time in DFS
    int                 f_time; // finishing time in DFS
    bool                isStreet = false; // Automatically make all vertexes false
    string              streetName;
    
};

template <class K, class D>
class Graph
{
private:
   map<K, VertexInfo<K,D>> vertices;    // mapping between vertex key and vertex info
   void     DFSVisit    ( K u, int& time ); // helper for DFS
public:
   int                         numV;    // number of vertices
   int                         numE;    // number of edges
   int                         numOfDuplicates;
            Graph          ( );
            Graph          ( vector<K> keys, vector<D> data, vector<tuple<K,K,int>> edges );
           ~Graph          ( );

   bool    isEdge          ( K v1, K v2 ) const;
   int     getWeight       ( K v1, K v2 ) const;
   void    insertEdge      ( K v1, K v2, double w, string streetName);
   void    insertVertex    ( K key, D data );
   int     size            ( ) {return numV;}
   string  toString        ( ) const;
   void    DFS             ( );
   string  topologicalSort ( );
   void    BFS             ( K source );
   string  shortestPath    ( K s, K d, int& totalDistance);
   int**   asAdjMatrix     ( ) const;
   void    changeVertexStatus (K key);
   string  dijsktra(K s, K d, int& totalDistance);
   string  dijsktraShortestHelper(K s, K d, int& totalDistance);
   string  turnMyTupleToString(const tuple<double, double>& myCoordinates);
};
#include "graph.cpp"
#endif