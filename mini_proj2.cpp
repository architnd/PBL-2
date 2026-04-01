#include <iostream>
#include <vector>
#include <string>

using namespace std;

class vertex {
    int id;
    string type;
public:
    vertex() : id(0), type("default") {}
    void setId(int id) {
        this->id = id;
    }
    friend class graph;
};

class graph {
    double length_matrix[50][50];
    double energy_matrix[50][50];
    vertex v_arr[50];
    int energy_level = 10;
public:
    int n;
    graph();
    void create();
    void dijkstra(vertex v, double energy_consumed[50], int startv);
    void find_shortest_path(vertex start, vertex end);
};

graph::graph() {
    cout << "\nEnter Total vertices: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        v_arr[i].id = i;
        cout << "Enter type for vertex " << i << ": ";
        cin >> v_arr[i].type;
        for (int j = 0; j < n; j++) {
            length_matrix[i][j] = 0;
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
                cout << "Enter the length: ";
                cin >> length_matrix[i][j];
                length_matrix[j][i] = length_matrix[i][j];
                energy_matrix[i][j] = length_matrix[i][j] * 0.5;
                energy_matrix[j][i] = energy_matrix[i][j];
            }
        }
    }
}

void graph::dijkstra(vertex v,double energy_consumed[50], int startv) {
    //double dist[50];
    bool s[50];
    int src = v.id;
    const int INF = 999999; 

    for (int i = 0; i < n; i++) {
        s[i] = false;
        if (energy_matrix[src][i] == 0 && i != src) {
            energy_consumed[i] = INF;
        } else {
            energy_consumed[i] = energy_matrix[src][i];
        }
    }

    s[src] = true;
    energy_consumed[src] = 0;

    for (int j = 1; j < n; j++) {
        int min = INF;
        int u = -1;

        for (int k = 0; k < n; k++) {
            if (!s[k] && energy_consumed[k] <= min) {
                min = energy_consumed[k];
                u = k;
            }
        }

        if (u == -1) break;

        s[u] = true;

        for (int w = 0; w < n; w++) {
            if (!s[w] && energy_matrix[u][w] != 0 && energy_consumed[u] != INF) {
                if (energy_consumed[w] > energy_consumed[u] + energy_matrix[u][w]) {
                    energy_consumed[w] = energy_consumed[u] + energy_matrix[u][w];
                }
            }
        }
    }
    cout << "\nDijkstra's Algorithm:\n";
    cout << "\nVertex \t Energy Consumed \t Type\n";
    if (energy_consumed[startv] == INF) {
        cout << startv << " \t " << "INF" << " \t\t\t " << v_arr[startv].type << "\n";
    } else {
        cout << startv << " \t " << energy_consumed[startv] << " \t\t\t " << v_arr[startv].type << "\n";
    }
    for (int i = 0; i < n; i++) {
        if (energy_consumed[i] == INF) {
            cout << i << " \t " << "INF" << " \t\t\t " << v_arr[i].type << "\n";
        }
        else if(i != startv) {
            cout << i << " \t " << energy_consumed[i] << " \t\t\t " << v_arr[i].type << "\n";
        }
    }
}
void graph::find_shortest_path(vertex start, vertex end,double energy_consumed[50]) {
}
int main() {
    graph g;
    g.create();
    int x = g.n;
    vertex start_[x];
    for (int i = 0; i < x; i++) {
        start_[i].setId(i);
    }
    double energy_consumed[50][50];
    for (int i = 0; i < x; i++) {
        g.dijkstra(start_[i], energy_consumed[i], i);
    }
    return 0;
}