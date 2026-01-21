#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct MobileUser {
    long long mobileNo;
    string name;
    float billAmount;
};

int linearSearch(vector<MobileUser>& users, long long key) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].mobileNo == key) {
            return i;
        }
    }
    return -1;
}

int binarySearchIterative(vector<MobileUser>& users, long long key) {
    int low = 0;
    int high = users.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (users[mid].mobileNo == key)
            return mid;
        
        if (users[mid].mobileNo < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int binarySearchRecursive(vector<MobileUser>& users, int low, int high, long long key) {
    if (low <= high) {
        int mid = low + (high - low) / 2;

        if (users[mid].mobileNo == key)
            return mid;
        
        if (users[mid].mobileNo < key)
            return binarySearchRecursive(users, mid + 1, high, key);
        
        return binarySearchRecursive(users, low, mid - 1, key);
    }
    return -1;
}

bool compareUsers(const MobileUser& a, const MobileUser& b) {
    return a.mobileNo < b.mobileNo;
}

void display(MobileUser u) {
    cout << "\nFound Record:\n";
    cout << "Name: " << u.name << "\n";
    cout << "Mobile: " << u.mobileNo << "\n";
    cout << "Bill: " << u.billAmount << "\n";
}

int main() {
    int n, choice, idx = -1;
    long long key;
    
    cout << "Enter number of users: ";
    cin >> n;

    vector<MobileUser> users(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter details for user " << i + 1 << " (Mobile Name Bill): ";
        cin >> users[i].mobileNo >> users[i].name >> users[i].billAmount;
    }

    sort(users.begin(), users.end(), compareUsers);
    
    do {
        cout << "\n1. Linear Search\n2. Binary Search (Iterative)\n3. Binary Search (Recursive)\n4. Exit\nChoice: ";
        cin >> choice;

        if (choice == 4) break;

        cout << "Enter Mobile No to search: ";
        cin >> key;

        switch (choice) {
            case 1:
                idx = linearSearch(users, key);
                break;
            case 2:
                idx = binarySearchIterative(users, key);
                break;
            case 3:
                idx = binarySearchRecursive(users, 0, n - 1, key);
                break;
            default:
                cout << "Invalid choice\n";
                continue;
        }

        if (idx != -1)
            display(users[idx]);
        else
            cout << "Record not found\n";

    } while (choice != 4);

    return 0;
}