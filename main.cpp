#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <cctype>
#include <iomanip>
#include "graph.h"
using namespace std;

// Structure to store route history
struct RouteHistory {
    string srcName;
    string destName;
    string algorithm;
    string result;
    int cost;
    string timestamp;
};

// Function to check if a string is a number (ID input)
bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to get current timestamp
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto tm = *localtime(&time_t);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
    return string(buffer);
}

// Function to display route history
void displayHistory(const vector<RouteHistory>& history) {
    if (history.empty()) {
        cout << "\nNo route history available.\n" << endl;
        return;
    }

    cout << "\n=== ROUTE HISTORY ===" << endl;
    cout << left << setw(4) << "No." << setw(12) << "From" << setw(12) << "To"
         << setw(10) << "Algorithm" << setw(8) << "Time" << setw(12) << "Cost/Stops" << "Status" << endl;
    cout << string(70, '-') << endl;

    for (size_t i = 0; i < history.size(); ++i) {
        cout << left << setw(4) << (i+1) << setw(12) << history[i].srcName.substr(0,11)
             << setw(12) << history[i].destName.substr(0,11) << setw(10) << history[i].algorithm
             << setw(8) << history[i].timestamp << setw(12) << history[i].cost << history[i].result << endl;
    }
    cout << endl;
}

// Function to find nearby stations within maxStops distance
void findNearbyStations(Graph& metro, const map<int,string>& id2name, int stationId, int maxStops) {
    cout << "\n=== NEARBY STATIONS ===" << endl;
    cout << "Stations within " << maxStops << " stops from " << id2name.at(stationId) << ":" << endl;

    map<int, vector<int>> stationsByDistance;

    // Find shortest path from source to all other stations using BFS-like approach
    for (int dest = 0; dest < 50; ++dest) {
        if (dest == stationId) continue;

        vector<int> path = metro.BFS(stationId, dest);
        if (!path.empty()) {
            int distance = path.size() - 1; // Number of stops
            if (distance <= maxStops) {
                stationsByDistance[distance].push_back(dest);
            }
        }
    }

    if (stationsByDistance.empty()) {
        cout << "No stations found within " << maxStops << " stops." << endl;
        return;
    }

    for (int dist = 1; dist <= maxStops; ++dist) {
        if (stationsByDistance.find(dist) != stationsByDistance.end()) {
            cout << "\n" << dist << " stop" << (dist > 1 ? "s" : "") << " away:" << endl;
            for (int station : stationsByDistance[dist]) {
                cout << "  - " << id2name.at(station) << " (ID: " << station << ")" << endl;
            }
        }
    }
    cout << endl;
}

int main() {
    // Load station names from stations.txt
    ifstream fin("stations.txt");
    if (!fin) {
        cerr << "Error: stations.txt file not found." << endl;
        return 1;
    }

    map<int, string> id2name;
    map<string, int> name2id;
    int id;
    string name;
    while (fin >> id >> name) {
        id2name[id] = name;
        name2id[name] = id;
    }
    fin.close();

    // Create graph with 50 stations (0..49)
    Graph metro(50);

    // Hardcoded connections between stations (bidirectional)
    metro.addEdge(0, 1, 2);
    metro.addEdge(1, 2, 1);
    metro.addEdge(2, 3, 5);
    metro.addEdge(3, 4, 4);
    metro.addEdge(3, 14, 1);
    metro.addEdge(4, 5, 4);
    metro.addEdge(4, 13, 10);
    metro.addEdge(4, 24, 7);
    metro.addEdge(4, 41, 10);
    metro.addEdge(4, 45, 1);
    metro.addEdge(5, 6, 3);
    metro.addEdge(5, 14, 2);
    metro.addEdge(5, 48, 1);
    metro.addEdge(6, 7, 2);
    metro.addEdge(7, 8, 9);
    metro.addEdge(7, 9, 3);
    metro.addEdge(8, 9, 2);
    metro.addEdge(9, 10, 10);
    metro.addEdge(9, 23, 8);
    metro.addEdge(10, 11, 3);
    metro.addEdge(10, 22, 6);
    metro.addEdge(10, 40, 9);
    metro.addEdge(11, 12, 7);
    metro.addEdge(11, 22, 5);
    metro.addEdge(12, 13, 3);
    metro.addEdge(12, 19, 4);
    metro.addEdge(13, 14, 6);
    metro.addEdge(13, 42, 5);
    metro.addEdge(14, 15, 2);
    metro.addEdge(15, 16, 8);
    metro.addEdge(15, 46, 3);
    metro.addEdge(16, 17, 4);
    metro.addEdge(17, 18, 1);
    metro.addEdge(17, 24, 8);
    metro.addEdge(18, 19, 5);
    metro.addEdge(18, 35, 10);
    metro.addEdge(19, 20, 3);
    metro.addEdge(20, 21, 9);
    metro.addEdge(21, 22, 2);
    metro.addEdge(22, 23, 4);
    metro.addEdge(23, 24, 1);
    metro.addEdge(23, 36, 4);
    metro.addEdge(23, 40, 3);
    metro.addEdge(24, 25, 6);
    metro.addEdge(25, 26, 7);
    metro.addEdge(25, 38, 2);
    metro.addEdge(26, 27, 5);
    metro.addEdge(27, 28, 1);
    metro.addEdge(28, 29, 6);
    metro.addEdge(29, 30, 4);
    metro.addEdge(30, 31, 2);
    metro.addEdge(31, 32, 7);
    metro.addEdge(32, 33, 3);
    metro.addEdge(33, 34, 8);
    metro.addEdge(34, 35, 6);
    metro.addEdge(35, 36, 2);
    metro.addEdge(35, 47, 9);
    metro.addEdge(36, 37, 2);
    metro.addEdge(37, 38, 7);
    metro.addEdge(38, 39, 2);
    metro.addEdge(39, 40, 6);
    metro.addEdge(40, 41, 6);
    metro.addEdge(40, 44, 9);
    metro.addEdge(41, 42, 10);
    metro.addEdge(42, 43, 5);
    metro.addEdge(43, 44, 1);
    metro.addEdge(44, 45, 8);
    metro.addEdge(45, 46, 9);
    metro.addEdge(46, 47, 2);
    metro.addEdge(47, 48, 7);
    metro.addEdge(48, 49, 2);

    vector<RouteHistory> routeHistory;
    string choice;

    cout << "=== METRO NAVIGATION SYSTEM ===" << endl;

    while (true) {
        cout << "\n--- MAIN MENU ---" << endl;
        cout << "1. Find Route" << endl;
        cout << "2. View Route History" << endl;
        cout << "3. Find Nearby Stations" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        if (choice == "1") {
            // Original route finding functionality
            string srcInput, destInput;
            cout << "\nEnter source station (ID or name): ";
            cin >> srcInput;
            cout << "Enter destination station (ID or name): ";
            cin >> destInput;

            // Parse source ID
            int src, dest;
            bool validInput = true;

            if (isNumber(srcInput)) {
                src = stoi(srcInput);
                if (id2name.find(src) == id2name.end()) {
                    cerr << "Invalid station ID." << endl;
                    validInput = false;
                }
            } else {
                if (name2id.find(srcInput) == name2id.end()) {
                    cerr << "Invalid station name." << endl;
                    validInput = false;
                } else {
                    src = name2id[srcInput];
                }
            }

            // Parse destination ID
            if (validInput) {
                if (isNumber(destInput)) {
                    dest = stoi(destInput);
                    if (id2name.find(dest) == id2name.end()) {
                        cerr << "Invalid station ID." << endl;
                        validInput = false;
                    }
                } else {
                    if (name2id.find(destInput) == name2id.end()) {
                        cerr << "Invalid station name." << endl;
                        validInput = false;
                    } else {
                        dest = name2id[destInput];
                    }
                }
            }

            if (!validInput) continue;

            // Choose algorithm
            cout << "Enter 'BFS' for unweighted shortest path or 'Dijkstra' for weighted shortest path: ";
            string mode;
            cin >> mode;

            vector<int> path;
            int totalCost = 0;

            // Measure execution time
            auto startTime = chrono::high_resolution_clock::now();

            if (mode == "BFS" || mode == "bfs") {
                path = metro.BFS(src, dest);
            } else if (mode == "Dijkstra" || mode == "dijkstra") {
                auto result = metro.Dijkstra(src, dest);
                path = result.first;
                totalCost = result.second;
            } else {
                cerr << "Unknown mode selection." << endl;
                continue;
            }

            auto endTime = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

            // Store in history
            RouteHistory entry;
            entry.srcName = id2name[src];
            entry.destName = id2name[dest];
            entry.algorithm = (mode == "BFS" || mode == "bfs") ? "BFS" : "Dijkstra";
            entry.timestamp = getCurrentTimestamp();

            // Output results
            if (path.empty()) {
                cout << "No path found between source and destination." << endl;
                entry.result = "No path";
                entry.cost = 0;
            } else {
                cout << "\nShortest path: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << id2name[path[i]];
                    if (i != path.size() - 1) cout << " -> ";
                }
                cout << endl;

                if (mode == "BFS" || mode == "bfs") {
                    int stops = path.size() - 1;
                    cout << "Number of stops: " << stops << endl;
                    entry.cost = stops;
                    entry.result = "Found";
                } else {
                    cout << "Total travel time: " << totalCost << " minutes" << endl;
                    entry.cost = totalCost;
                    entry.result = "Found";
                }
                cout << "Execution time: " << duration << " ms" << endl;
            }

            routeHistory.push_back(entry);

        } else if (choice == "2") {
            // Display route history
            displayHistory(routeHistory);

        } else if (choice == "3") {
            // Find nearby stations
            string stationInput;
            int maxStops;

            cout << "\nEnter station (ID or name): ";
            cin >> stationInput;
            cout << "Enter maximum number of stops (1-10): ";
            cin >> maxStops;

            if (maxStops < 1 || maxStops > 10) {
                cout << "Invalid number of stops. Please enter 1-10." << endl;
                continue;
            }

            int stationId;
            bool validInput = true;

            if (isNumber(stationInput)) {
                stationId = stoi(stationInput);
                if (id2name.find(stationId) == id2name.end()) {
                    cerr << "Invalid station ID." << endl;
                    validInput = false;
                }
            } else {
                if (name2id.find(stationInput) == name2id.end()) {
                    cerr << "Invalid station name." << endl;
                    validInput = false;
                } else {
                    stationId = name2id[stationInput];
                }
            }

            if (validInput) {
                findNearbyStations(metro, id2name, stationId, maxStops);
            }

        } else if (choice == "4") {
            cout << "Thank you for using Metro Navigation System!" << endl;
            break;

        } else {
            cout << "Invalid choice. Please enter 1-4." << endl;
        }
    }

    return 0;
}