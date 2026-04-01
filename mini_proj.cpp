#include<iostream>
#include<vector>
using namespace std;

class vertex {
    int id;
    string type;
public:
    vertex() : id(0), type("default") {}
    friend class graph;
};
class graph {
    int n;
    int length[50][50];
    vertex v_arr[50];
    double battery = 100.0;
    double consumption_rate = 0.5;
public:
    graph();
    void create();
    void shortest(vertex v);
    void dijkstra (vertex v);
};
graph::graph() {
    cout << "\nEnter Total vertices: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        v_arr[i].id = i;
        cout << "Enter type for vertex " << i << ": ";
        cin >> v_arr[i].type;
        for (int j = 0; j < n; j++) {
            length[i][j] = 0;
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
                cin >> length[i][j];
                length[j][i] = length[i][j];
            }
        }
    }
}

