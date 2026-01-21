#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
class mobileuser {
protected:
    long int mobile_no;
    string name;
    long int billamount;
public:
    mobileuser() {
        mobile_no = 0;
        name = "";
        billamount = 0;
    }
    friend class records;
};
class records {
protected:
    mobileuser M[10];
    int n;
public:
    records(int n1) {
        n = n1;
    }
    void accept();
    void display();
    void quickSort(int p, int r);
    int partition(int p, int r);
    void mergeSort(int l, int r);
    void merge(int l, int m, int r);
};
void records::accept() {
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
void records::display() {
    cout << "\nMobile User Records:\n";
    cout << "Mobile Number\t\tName\tBill Amount\n";
    for (int i = 0; i < n; i++) {
        cout << M[i].mobile_no << "\t\t" << M[i].name << "\t" << M[i].billamount << endl;
    }
}
int records::partition(int p, int r) {
    long int x = M[r].mobile_no;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (M[j].mobile_no <= x) {
            i++;
            swap(M[i], M[j]);
        }
    }
    swap(M[i + 1], M[r]);
    return i + 1;
}
void records::quickSort(int p, int r) {
    if (p < r) {
        int q = partition(p, r);
        quickSort(p, q - 1);
        quickSort(q + 1, r);
    }
}
#if 0
void records::merge(int l, int m, int r) {
    vector<mobileuser> vec;
    int left = l, right = m + 1;
    while (left <= m && right <= r) {
        if (M[left].mobile_no <= M[right].mobile_no)
            vec.push_back(M[left++]);
        else
            vec.push_back(M[right++]);
    }
    while (left <= m)
        vec.push_back(M[left++]);
    while (right <= r)
        vec.push_back(M[right++]);
    for (int i = l; i <= r; i++) {
        M[i] = vec[i - l];
    }
}
void records::mergeSort(int l, int r) {
    if (l >= r)
        return;
    int m = (l + r) / 2;
    mergeSort(l, m);
    mergeSort(m + 1, r);
    merge(l, m, r);
}
#endif
#if 1
void records::mergeSort(int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(low, mid);   
        mergeSort(mid + 1, high);
        merge(low, mid, high);
    }
}
void records::merge(int low, int mid, int high) {
    int h = low;
    int i = low;
    int j = mid + 1;
    
    mobileuser b[10];

    while ((h <= mid) && (j <= high)) {
        if (M[h].mobile_no <= M[j].mobile_no) {
            b[i] = M[h];
            h = h + 1;
        } else {
            b[i] = M[j];
            j = j + 1;
        }
        i = i + 1;
    }
    if (h > mid) {
        for (int k = j; k <= high; k++) {
            b[i] = M[k];
            i = i + 1;
        }
    } else {
        for (int k = h; k <= mid; k++) {
            b[i] = M[k];
            i = i + 1;
        }
    }
    for (int k = low; k <= high; k++) {
        M[k] = b[k];
    }
}
#endif
int main() {
    int n, choice;
    long int key;
    cout << " Mobile User Records System" << endl;
    cout << "Enter number of records (max 10): ";
    cin >> n;
    if(n <= 0 || n > 10) {
        cout << "Invalid number! Must be between 1 and 10." << endl;
        return 1;
    }
   
    records r(n);
   
    while(true) {
        cout << "\nMENU:" << endl;
        cout << "1. Accept data" << endl;
        cout << "2. Display data" << endl;
        cout << "3. Sort records by Mobile Number(quick sort)" << endl;
        cout << "4. Sort records by Mobile Number(merge sort)" << endl;
        cout << "5. Exit" << endl;
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
                r.quickSort(0, n - 1);
                cout << "Records sorted by Mobile Number(quick sort)." << endl;
                break;
            case 4:
                r.mergeSort(0, n - 1);
                cout << "Records sorted by Mobile Number(merge sort)." << endl;
                break;
            case 5:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}