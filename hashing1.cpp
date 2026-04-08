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

    void linear_probing_wor(int key) {
        int loc = key % MAX;

        if (h[loc].rno == -1) {
            h[loc].rno = key;
            h[loc].pos = loc;
            h[loc].rec = r; 
            cout << " Inserted directly at index " << loc << ".\n";
        } else {
            int i = (loc + 1) % MAX;
            while (i != loc) {
                if (h[i].rno == -1) {
                    h[i].rno = key;
                    h[i].pos = loc; 
                    h[i].rec = r;
                    cout << " Collision! Inserted at index " << i << ".\n";
                    return;
                }
                i = (i + 1) % MAX;
            }
            cout << " Hash is full! Cannot insert roll no: " << key << endl;
        }
    }

    void linear_probing_wr(int key) {
        int loc = key % MAX;

        if (h[loc].rno == -1) {
            h[loc].rno = key;
            h[loc].pos = loc;
            h[loc].rec = r;
            cout << " Inserted directly at index " << loc << ".\n";
        } else {
            record temp_rec = r;
            int temp_pos = loc;

            if (loc != (h[loc].rno % MAX)) {
                temp_rec = h[loc].rec;    
                temp_pos = h[loc].pos;

                h[loc].rno = key;         
                h[loc].pos = loc;
                h[loc].rec = r;
                cout << " Replaced squatter at index " << loc << ". Repositioning...\n";
            }

            int i = (loc + 1) % MAX;
            while (i != loc) {
                if (h[i].rno == -1) {
                    h[i].rno = temp_rec.rno;
                    h[i].pos = temp_pos; 
                    h[i].rec = temp_rec;
                    cout << " Collision handled! Placed at index " << i << ".\n";
                    return;
                }
                i = (i + 1) % MAX;
            }
            cout << " Hash is full! Cannot insert.\n";
        }
    }
    void search(int key) {
        int loc = key % MAX;
        int i = loc;

        do {
            if (h[i].rno == key) {
                cout << " Record found at position " << i << "\n";
                cout << " Name: " << h[i].rec.name << " , Marks: " << h[i].rec.marks << "\n";
                return; 
            }
            if (h[i].rno == -1) {
                break; 
            }
            i = (i + 1) % MAX;
        } while (i != loc); 

        cout << "Record with roll no " << key << " not found.\n";
    }
    void display() {
        cout << "pos\tRNo\tName\tMarks\n";
        for (int i = 0; i < MAX; i++) {
            if (h[i].rno != -1) {
                cout << i << "\t" << h[i].rno << "\t" << h[i].rec.name << "\t" << h[i].rec.marks << "\n";
            } else {
                cout << i << "\t-\t-\t-\n";
            }
        }
    }
};

int main() {
    student s1;
    student s2;
    int choice, n, search_key;

    do {
        cout << "\nHASHING MAIN MENU:\n";
        cout << "1. Insert (Linear Probing WITHOUT Replacement)\n";
        cout << "2. Insert (Linear Probing WITH Replacement)\n";
        cout << "3. Display Hash Table without Replacement\n";
        cout << "4. Display Hash Table with Replacement\n";
        cout << "5. Search for a Record in Hash Table without Replacement\n";
        cout << "6. Search for a Record in Hash Table with Replacement\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                cout << "\nHow many records do you want to insert? ";
                cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "Enter Roll No: ";
                    cin >> s1.r.rno;
                    s1.r.name = "-"; 
                    s1.r.marks = 0;  
                    s1.linear_probing_wor(s1.r.rno);
                }
                break;
            case 2: 
                cout << "\nHow many records do you want to insert? ";
                cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "Enter Roll No: ";
                    cin >> s2.r.rno;
                    s2.r.name = "-"; 
                    s2.r.marks = 0;  
                    s2.linear_probing_wr(s2.r.rno); 
                }
                break;
            case 3:
                cout << "\nHASH TABLE WITHOUT REPLACEMENT\n";
                s1.display(); 
                break;
            case 4: 
                cout << "\nHASH TABLE WITH REPLACEMENT\n";
                s2.display(); 
                break;
            case 5:
                cout << "\nEnter Roll No to search in HASH TABLE WITHOUT REPLACEMENT: ";
                cin >> search_key;
                s1.search(search_key);
                break;
            case 6:
                cout << "\nEnter Roll No to search in HASH TABLE WITH REPLACEMENT: ";
                cin >> search_key;
                s2.search(search_key);
                break;
            case 7: 
                cout << "Exiting program...\n"; 
                break;
            default: 
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}