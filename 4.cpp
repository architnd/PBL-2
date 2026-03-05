#include <iostream>
using namespace std;

#define INF 99999 // A large number to represent infinity (no connection)

class Graph {
    int vertices;
    int cost[50][50]; // Adjacency matrix to store edge weights

public:
    // Constructor to initialize the graph
    Graph(int v) {
        vertices = v;
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                cost[i][j] = 0; // 0 means no edge
            }
        }
    }

    void createGraph() {
        int edges;
        cout << "Enter total number of edges: ";
        cin >> edges;

        cout << "Enter edges in format (source destination weight):\n";
        cout << "Note: Vertices should be between 0 and " << (vertices - 1) << "\n";
        
        for (int i = 0; i < edges; i++) {
            int u, v, weight;
            cout << "Edge " << (i + 1) << ": ";
            cin >> u >> v >> weight;

            // Check if vertices are valid
            if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
                cost[u][v] = weight;
                cost[v][u] = weight; // Assuming it's an undirected graph
            } else {
                cout << "Invalid vertices! Please try again.\n";
                i--; // Decrement counter to retry this edge
            }
        }
    }

    void displayGraph() {
        cout << "\nAdjacency Matrix (Edge Weights):\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                cout << cost[i][j] << "\t";
            }
            cout << "\n";
        }
    }

    void primsAlgorithm() {
        int visited[50] = {0}; // Track visited vertices (0 = unvisited, 1 = visited)
        visited[0] = 1;        // Start from vertex 0

        int totalCost = 0;
        int edgesCount = 0;

        cout << "\nMinimum Spanning Tree (MST) Edges:\n";
        
        // MST will always have exactly (vertices - 1) edges
        while (edgesCount < vertices - 1) {
            int minWeight = INF;
            int currentU = -1, currentV = -1;

            // Loop through all vertices
            for (int i = 0; i < vertices; i++) {
                // If the vertex 'i' is already visited
                if (visited[i] == 1) {
                    // Look at all its adjacent vertices 'j'
                    for (int j = 0; j < vertices; j++) {
                        // If 'j' is NOT visited, and there is an edge, and it's the smallest we've seen
                        if (visited[j] == 0 && cost[i][j] != 0 && cost[i][j] < minWeight) {
                            minWeight = cost[i][j];
                            currentU = i;
                            currentV = j;
                        }
                    }
                }
            }

            // If we found a valid minimum edge
            if (currentU != -1 && currentV != -1) {
                cout << "Edge: " << currentU << " - " << currentV << " | Weight: " << minWeight << "\n";
                visited[currentV] = 1; // Mark the new vertex as visited
                totalCost += minWeight;
                edgesCount++;
            } else {
                // If no edge is found but we haven't reached V-1 edges, the graph is disconnected
                cout << "Graph is disconnected! Cannot form a complete MST.\n";
                break;
            }
        }
        cout << "Total Minimum Cost: " << totalCost << "\n";
    }
};

int main() {
    int v, choice;
    cout << "Enter number of vertices in the graph (max 50): ";
    cin >> v;

    if (v <= 0 || v > 50) {
        cout << "Invalid number of vertices!\n";
        return 1;
    }

    Graph g(v);

    while (true) {
        cout << "\n=== Graph Operations Menu ===\n";
        cout << "1. Create Graph (Enter edges and weights)\n";
        cout << "2. Display Adjacency Matrix\n";
        cout << "3. Find Minimum Spanning Tree (Prim's Algorithm)\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                g.createGraph();
                break;
            case 2:
                g.displayGraph();
                break;
            case 3:
                g.primsAlgorithm();
                break;
            case 4:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
