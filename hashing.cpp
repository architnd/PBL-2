#include <iostream>
#include <string>

#define MAX 10 

using namespace std;

class record {
public:
    int rno;
    string name;
    int marks;

    record() {
        rno = -1;
        name = "-";
        marks = 0;
    }
};

class hashing {
public:
    int rno;
    int pos;        
    record rec;     

    hashing() {     
        rno = -1;
        pos = -1;
    }
};

class student {
public:
    record r;       
    hashing h[MAX]; 
    int offset;

    student() {
        offset = 0;
    }

    void create_new_table() {
        offset += MAX;
        for (int i = 0; i < MAX; i++) {
            h[i].rno = -1;
            h[i].pos = -1;
            h[i].rec.name = "-";
            h[i].rec.marks = 0;
        }
        cout << " New Hash Table created! Positions will now start from " << offset << ".\n";
    }

    void linear_probing_wor(hashing ht[], int key) {
        int loc = key % MAX;

        if (ht[loc].rno == -1) {
            ht[loc].rno = key;
            ht[loc].pos = loc;
            ht[loc].rec = r; 
            cout << " Inserted directly at index " << (loc + offset) << ".\n";
        } else {
            int i = (loc + 1) % MAX;
            while (i != loc) {
                if (ht[i].rno == -1) {
                    ht[i].rno = key;
                    ht[i].pos = loc; 
                    ht[i].rec = r;
                    cout << " Collision! Inserted at index " << (i + offset) << ".\n";
                    return;
                }
                i = (i + 1) % MAX;
            }
            cout << " Hash is full! Cannot insert roll no: " << key << endl;
        }
    }

    void linear_probing_wr(hashing ht[], int key) {
        int loc = key % MAX;

        if (ht[loc].rno == -1) {
            ht[loc].rno = key;
            ht[loc].pos = loc;
            ht[loc].rec = r;
            cout << " Inserted directly at index " << (loc + offset) << ".\n";
        } else {
            record temp_rec = r;
            int temp_pos = loc;

            if (loc != (ht[loc].rno % MAX)) {
                temp_rec = ht[loc].rec;    
                temp_pos = ht[loc].pos;

                ht[loc].rno = key;         
                ht[loc].pos = loc;
                ht[loc].rec = r;
                cout << " Replaced squatter at index " << (loc + offset) << ". Repositioning...\n";
            }

            int i = (loc + 1) % MAX;
            while (i != loc) {
                if (ht[i].rno == -1) {
                    ht[i].rno = temp_rec.rno;
                    ht[i].pos = temp_pos; 
                    ht[i].rec = temp_rec;
                    cout << " Collision handled! Placed at index " << (i + offset) << ".\n";
                    return;
                }
                i = (i + 1) % MAX;
            }
            cout << " Hash is full! Cannot insert.\n";
        }
    }

    void display(hashing ht[]) {
        cout << "\n--- HASH TABLE ---\n";
        cout << "pos\tRNo\tName\tMarks\n";
        for (int i = 0; i < MAX; i++) {
            if (ht[i].rno != -1) {
                cout << (i + offset) << "\t" << ht[i].rno << "\t" << ht[i].rec.name << "\t" << ht[i].rec.marks << "\n";
            } else {
                cout << (i + offset) << "\t-\t-\t-\n";
            }
        }
    }

    void search(hashing ht[], int key) {
        int loc = key % MAX;
        int i = loc;
        bool found = false;

        do {
            if (ht[i].rno == key) {
                cout << " Record found at index " << (i + offset) << "!\n";
                cout << " Name: " << ht[i].rec.name << " | Marks: " << ht[i].rec.marks << "\n";
                found = true;
                break;
            }
            if (ht[i].rno == -1) {
                break; 
            }
            i = (i + 1) % MAX;
        } while (i != loc); 

        if (!found) {
            cout << " Record with roll no " << key << " not found.\n";
        }
    }
};

int main() {
    student s;
    int choice, n, search_key;

    do {
        cout << "\n===== HASHING MAIN MENU =====\n";
        cout << "1. Create New Hash Table (Reset & Update Position)\n";
        cout << "2. Insert (Linear Probing WITHOUT Replacement)\n";
        cout << "3. Insert (Linear Probing WITH Replacement)\n";
        cout << "4. Display Hash Table\n";
        cout << "5. Search for a Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                s.create_new_table();
                break;
            case 2: 
                cout << "\nHow many records do you want to insert? ";
                cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "Enter Roll No: ";
                    cin >> s.r.rno;
                    s.r.name = "-"; 
                    s.r.marks = 0;  
                    s.linear_probing_wor(s.h, s.r.rno); 
                }
                break;
            case 3: 
                cout << "\nHow many records do you want to insert? ";
                cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "Enter Roll No: ";
                    cin >> s.r.rno;
                    s.r.name = "-"; 
                    s.r.marks = 0;  
                    s.linear_probing_wr(s.h, s.r.rno); 
                }
                break;
            case 4: 
                s.display(s.h); 
                break;
            case 5: 
                cout << "\nEnter Roll No to search: ";
                cin >> search_key;
                s.search(s.h, search_key);
                break;
            case 6: 
                cout << "Exiting program...\n"; 
                break;
            default: 
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}