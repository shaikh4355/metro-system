#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <string>
using namespace std;

// Graph class represents a metro network with stations (vertices) and connections (edges)
class Graph {
private:
    int V;  // Number of vertices (stations)
    // Adjacency list: each pair is (neighbor, weight)
    vector<vector<pair<int,int>>> adj;

public:
    // Constructor: initialize a graph with V vertices
    Graph(int V);

    // Add an undirected edge between u and v with weight w
    void addEdge(int u, int v, int w);

    // Perform BFS to find the shortest path (in terms of number of edges)
    // in the unweighted graph from src to dest. Returns the path as a list of vertex IDs.
    vector<int> BFS(int src, int dest);

    // Perform Dijkstra's algorithm to find the shortest weighted path (by travel time)
    // from src to dest. Returns a pair: (path, total_distance).
    pair<vector<int>, int> Dijkstra(int src, int dest);
};

#endif // GRAPH_H