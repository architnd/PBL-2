#include <iostream>
#include <string.h> // Required for strcpy() to manipulate C-style strings
#include <fstream>  // Required for file handling (ifstream, ofstream)
#include <cstdio>   // Required for file operations like remove() and rename()

#define MAX 10 // Defines the fixed size of our Hash Table

using namespace std;

// ==========================================
// 1. DATA CLASS (The Payload)
// ==========================================
class record {
public:
    int rollno;
    char name[20]; // Using a char array instead of std::string makes it safer for raw memory/file operations
    int marks;

    // Constructor: Automatically sets default values when a new record is created
    record() {
        rollno = -1;
        strcpy(name, "-");
        marks = 0;
    }
};

// ==========================================
// 2. HASH SLOT CLASS (The Table Entry)
// ==========================================
class hashing {
public:
    int rollno; // Acts as the "Key" for the hash table
    int pos;    // Stores the sequential order (0, 1, 2...) in which records were added

    // Constructor: Sets slots to -1 to indicate they are "empty"
    hashing() {     
        rollno = -1;
        pos = -1;
    }
};

// ==========================================
// 3. MAIN MANAGER CLASS
// ==========================================
class student {
public:
    record rec;       // Temporary record object to hold user input
    hashing h[MAX];   // The actual Hash Table array of size 10
    int relt;         // Relative position counter (counts how many items are in the current table)
    int table_count;  // Shared counter across all functions to uniquely number the saved tables

    // Constructor: Initializes counters when the program starts
    student() {
        relt = 0;
        table_count = 1; 
    }

    // ---------------------------------------------------------
    // INSERTION: LINEAR PROBING WITHOUT REPLACEMENT
    // ---------------------------------------------------------
    void create_wor() {
        char ans;
        int loc;
        relt = 0; // Reset the item counter for this new table session
        
        // Step 1: Wipe the hash table clean for a fresh start
        for (int i = 0; i < MAX; i++) {
            h[i].rollno = -1;
            h[i].pos = -1;
        }

        // Step 2: Loop to continuously accept student records
        do {
            cout << "\nEnter Roll no. : ";
            cin >> rec.rollno;
            
            // Auto-fill default data to save typing
            strcpy(rec.name, "-");
            rec.marks = 0;
            
            // HASH FUNCTION: Division Method
            loc = rec.rollno % MAX;

            // Scenario A: The calculated "home" slot is empty
            if (h[loc].rollno == -1) {
                h[loc].rollno = rec.rollno; // Claim the slot
                h[loc].pos = relt;          // Assign its relative position
                relt = relt + 1;            // Increment counter for the next item
                cout << " Inserted directly at index " << loc << ".\n";
            } 
            // Scenario B: Collision! The slot is already taken
            else {
                // Start probing at the VERY NEXT index
                int i = (loc + 1) % MAX; 
                bool inserted = false;
                
                // Keep moving forward until we circle back to the start
                while (i != loc) {
                    if (h[i].rollno == -1) { // Found an empty spot!
                        h[i].rollno = rec.rollno;
                        h[i].pos = relt; 
                        relt = relt + 1;
                        cout << " Collision! Inserted at index " << i << ".\n";
                        inserted = true;
                        break; // Stop looking, we successfully inserted it
                    }
                    // Move to the next index, wrapping around to 0 if we hit the end
                    i = (i + 1) % MAX;
                }
                
                // If we checked everywhere and 'inserted' is still false, the table is full
                if (!inserted) {
                    cout << " Hash is full! Cannot insert roll no: " << rec.rollno << endl;
                }
            }
            
            cout << "Do you want to insert more records? (y/n): ";
            cin >> ans;
        } while (ans == 'y' || ans == 'Y');
        
        // Step 3: Save a visual snapshot of the completed table to our text file
        ofstream out("student.txt", ios::app); // Open in append mode so we don't erase older tables
        out << "TABLE " << table_count++ << " (Without Replacement)\n";
        out << "pos\tRNo\tName\tMarks\n";
        
        // Loop through the array and write its contents to the file
        for (int j = 0; j < MAX; j++) {
            if (h[j].rollno != -1) {
                out << j << "\t" << h[j].rollno << "\t" << rec.name << "\t" << rec.marks << "\n";
            } else {
                out << j << "\t-\t-\t-\n"; // Print dashes for empty slots
            }
        }
        out << "\n"; // Add a blank line to separate tables
        out.close();
        cout << "\nTable successfully built and saved to student.txt.\n";
    }

    // ---------------------------------------------------------
    // INSERTION: LINEAR PROBING WITH REPLACEMENT
    // ---------------------------------------------------------
    void create_wr() {
        char ans;
        int loc;
        relt = 0; 
        
        // Wipe table clean
        for (int i = 0; i < MAX; i++) {
            h[i].rollno = -1;
            h[i].pos = -1;
        }

        do {
            cout << "\nEnter Roll no. : ";
            cin >> rec.rollno;

            strcpy(rec.name, "-");
            rec.marks = 0;
            
            loc = rec.rollno % MAX;

            // Scenario A: Home slot is empty
            if (h[loc].rollno == -1) {
                h[loc].rollno = rec.rollno;
                h[loc].pos = relt;
                relt = relt + 1;
                cout << " Inserted directly at index " << loc << ".\n";
            } 
            // Scenario B: Collision! Need to check for squatters.
            else {
                int current_new_pos = relt; 
                relt = relt + 1;

                int temp_rollno = rec.rollno;
                int temp_pos = current_new_pos;

                // SQUATTER CHECK: Does the current occupant naturally belong here?
                // Calculate where the current occupant *should* be (h[loc].rollno % MAX)
                // If it doesn't match 'loc', they are a squatter and must be evicted!
                if (loc != (h[loc].rollno % MAX)) {
                    // 1. Move squatter data into our temp variables
                    temp_rollno = h[loc].rollno;    
                    temp_pos = h[loc].pos;

                    // 2. Put the new rightful owner in this slot
                    h[loc].rollno = rec.rollno;         
                    h[loc].pos = current_new_pos;
                    cout << " Replaced squatter at index " << loc << ". Repositioning...\n";
                }

                // Probe for an empty slot. 
                // Note: We are searching for a spot for 'temp_rollno'. 
                // This could be the evicted squatter, OR the new record if no eviction happened.
                int i = (loc + 1) % MAX;
                bool inserted = false;
                while (i != loc) {
                    if (h[i].rollno == -1) {
                        h[i].rollno = temp_rollno;
                        h[i].pos = temp_pos; 
                        cout << " Collision handled! Placed at index " << i << ".\n";
                        inserted = true;
                        break; 
                    }
                    i = (i + 1) % MAX;
                }
                if (!inserted) {
                    cout << " Hash is full! Cannot insert.\n";
                }
            }
            cout << "Do you want to insert more records? (y/n): ";
            cin >> ans;
        } while (ans == 'y' || ans == 'Y');
        
        // Save snapshot to file
        ofstream out("student.txt", ios::app);
        out << "TABLE " << table_count++ << " (With Replacement)\n";
        out << "pos\tRNo\tName\tMarks\n";
        
        for (int j = 0; j < MAX; j++) {
            if (h[j].rollno != -1) {
                out << j << "\t" << h[j].rollno << "\t" << rec.name << "\t" << rec.marks << "\n";
            } else {
                out << j << "\t-\t-\t-\n";
            }
        }
        out << "\n"; 
        out.close();
        cout << "\nTable successfully built and saved to student.txt.\n";
    }

    // ---------------------------------------------------------
    // SEARCH / RETRIEVE FUNCTION (Reading from File)
    // ---------------------------------------------------------
    void retrieve() {
        ifstream in("student.txt"); // Open file for reading
        if (!in) {
            cout << " No tables created yet.\n";
            return;
        }

        int tno, search_key;
        cout << "\nEnter Table Number to search in: ";
        cin >> tno;
        cout << "Enter Roll no. to find: ";
        cin >> search_key;

        string target = "TABLE " + to_string(tno); // e.g., "TABLE 1"
        bool table_found = false;
        bool key_found = false;
        string line;
        
        // Read file line by line
        while (getline(in, line)) {
            // Check if the current line contains our target table name
            if (line.find(target) != string::npos) {
                table_found = true;
                getline(in, line); // Skip the "pos RNo Name Marks" header line
                
                // Read rows of this table until we hit a blank line
                while (getline(in, line) && !line.empty()) {
                    
                    // Logic to extract just the Roll No from the text string
                    // Data format is: POS [tab] RNO [tab] NAME [tab] MARKS
                    int first_tab = line.find('\t'); // Find first tab
                    if (first_tab != string::npos) {
                        int second_tab = line.find('\t', first_tab + 1); // Find second tab
                        if (second_tab != string::npos) {
                            // Substring between the two tabs is our Roll No
                            string rno_str = line.substr(first_tab + 1, second_tab - first_tab - 1);
                            
                            // If the string matches our search key, we found it!
                            if (rno_str == to_string(search_key)) {
                                cout << "\nkey found\n";
                                cout << "pos\tRNo\tName\tMarks\n";
                                cout << line << "\n";
                                key_found = true;
                                break; // Stop searching this table
                            }
                        }
                    }
                }
                break; // We finished checking the requested table, no need to read the rest of the file
            }
        }
        
        // Output results
        if (!table_found) {
            cout << "Table " << tno << " not found.\n";
        } else if (!key_found) {
            cout << "\nkey not found\n";
        }
        
        in.close();
    }

    // ---------------------------------------------------------
    // MODIFY FUNCTION (Rewriting the File)
    // ---------------------------------------------------------
    void modify() {
        ifstream in("student.txt");
        if (!in) {
            cout << " No tables created yet.\n";
            return;
        }

        int tno, search_key;
        cout << "\nEnter Table Number where the record exists: ";
        cin >> tno;
        cout << "Enter Roll no. to modify: ";
        cin >> search_key;

        // Since we can't easily overwrite the middle of a text file, 
        // we write everything to a temporary file, intercepting and changing the target line.
        ofstream out("temp.txt"); 
        string target = "TABLE " + to_string(tno);
        bool table_found = false;
        bool key_found = false;
        string line;

        while (getline(in, line)) {
            // When we find our target table...
            if (!table_found && line.find(target) != string::npos) {
                table_found = true;
                out << line << "\n"; // Write the "TABLE X" title to temp file
                
                if (getline(in, line)) {
                    out << line << "\n"; // Write the column headers to temp file
                }
                
                // Process the actual data rows of the table
                while (getline(in, line) && !line.empty()) {
                    int first_tab = line.find('\t');
                    // If we haven't modified the key yet, parse the row to check it
                    if (first_tab != string::npos && !key_found) {
                        int second_tab = line.find('\t', first_tab + 1);
                        if (second_tab != string::npos) {
                            string rno_str = line.substr(first_tab + 1, second_tab - first_tab - 1);
                            
                            // WE FOUND THE TARGET ROW!
                            if (rno_str == to_string(search_key)) {
                                key_found = true;
                                cout << "\nRecord found!\n";
                                cout << "Current Record: " << line << "\n";
                                
                                // Ask user for the new data
                                char new_name[20];
                                int new_marks;
                                cout << "Enter New Name: ";
                                cin >> new_name;
                                cout << "Enter New Marks: ";
                                cin >> new_marks;
                                
                                // Extract the original position index so we don't mess up the layout
                                string pos_str = line.substr(0, first_tab);
                                
                                // Write the BRAND NEW formulated line into the temp file
                                out << pos_str << "\t" << search_key << "\t" << new_name << "\t" << new_marks << "\n";
                                cout << "Record updated successfully.\n";
                                
                                continue; // SKIP writing the old 'line' to the file, moving to next loop iteration
                            }
                        }
                    }
                    // For all other normal rows, just copy them exactly as they are
                    out << line << "\n"; 
                }
                out << "\n"; // Add the blank space at the end of the table
            } else {
                // If it's not our target table, just copy the line directly to temp
                out << line << "\n"; 
            }
        }

        // Close files to unlock them
        in.close();
        out.close();

        // Evaluate what happened and manage files
        if (!table_found) {
            cout << "Table " << tno << " not found.\n";
            remove("temp.txt"); // Discard the temp file
        } else if (!key_found) {
            cout << "Roll no " << search_key << " not found in Table " << tno << ".\n";
            remove("temp.txt"); // Discard the temp file
        } else {
            // SUCCESS! Delete the old database, and rename the temp file to be the new database
            remove("student.txt");
            rename("temp.txt", "student.txt");
        }
    }

    // ---------------------------------------------------------
    // DISPLAY SPECIFIC TABLE
    // ---------------------------------------------------------
    void displaySpecific() {
        ifstream in("student.txt");
        if (!in) {
            cout << " No tables created yet.\n";
            return;
        }

        int tno;
        cout << "\nEnter Table Number to display: ";
        cin >> tno;
        string target = "TABLE " + to_string(tno);
        bool found = false;
        string line;
        
        cout << "\n";
        // Search file for the title
        while (getline(in, line)) {
            if (line.find(target) != string::npos) {
                found = true;
                cout << line << "\n";
                // Print subsequent lines until a blank row is hit
                while (getline(in, line) && !line.empty()) {
                    cout << line << "\n";
                }
                break;
            }
        }
        if (!found) {
            cout << "Table " << tno << " not found.\n";
        }
        in.close();
    }

    // ---------------------------------------------------------
    // DISPLAY ALL TABLES
    // ---------------------------------------------------------
    void displayAll() {
        ifstream in("student.txt");
        if (!in) {
            cout << " No tables created yet.\n";
            return;
        }

        string line;
        cout << "\n--- ALL SAVED HASH TABLES ---\n";
        // Just read and print literally every line in the file
        while (getline(in, line)) {
            cout << line << "\n";
        }
        in.close();
    }
};

// ==========================================
// MAIN FUNCTION & MENU
// ==========================================
int main() {
    // SECURITY/CLEANUP: Open file in Truncate mode immediately on startup.
    // This instantly wipes the text file completely blank so you don't 
    // mix tables from previous runs of the program with your current run.
    ofstream clearText("student.txt", ios::trunc); 
    clearText.close();

    student s; 
    int choice;

    // Standard Menu-Driven Loop
    do {
        cout << "\nHASHING MAIN MENU:\n";
        cout << "1. Create Hash Table(WITHOUT Replacement)\n";
        cout << "2. Create Hash Table(WITH Replacement)\n";
        cout << "3. Display Specific Table Number\n";
        cout << "4. Display All Tables\n";
        cout << "5. Retrieve a Record\n";
        cout << "6. Modify a Record\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: s.create_wor(); break;
            case 2: s.create_wr(); break;
            case 3: s.displaySpecific(); break;
            case 4: s.displayAll(); break;
            case 5: s.retrieve(); break;
            case 6: s.modify(); break;
            case 7: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}