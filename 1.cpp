#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
class MobileUser {
private:
    long long mobile_no;
    string name;
    long long billamount;
   
public:
    MobileUser() : mobile_no(0), name(""), billamount(0) {}
    friend class Records;
};
class Records {
private:
    MobileUser M[10];
    int n;
   
public:
    Records(int n1) {
        n = n1;
    }
    void accept();  
    void display();
    int linear_Search(long int key);
    int Binary_Search(string key_name);
    int Recursive_Binary_Search(int low, int high, string key_name);
};
void Records::accept() {
    for (int i = 0; i < n; i++) {
        cout << "Enter details of Mobile User " << (i + 1) << ":\n";
        cout << "Mobile Number: ";
        cin >> M[i].mobile_no;
        cout << "Name: ";
        cin >> M[i].name;
        cout << "Bill Amount: ";
        cin >> M[i].billamount;
    }
};
void Records::display() {
    cout << "\nMobile User Records:\n";
    cout << "Mobile Number\t\tName\tBill Amount\n";
    for (int i = 0; i < n; i++) {
        cout << M[i].mobile_no << "\t\t" << M[i].name << "\t" << M[i].billamount << endl;
    }
}
int Records::linear_Search(long int key) {
    for (int i = 0; i < n; i++) {
        if (M[i].mobile_no == key) {
            return i;
        }
    }
    return -1;
}
int Records::Binary_Search(string key_name) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (M[mid].name == key_name) {
            return mid;
        }
        if (M[mid].name < key_name) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
int Records::Recursive_Binary_Search(int low, int high, string key_name) {
    if (low <= high) {
        int mid = low + (high - low) / 2;

        if (M[mid].name == key_name) {
            return mid;
        }
        
        if (M[mid].name < key_name) {
            return Recursive_Binary_Search(mid + 1, high, key_name);
        } else {
            return Recursive_Binary_Search(low, mid - 1, key_name);
        }
    }
    return -1;
}
int main() {
    int n, choice;
    long int key;
    string name_key;
    int index;
   
    cout << " Mobile User Records System" << endl;
    cout << "Enter number of records (max 10): ";
    cin >> n;
   
    if(n <= 0 || n > 10) {
        cout << "Invalid number! Must be between 1 and 10." << endl;
        return 1;
    }
   
    Records r(n);
   
    while(true) {
        cout << "\nMENU:" << endl;
        cout << "1. Accept data" << endl;
        cout << "2. Display data" << endl;    
        cout << "3. Search by Mobile Number (Linear Search)" << endl;
        cout << "4. Search by Name (Binary Search non-recursive)" << endl;
        cout << "5. Search by Name (Binary Search recursive)" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                r.accept();
                break;
            case 2:
                r.display();
                break;
            case 3:
                cout << "Enter Mobile Number to search: ";
                cin >> key;
                index = r.linear_Search(key);
                if(index != -1) {
                    cout << "Record found at index " << index << endl;
                } else {
                    cout << "Record not found." << endl;
                }
                break;
            case 4:
                cout << "Enter Name to search: ";
                cin >> name_key;
                index = r.Binary_Search(name_key);
                if(index != -1) {
                    cout << "Record found at index " << index << endl;
                } else {
                    cout << "Record not found." << endl;
                }
                break;
            case 5:
                cout << "Enter Name to search: ";
                cin >> name_key;
                index = r.Recursive_Binary_Search(0, n - 1, name_key);
                if(index != -1) {
                    cout << "Record found at index " << index << endl;
                } else {
                    cout << "Record not found." << endl;
                }
                break;
            case 6:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}