#include <iostream>
using namespace std;

class vertex {
    int id;
public:
    vertex() : id(0) {}
    friend class graph;
};

class graph {
    int n;
    int cost[50][50];
    int t[50][3];
    int edges_in_t;
    int min_cost;
    vertex v_arr[50];
public:
    graph();
    void create();
    void display_initial();
    void display_t_matrix();
    void prims(int v);
};

graph::graph() {
    cout << "\nEnter Total vertices: ";
    cin >> n;
    min_cost = 0;
    edges_in_t = 0;
    
    for (int i = 0; i < n; i++) {
        v_arr[i].id = i;
        for (int j = 0; j < n; j++) {
            cost[i][j] = 999;
        }
    }
}

void graph::create() {
    char c;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cout << "\nDo you have an edge between vertex " 
                 << i << " and vertex " << j << "? (y/n): ";
            cin >> c;
            if (c == 'y' || c == 'Y') {
                cout << "Enter the cost: ";
                cin >> cost[i][j];
                cost[j][i] = cost[i][j];
            }
        }
    }
}

void graph::prims(int v) {
    int nearest[50];
    edges_in_t = 0;
    min_cost = 0;
    
    for (int i = 0; i < n; i++) {
        if (i == v)
            nearest[i] = -1;
        else
            nearest[i] = v;
    }

    for (int i = 0; i < n - 1; i++) {
        int min = 999;
        int j = -1;

        for (int k = 0; k < n; k++) {
            if (nearest[k] != -1 && cost[k][nearest[k]] < min) {
                min = cost[k][nearest[k]];
                j = k;
            }
        }
        
        if (j == -1) break;

        t[edges_in_t][0] = nearest[j];
        t[edges_in_t][1] = j;
        t[edges_in_t][2] = min;

        min_cost += min;
        edges_in_t++;

        nearest[j] = -1;

        for (int k = 0; k < n; k++) {
            if (nearest[k] != -1 && cost[k][j] < cost[k][nearest[k]]) {
                nearest[k] = j;
            }
        }
    }
    
    cout << "\nPrim's Algorithm computed successfully.\n";
}

void graph::display_initial() {
    cout << "\nInitial Adjacency Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << cost[i][j] << "\t";
        }
        cout << endl;
    }
}

void graph::display_t_matrix() {
    if (edges_in_t == 0) {
        cout << "\nSpanning tree not computed yet. Please run Prim's Algorithm first.\n";
        return;
    }

    cout << "\nSpanning Tree Edges (t matrix):\n";
    cout << "Src\tDest\tCost\n";
    for (int i = 0; i < edges_in_t; i++) {
        cout << t[i][0] << "\t" << t[i][1] << "\t" << t[i][2] << endl;
    }
    cout << "\nTotal cost = " << min_cost << endl;
}

int main() {
    graph g;
    int choice, v;

    while (true) {
        cout << "\n=== GRAPH OPERATIONS MENU ===\n";
        cout << "1. Create Graph\n";
        cout << "2. Display Initial Cost Adjacency Matrix\n";
        cout << "3. Use Prim's Algorithm\n";
        cout << "4. Display Spanning Tree Edges (t matrix) & Cost\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                g.create();
                break;
            case 2:
                g.display_initial();
                break;
            case 3:
                cout << "\nEnter the starting vertex: ";
                cin >> v;
                g.prims(v);
                break;
            case 4:
                g.display_t_matrix();
                break;
            case 5:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}