#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ==========================================
// CLASS: vertex
// Represents a single node on the map (warehouse, house, pad, intersection)
// ==========================================
class vertex {
    int id;
    string type;
public:
    // Default constructor initializes ID to 0 and type to default
    vertex() : id(0), type("default") {}
    
    // Helper method to easily set a vertex's properties
    void set(int id, string type) {
        this->id = id;
        this->type = type;
    }
    
    // Getter for the type string
    string getType() const { return type; }
    
    // Allows the graph class to access private members of vertex directly
    friend class graph;
};

// ==========================================
// CLASS: graph
// Manages the map, routing algorithms, and EV simulation
// ==========================================
class graph {
    // Dynamic 2D vectors for adjacency matrices. 
    // Vector allows the map to grow infinitely without crashing.
    vector<vector<double>> length_matrix; 
    vector<vector<double>> energy_matrix;
    
    // 1D vector holding all vertex objects
    vector<vertex> v_arr;

public:
    int n; // Total number of active vertices
    
    // Constructor initializes graph with 0 vertices
    graph() { n = 0; }
    
    // Public methods called by the main menu
    void create_fixed_graph();
    void add_vertex();
    vector<int> select_houses();
    void reset_houses();
    void simulate_delivery(const vector<int>& houses);
    
private:
    // Internal helper methods
    void resize_matrices(int new_size);
    void dijkstra_helper(int src, vector<double>& dist, vector<int>& parent);
    void print_path(int src, int dest, const vector<int>& parent);
    void add_edge(int u, int v, double length);
};

// ==========================================
// METHOD: resize_matrices
// Expands the 2D grid dynamically when a new vertex is added
// ==========================================
void graph::resize_matrices(int new_size) {
    length_matrix.resize(new_size);
    energy_matrix.resize(new_size);
    
    // Ensure every row inside the 2D vector is also resized to the new width
    // New cells are initialized with 0.0
    for (int i = 0; i < new_size; i++) {
        length_matrix[i].resize(new_size, 0.0);
        energy_matrix[i].resize(new_size, 0.0);
    }
}

// ==========================================
// METHOD: add_edge
// Connects two nodes in the adjacency matrices
// ==========================================
void graph::add_edge(int u, int v, double length) {
    // Undirected graph: link u to v, AND link v to u
    length_matrix[u][v] = length;
    length_matrix[v][u] = length;
    
    // Energy cost is 50% of the distance length
    energy_matrix[u][v] = length * 0.5;
    energy_matrix[v][u] = length * 0.5;
}

// ==========================================
// METHOD: create_fixed_graph
// Generates the initial 8-node map for testing
// ==========================================
void graph::create_fixed_graph() {
    n = 8;
    
    // Size vectors exactly to our starting node count (8x8 grids)
    v_arr.resize(n);
    resize_matrices(n);

    // Set predefined vertices based on the hardcoded map design
    v_arr[0].set(0, "warehouse");
    v_arr[1].set(1, "energy pad");
    v_arr[2].set(2, "energy pad");
    v_arr[3].set(3, "energy pad");
    v_arr[4].set(4, "intersection");
    v_arr[5].set(5, "intersection");
    v_arr[6].set(6, "intersection");
    v_arr[7].set(7, "intersection");

    // Connect the nodes with edges and distances
    add_edge(0, 4, 40); 
    add_edge(4, 1, 20); 
    add_edge(4, 5, 50); 
    add_edge(5, 2, 10); 
    add_edge(5, 6, 60);
    add_edge(6, 3, 30); 
    add_edge(6, 7, 40);
    add_edge(7, 0, 70);
    add_edge(4, 7, 90); 

    cout << "\n[+] Fixed graph generated successfully with 8 vertices (1 Warehouse, 3 Pads, 4 Intersections).\n";
}

// ==========================================
// METHOD: add_vertex
// Dynamically adds a new node to the map
// ==========================================
void graph::add_vertex() {
    int id = n; // The new ID will be the current total size
    n++;        // Increment total vertex count
    
    // Add an empty vertex object to the list
    v_arr.push_back(vertex());
    
    // Automatically default its type to an intersection to save time
    v_arr[id].set(id, "intersection");
    
    // Grow the 2D matrices by 1 column and 1 row to accommodate the new node
    resize_matrices(n); 
    
    cout << "\n[+] Adding new vertex " << id << " (Default type: intersection)\n";
    
    int edges;
    cout << "How many existing vertices is this connected to? ";
    cin >> edges;
    
    // Loop to let the user connect this new node to existing nodes
    for (int i = 0; i < edges; i++) {
        int target;
        double dist;
        cout << "Enter target vertex ID and distance (e.g., 4 30): ";
        cin >> target >> dist;
        
        // Validation check to ensure the target actually exists
        if (target >= 0 && target < n - 1) { 
            add_edge(id, target, dist); // Link them
        } else {
            cout << "Invalid vertex ID!\n";
        }
    }
    cout << "[+] Vertex " << id << " added successfully.\n";
}

// ==========================================
// METHOD: select_houses
// Converts generic intersections into target delivery houses
// ==========================================
vector<int> graph::select_houses() {
    vector<int> houses;
    int count;
    cout << "How many intersections do you want to set as houses? ";
    cin >> count;
    
    for (int i = 0; i < count; i++) {
        int id;
        cout << "Enter vertex ID for house " << i + 1 << ": ";
        cin >> id;
        
        // Ensure ID is valid AND that we don't accidentally overwrite a pad/warehouse
        if (id >= 0 && id < n && v_arr[id].getType() == "intersection") {
            v_arr[id].type = "house"; // Change its identity
            houses.push_back(id);     // Add to our delivery list
            cout << "Vertex " << id << " is now a house.\n";
        } else {
            cout << "Invalid ID or not an intersection. Try again.\n";
            i--; // Decrement i to force the user to retry this specific house
        }
    }
    return houses; // Return the list of targeted houses
}

// ==========================================
// METHOD: reset_houses
// Reverts all houses back to normal intersections
// ==========================================
void graph::reset_houses() {
    int count = 0;
    // Scan the whole map for houses
    for (int i = 0; i < n; i++) {
        if (v_arr[i].getType() == "house") {
            v_arr[i].type = "intersection"; // Revert it
            count++;
        }
    }
    cout << "[+] Reset " << count << " houses back to intersections.\n";
}

// ==========================================
// METHOD: dijkstra_helper
// Finds the shortest path from a starting node to ALL other nodes based on ENERGY cost.
// Populates the 'dist' (distances) and 'parent' (routing path) vectors.
// ==========================================
void graph::dijkstra_helper(int src, vector<double>& dist, vector<int>& parent) {
    // Initialize distances to an artificial infinity (9999.0)
    dist.assign(n, 9999.0);
    // Initialize parent tracking to -1 (meaning no path yet)
    parent.assign(n, -1);
    // Keep track of nodes we have already fully optimized
    vector<bool> visited(n, false);

    // Distance from source to source is always 0
    dist[src] = 0;

    // Run the algorithm for every node
    for (int i = 0; i < n - 1; i++) {
        double min_dist = 9999.0; 
        int u = -1;

        // Step 1: Find the unvisited node with the smallest known distance
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }

        // If no reachable nodes are left, break out early
        if (u == -1) break;
        
        // Mark this node as optimized/locked
        visited[u] = true;

        // Step 2: Update the distances of all neighbors of the current node 'u'
        for (int v = 0; v < n; v++) {
            // If connected AND unvisited...
            if (!visited[v] && energy_matrix[u][v] > 0) {
                // If the path jumping through 'u' is better than 'v's current path...
                if (dist[u] + energy_matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + energy_matrix[u][v]; // Update shorter distance
                    parent[v] = u; // Record that we got to 'v' via 'u'
                }
            }
        }
    }
}

// ==========================================
// METHOD: print_path
// Traces backward through the parent array to print the chronological route
// ==========================================
void graph::print_path(int src, int dest, const vector<int>& parent) {
    if (src == dest) return;
    
    vector<int> path;
    int curr = dest;
    
    // Trace backward from destination up to the source
    while (curr != -1) {
        path.push_back(curr);
        if (curr == src) break;
        curr = parent[curr]; // Jump to the node that brought us here
    }
    
    // Print the path in reverse order (so it reads Source -> Destination)
    cout << "    Route: ";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << path[i] << (i == 0 ? "" : " -> ");
    }
    cout << "\n";
}

// ==========================================
// METHOD: simulate_delivery
// Core logic for managing the EV's energy, routes, and charging detours
// ==========================================
void graph::simulate_delivery(const vector<int>& houses) {
    if (houses.empty()) {
        cout << "No houses selected for delivery!\n";
        return;
    }

    double current_energy = 100.0;
    int current_node = 0; // Starts at warehouse

    // Compile the master route list: all houses, ending back at warehouse
    vector<int> route_targets = houses;
    route_targets.push_back(0); 

    cout << "\n=== STARTING EV DELIVERY SIMULATION ===\n";
    cout << "Initial Energy: 100.0 | Location: Warehouse (0)\n\n";

    // Loop through every destination on the manifest
    for (size_t t = 0; t < route_targets.size(); t++) {
        int target = route_targets[t];
        string target_name = (target == 0) ? "Warehouse (0)" : "House (" + to_string(target) + ")";
        cout << ">>> Next Delivery Target: " << target_name << "\n";

        // While loop is used because we might detour before reaching the target.
        // It keeps trying to reach the target until current_node == target.
        while (current_node != target) {
            
            // Phase 1: Calculate routes from WHERE WE ARE right now
            vector<double> dist_from_curr;
            vector<int> parent_from_curr;
            dijkstra_helper(current_node, dist_from_curr, parent_from_curr);

            // Cost just to reach the immediate target
            double E_to_target = dist_from_curr[target];

            // Phase 2: Calculate "Safety Buffer" (Energy needed AFTER reaching target)
            double E_target_to_pad = 9999.0;
            
            // If target is warehouse, no safety buffer needed (we are home!)
            if (v_arr[target].getType() == "warehouse") {
                E_target_to_pad = 0;
            } else {
                // Otherwise, simulate paths FROM the target to find the closest charger
                vector<double> dist_from_target;
                vector<int> temp_parent;
                dijkstra_helper(target, dist_from_target, temp_parent);
                
                // Scan to find the minimum distance to ANY pad or the warehouse
                for (int i = 0; i < n; i++) {
                    if (v_arr[i].getType() == "energy pad" || v_arr[i].getType() == "warehouse") {
                        E_target_to_pad = min(E_target_to_pad, dist_from_target[i]);
                    }
                }
            }

            // Phase 3: DECISION LOGIC
            // Check if our battery holds enough for: (Trip there) + (Safety trip to charger)
            if (current_energy >= E_to_target + E_target_to_pad) {
                // We have enough energy! Drive directly to target.
                cout << "    [SUCCESS] Enough energy to reach " << target_name << " directly.\n";
                print_path(current_node, target, parent_from_curr);
                
                // Update EV stats
                current_energy -= E_to_target;
                current_node = target; // We arrived, which breaks the while-loop
                
                cout << "    Arrived at " << target_name << ". Remaining Energy: " << current_energy << "\n\n";
            } else {
                // Danger! Not enough energy for round trip. We must detour to charge.
                cout << "    [WARNING] Insufficient energy for round-trip safety. Locating nearest reachable Energy Pad...\n";
                
                int best_pad = -1;
                double min_detour_cost = 9999.0;

                // Scan map for the optimal charging location
                for (int i = 0; i < n; i++) {
                    if (v_arr[i].getType() == "energy pad" || v_arr[i].getType() == "warehouse") {
                        // Ensure we actually have enough juice just to reach THIS pad
                        if (dist_from_curr[i] <= current_energy) {
                            
                            // Simulate paths FROM this pad to ensure it actually helps us
                            vector<double> dist_from_pad;
                            vector<int> p_pad;
                            dijkstra_helper(i, dist_from_pad, p_pad);
                            
                            // Check if a fully charged battery (100.0) from THIS pad can reach target safely
                            if (100.0 >= dist_from_pad[target] + E_target_to_pad) {
                                // Calculate total cost of this detour to find the most efficient one
                                double detour_cost = dist_from_curr[i] + dist_from_pad[target];
                                if (detour_cost < min_detour_cost) {
                                    min_detour_cost = detour_cost;
                                    best_pad = i; // Save the best option
                                }
                            }
                        }
                    }
                }

                // If no pad is reachable or viable, the mission fails
                if (best_pad == -1) {
                    cout << "\n[CRITICAL FAILURE] EV is stranded. No reachable energy pad can salvage this route.\n";
                    return;
                }

                // Execute the detour
                cout << "    Detouring to " << v_arr[best_pad].getType() << " (" << best_pad << ").\n";
                print_path(current_node, best_pad, parent_from_curr);
                
                // Update EV stats to reflect arriving at the pad
                current_node = best_pad;
                current_energy = 100.0; // Fully charged!
                cout << "    [CHARGED] Battery restored to 100.0 at node " << best_pad << ".\n\n";
                
                // Notice we do NOT break the while loop.
                // The loop restarts, but now 'current_node' is the pad, and it will try for the target again.
            }
        }
    }
    cout << "=== ROUTE COMPLETE! EV returned to Warehouse. ===\n";
}

// ==========================================
// MAIN FUNCTION
// Drives the user menu interface
// ==========================================
int main() {
    graph g;
    g.create_fixed_graph(); // Initialize the map instantly
    int choice;
    vector<int> current_houses; // Tracks which nodes are currently delivery targets
    
    // Infinite menu loop
    while (true) {
        cout << "\n--- EV Delivery System Menu ---\n";
        cout << "1. Add a vertex\n";
        cout << "2. Select intersections as houses\n";
        cout << "3. Run Delivery Route Simulation\n";
        cout << "4. Reset houses to intersections\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            g.add_vertex();
        } else if (choice == 2) {
            current_houses = g.select_houses();
        } else if (choice == 3) {
            g.simulate_delivery(current_houses);
        } else if (choice == 4) {
            g.reset_houses();
            current_houses.clear(); // Empty the delivery manifest
        } else if (choice == 5) {
            cout << "Exiting system...\n";
            break; // Exit loop to close program
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}