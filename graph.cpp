#include "graph.h"
#include <queue>
#include <algorithm>
#include <limits>
using namespace std;

// Constructor: create a graph with V vertices
Graph::Graph(int V) : V(V) {
    adj.resize(V);
}

// Add an undirected edge with weight
void Graph::addEdge(int u, int v, int w) {
    // Add edge u->v and v->u (bidirectional for metro connections)
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

// BFS for unweighted shortest path (by number of edges)
vector<int> Graph::BFS(int src, int dest) {
    vector<bool> visited(V, false);
    vector<int> pred(V, -1); // To store predecessor of each node for path reconstruction

    queue<int> q;
    visited[src] = true;
    q.push(src);

    // Standard BFS loop
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // Stop BFS when destination is found
        if (u == dest) {
            break;
        }

        for (auto& neighbor : adj[u]) {
            int v = neighbor.first;
            if (!visited[v]) {
                visited[v] = true;
                pred[v] = u;
                q.push(v);
            }
        }
    }

    // Reconstruct path from src to dest
    vector<int> path;
    if (!visited[dest]) {
        // No path found
        return path;
    }
    for (int at = dest; at != -1; at = pred[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

// Dijkstra's algorithm for weighted shortest path (by travel time)
pair<vector<int>, int> Graph::Dijkstra(int src, int dest) {
    const int INF = numeric_limits<int>::max();
    vector<int> dist(V, INF); // Distance from src to each vertex
    vector<int> pred(V, -1);  // To store path

    dist[src] = 0;
    // Min-heap priority queue: (distance, vertex)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push(make_pair(0, src));

    while (!pq.empty()) {
        auto it = pq.top();
        pq.pop();
        int currentDist = it.first, u = it.second;

        // If this distance is not up to date, skip
        if (currentDist > dist[u]) continue;

        // Early exit if we reached the destination
        if (u == dest) {
            break;
        }

        // Explore neighbors
        for (auto& neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pred[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Reconstruct path
    vector<int> path;
    if (dist[dest] == INF) {
        // No path found
        return make_pair(path, -1);
    }
    for (int at = dest; at != -1; at = pred[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return make_pair(path, dist[dest]);
}