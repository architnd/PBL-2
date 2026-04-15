#include <iostream>
#include <string.h>
#include <fstream>

#define MAX 10 

using namespace std;

class record {
public:
    int rollno;
    char name[20]; 
    int marks;

    record() {
        rollno = -1;
        strcpy(name, "-");
        marks = 0;
    }
};

class hashing {
public:
    int rollno;
    int pos;        

    hashing() {     
        rollno = -1;
        pos = -1;
    }
};

class student {
public:
    record rec;       
    hashing h[MAX]; 
    int relt;

    student() {
        relt = 0;
    }

    void create_wor() {
        char ans;
        int loc;
        relt = 0; // Reset position counter for the new table
        
        // Reset hash table for a fresh new table layout in memory
        for (int i = 0; i < MAX; i++) {
            h[i].rollno = -1;
            h[i].pos = -1;
        }

        do {
            cout << "\nEnter Roll no. : ";
            cin >> rec.rollno;
            
            // Automatically set default name and marks
            strcpy(rec.name, "-");
            rec.marks = 0;
            
            loc = rec.rollno % MAX;

            if (h[loc].rollno == -1) {
                h[loc].rollno = rec.rollno;
                h[loc].pos = relt; 
                relt = relt + 1;
                cout << " Inserted directly at index " << loc << ".\n";
            } else {
                int i = (loc + 1) % MAX;
                bool inserted = false;
                while (i != loc) {
                    if (h[i].rollno == -1) {
                        h[i].rollno = rec.rollno;
                        h[i].pos = relt; 
                        relt = relt + 1;
                        cout << " Collision! Inserted at index " << i << ".\n";
                        inserted = true;
                        break; 
                    }
                    i = (i + 1) % MAX;
                }
                if (!inserted) {
                    cout << " Hash is full! Cannot insert roll no: " << rec.rollno << endl;
                }
            }
            cout << "Do you want to insert more records? (y/n): ";
            cin >> ans;
        } while (ans == 'y' || ans == 'Y');
        
        // --- Save the visual snapshot of the table to our SINGLE file ---
        static int table_count = 1; 
        ofstream out("student.txt", ios::app);
        out << "TABLE " << table_count++ << " (Without Replacement)\n";
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

    void create_wr() {
        char ans;
        int loc;
        relt = 0; // Reset position counter for the new table
        
        for (int i = 0; i < MAX; i++) {
            h[i].rollno = -1;
            h[i].pos = -1;
        }

        do {
            cout << "\nEnter Roll no. : ";
            cin >> rec.rollno;

            // Automatically set default name and marks
            strcpy(rec.name, "-");
            rec.marks = 0;
            
            loc = rec.rollno % MAX;

            if (h[loc].rollno == -1) {
                h[loc].rollno = rec.rollno;
                h[loc].pos = relt;
                relt = relt + 1;
                cout << " Inserted directly at index " << loc << ".\n";
            } else {
                int current_new_pos = relt; 
                relt = relt + 1;

                int temp_rollno = rec.rollno;
                int temp_pos = current_new_pos;

                // If squatter doesn't naturally belong at 'loc', replace them
                if (loc != (h[loc].rollno % MAX)) {
                    temp_rollno = h[loc].rollno;    
                    temp_pos = h[loc].pos;

                    h[loc].rollno = rec.rollno;         
                    h[loc].pos = current_new_pos;
                    cout << " Replaced squatter at index " << loc << ". Repositioning...\n";
                }

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
        
        // --- Save the visual snapshot of the table to our SINGLE file ---
        static int table_count = 1; 
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
        while (getline(in, line)) {
            // If line contains the target table string
            if (line.find(target) != string::npos) {
                found = true;
                cout << line << "\n";
                // Print the rest of the table until an empty line is hit
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

    void displayAll() {
        ifstream in("student.txt");
        if (!in) {
            cout << " No tables created yet.\n";
            return;
        }

        string line;
        cout << "\n--- ALL SAVED HASH TABLES ---\n";
        while (getline(in, line)) {
            cout << line << "\n";
        }
        in.close();
    }
};

int main() {
    // Clear previous text file on new run for cleanliness 
    ofstream clearText("student.txt", ios::trunc); 
    clearText.close();

    student s; 
    int choice;

    do {
        cout << "\nHASHING MAIN MENU:\n";
        cout << "1. Create Hash Table(WITHOUT Replacement)\n";
        cout << "2. Create Hash Table(WITH Replacement)\n";
        cout << "3. Display Specific Table Number\n";
        cout << "4. Display All Tables\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                s.create_wor();
                break;
            case 2: 
                s.create_wr();
                break;
            case 3:
                s.displaySpecific(); 
                break;
            case 4:
                s.displayAll();
                break;
            case 5: 
                cout << "Exiting program...\n"; 
                break;
            default: 
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}