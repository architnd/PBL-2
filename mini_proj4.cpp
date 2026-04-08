#include <iostream>
#include <string>

using namespace std;

// Represents a single node on the map (house, pad, intersection)
class vertex {
    int id; 
    string type; 
public:
    vertex() : id(0), type("default") {}
    
    // Assigns ID and type to the node
    void set(int id, string type) {
        this->id = id;
        this->type = type;
    }
    
    string getType() const { return type; }
    
    friend class graph;
};

// Manages the map using fixed 50x50 arrays
class graph {
    double length_matrix[50][50]; 
    double energy_matrix[50][50];
    vertex v_arr[50];

public:
    int n; 
    
    graph() { n = 0; }
    
    void create_fixed_graph();
    void add_vertex();
    int select_houses(int houses_arr[]); 
    void reset_houses();
    void simulate_delivery(const int houses[], int num_houses);
    
private:
    void dijkstra_helper(int src, double dist[50], int parent[50]);
    void print_path(int src, int dest, const int parent[50]);
    void add_edge(int u, int v, double length);
};

// Connects two nodes and automatically calculates energy cost
void graph::add_edge(int u, int v, double length) {
    length_matrix[u][v] = length;
    length_matrix[v][u] = length;
    
    energy_matrix[u][v] = length * 0.5;
    energy_matrix[v][u] = length * 0.5;
}

// Initializes the default 8-node map with predetermined connections
void graph::create_fixed_graph() {
    n = 8;
    
    // Clear matrices
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            length_matrix[i][j] = 0.0;
            energy_matrix[i][j] = 0.0;
        }
    }

    // Set predefined node types
    v_arr[0].set(0, "warehouse");
    v_arr[1].set(1, "energy pad");
    v_arr[2].set(2, "energy pad");
    v_arr[3].set(3, "energy pad");
    v_arr[4].set(4, "intersection");
    v_arr[5].set(5, "intersection");
    v_arr[6].set(6, "intersection");
    v_arr[7].set(7, "intersection");

    // Map out roads and distances
    add_edge(0, 4, 40); 
    add_edge(4, 1, 20); 
    add_edge(4, 5, 50); 
    add_edge(5, 2, 10); 
    add_edge(5, 6, 60);
    add_edge(6, 3, 30); 
    add_edge(6, 7, 40);
    add_edge(7, 0, 70);
    add_edge(4, 7, 90); 

    cout << "\n[+] Fixed graph generated successfully with 8 vertices.\n";
}

// Allows user to dynamically add a new intersection and link it to the map
void graph::add_vertex() {
    if (n >= 50) {
        cout << "\n[!] ERROR: Maximum capacity of 50 vertices reached.\n";
        return;
    }
    
    int id = n; 
    n++;        
    v_arr[id].set(id, "intersection");
    
    // Clear new rows/cols for the added node
    for (int i = 0; i < n; i++) {
        length_matrix[id][i] = 0.0;
        length_matrix[i][id] = 0.0;
        energy_matrix[id][i] = 0.0;
        energy_matrix[i][id] = 0.0;
    }
    
    cout << "\n[+] Adding new vertex " << id << " (Default type: intersection)\n";
    
    int edges;
    cout << "How many existing vertices is this connected to? ";
    cin >> edges;
    
    // Link new node based on user input
    for (int i = 0; i < edges; i++) {
        int target;
        double dist;
        cout << "Enter target vertex ID and distance (e.g., 4 30): ";
        cin >> target >> dist;
        
        if (target >= 0 && target < n - 1) { 
            add_edge(id, target, dist);
        } else {
            cout << "Invalid vertex ID!\n";
        }
    }
    cout << "[+] Vertex " << id << " added successfully.\n";
}

// Prompts user to pick intersections to act as delivery targets
int graph::select_houses(int houses_arr[]) {
    int count;
    int valid_count = 0;
    cout << "How many intersections do you want to set as houses? ";
    cin >> count;
    
    if(count > 50) {
        count = 50;
    }
    
    // Process user selections and populate array
    for (int i = 0; i < count; i++) {
        int id;
        cout << "Enter vertex ID for house " << i + 1 << ": ";
        cin >> id;
        
        if (id >= 0 && id < n && v_arr[id].getType() == "intersection") {
            v_arr[id].type = "house"; 
            houses_arr[valid_count] = id; 
            valid_count++;
            cout << "Vertex " << id << " is now a house.\n";
        } else {
            cout << "Invalid ID or not an intersection. Try again.\n";
            i--; 
        }
    }
    return valid_count; 
}

// Reverts all house nodes back to normal intersections
void graph::reset_houses() {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (v_arr[i].getType() == "house") {
            v_arr[i].type = "intersection"; 
            count++;
        }
    }
    cout << " Reset " << count << " houses back to intersections.\n";
}

// Finds shortest paths from a source to all nodes, storing distances and routes
void graph::dijkstra_helper(int src, double dist[50], int parent[50]) {
    bool visited[50];

    // Initialize arrays to represent unvisited, disconnected state
    for(int i = 0; i < n; i++) {
        dist[i] = 9999.0;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[src] = 0.0;

    // Main loop to lock in shortest paths node by node
    for (int i = 0; i < n - 1; i++) {
        double min_dist = 9999.0; 
        int u = -1;

        // Find the closest unvisited node
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        // Check all neighbors to find cheaper shortcuts (Relaxation)
        for (int v = 0; v < n; v++) {
            if (!visited[v] && energy_matrix[u][v] > 0) {
                if (dist[u] + energy_matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + energy_matrix[u][v];
                    parent[v] = u; 
                }
            }
        }
    }
}

// Traces the parent array backward to print the driving route
void graph::print_path(int src, int dest, const int parent[50]) {
    if (src == dest) return;
    
    int path[50];
    int path_len = 0;
    int curr = dest;
    
    // Trace backward from destination to source
    while (curr != -1) {
        path[path_len] = curr;
        path_len++;
        if (curr == src) break;
        curr = parent[curr]; 
    }
    
    cout << "    Route: ";
    // Print forward
    for (int i = path_len - 1; i >= 0; i--) {
        cout << path[i];
        if (i != 0) {
            cout << " -> ";
        }
    }
    cout << "\n";
}

// Manages delivery routing, ensuring the EV always has enough battery for a safe trip
void graph::simulate_delivery(const int houses[], int num_houses) {
    if (num_houses == 0) {
        cout << "No houses selected for delivery!\n";
        return;
    }

    double current_energy = 100.0;
    int current_node = 0; 

    // Build the manifest and append the warehouse (0) as the final forced stop
    int route_targets[50];
    for(int i = 0; i < num_houses; i++) {
        route_targets[i] = houses[i];
    }
    route_targets[num_houses] = 0; 
    int total_targets = num_houses + 1;

    cout << "\nSTARTING EV DELIVERY SIMULATION:\n";
    cout << "Initial Energy: 100.0 | Location: Warehouse (0)\n\n";

    // Process each delivery target in the manifest
    for (int t = 0; t < total_targets; t++) {
        int target = route_targets[t];
        
        // Format display name
        string target_name;
        if (target == 0) {
            target_name = "Warehouse (0)";
        } else {
            target_name = "House (" + to_string(target) + ")";
        }
        
        cout << "Next Delivery Target: " << target_name << "\n";

        // Loop continues if a charging detour is taken before reaching target
        while (current_node != target) {
            
            // Phase 1: Calculate energy needed for the direct route
            double dist_from_curr[50];
            int parent_from_curr[50];
            dijkstra_helper(current_node, dist_from_curr, parent_from_curr);

            double E_to_target = dist_from_curr[target];

            // Phase 2: Calculate buffer energy needed to reach safety after delivery
            double E_target_to_pad = 9999.0;
            
            if (v_arr[target].getType() == "warehouse") {
                E_target_to_pad = 0.0;
            } else {
                double dist_from_target[50];
                int temp_parent[50];
                dijkstra_helper(target, dist_from_target, temp_parent);
                
                for (int i = 0; i < n; i++) {
                    if (v_arr[i].getType() == "energy pad" || v_arr[i].getType() == "warehouse") {
                        if(dist_from_target[i] < E_target_to_pad) {
                            E_target_to_pad = dist_from_target[i];
                        }
                    }
                }
            }

            // Phase 3: Decide between direct trip or detour
            if (current_energy >= E_to_target + E_target_to_pad) {
                // Execute direct route and deduct energy
                cout << "    [SUCCESS] Enough energy to reach " << target_name << " directly.\n";
                print_path(current_node, target, parent_from_curr);
                
                current_energy -= E_to_target;
                current_node = target; 
                cout << "    Arrived at " << target_name << ". Remaining Energy: " << current_energy << "\n\n";
            } else {
                // Scan for the most efficient reachable charging pad
                cout << "    [WARNING] Insufficient energy for round-trip safety. Locating nearest reachable Energy Pad...\n";
                int best_pad = -1;
                double min_detour_cost = 9999.0;

                for (int i = 0; i < n; i++) {
                    if (v_arr[i].getType() == "energy pad" || v_arr[i].getType() == "warehouse") {
                        if (dist_from_curr[i] <= current_energy) {
                            
                            double dist_from_pad[50];
                            int p_pad[50];
                            dijkstra_helper(i, dist_from_pad, p_pad);
                            
                            if (100.0 >= dist_from_pad[target] + E_target_to_pad) {
                                double detour_cost = dist_from_curr[i] + dist_from_pad[target];
                                if (detour_cost < min_detour_cost) {
                                    min_detour_cost = detour_cost;
                                    best_pad = i; 
                                }
                            }
                        }
                    }
                }

                if (best_pad == -1) {
                    cout << "\n[CRITICAL FAILURE] EV is stranded. No reachable energy pad can salvage this route.\n";
                    return;
                }

                // Execute detour and fully recharge battery
                cout << "    Detouring to " << v_arr[best_pad].getType() << " (" << best_pad << ").\n";
                print_path(current_node, best_pad, parent_from_curr);
                
                current_node = best_pad;
                current_energy = 100.0; 
                cout << "    [CHARGED] Battery restored to 100.0 at node " << best_pad << ".\n\n";
            }
        }
    }
    cout << "\nROUTE COMPLETE! EV returned to Warehouse.\n";
}

int main() {
    graph g;
    g.create_fixed_graph(); 
    
    int choice;
    int current_houses[50]; 
    int num_houses = 0;     
    
    // Main system menu loop
    while (true) {
        cout << "\n--- EV Delivery System Menu ---\n";
        cout << "1. Add a vertex\n";
        cout << "2. Select intersections as houses\n";
        cout << "3. Run Delivery Route Simulation\n";
        cout << "4. Reset houses to intersections\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        // Execute user selection
        if (choice == 1) {
            g.add_vertex();
        } else if (choice == 2) {
            num_houses = g.select_houses(current_houses);
        } else if (choice == 3) {
            g.simulate_delivery(current_houses, num_houses);
        } else if (choice == 4) {
            g.reset_houses();
            num_houses = 0; 
        } else if (choice == 5) {
            cout << "Exiting system...\n";
            break; 
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}