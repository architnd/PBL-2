#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class mobileuser {
public:
    long int mobile_no;
    string name;
    long int billamount;
    
    mobileuser() {
        mobile_no = 0;
        name = "";
        billamount = 0;
    }
};

class records {
public:
    mobileuser M[10];
    mobileuser S[10];
    int n;
    int pass_count;

    records(int n1) {
        n = n1;
        pass_count = 0;
    }
    
    void accept();
    void display(int type); 
    void quickSort(int p, int r);
    int partition(int p, int r);
    void mergeSort(int l, int r);
    void merge(int l, int m, int r);
    void heapSort(int n);
    void heapify(int n, int i);
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

void records::display(int type) {
    cout << "Mobile Number\t\tName\tBill Amount\n";
    for (int i = 0; i < n; i++) {
        if(type == 0)
            cout << M[i].mobile_no << "\t\t" << M[i].name << "\t" << M[i].billamount << endl;
        else
            cout << S[i].mobile_no << "\t\t" << S[i].name << "\t" << S[i].billamount << endl;
    }
}

int records::partition(int p, int r) {
    long int x = S[r].mobile_no;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (S[j].mobile_no >= x) {
            i++;
            swap(S[i], S[j]);
        }
    }
    swap(S[i + 1], S[r]);

    pass_count++;
    cout << "\nPass " << pass_count << ":" << endl;
    display(1);

    return i + 1;
}

void records::quickSort(int p, int r) {
    if (p < r) {
        int q = partition(p, r);
        quickSort(p, q - 1);
        quickSort(q + 1, r);
    }
}

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
        if (S[h].billamount <= S[j].billamount) { 
            b[i] = S[h];
            h = h + 1;
        } else {
            b[i] = S[j];
            j = j + 1;
        }
        i = i + 1;
    }
    if (h > mid) {
        for (int k = j; k <= high; k++) {
            b[i] = S[k];
            i = i + 1;
        }
    } else {
        for (int k = h; k <= mid; k++) {
            b[i] = S[k];
            i = i + 1;
        }
    }
    for (int k = low; k <= high; k++) {
        S[k] = b[k];
    }

    pass_count++;
    cout << "\nPass " << pass_count << ":" << endl;
    display(1);
}

void records::heapify(int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && S[l].billamount > S[largest].billamount)
        largest = l;

    if (r < n && S[r].billamount > S[largest].billamount)
        largest = r;

    if (largest != i) {
        swap(S[i], S[largest]);
        heapify(n, largest);
    }
}

void records::heapSort(int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(S[0], S[i]);
        heapify(i, 0);

        pass_count++;
        cout << "\nPass " << pass_count << ":" << endl;
        display(1);
    }
}

int main() {
    int n, choice, dispChoice;
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
        cout << "3. Quick Sort (Descending by Mobile No)" << endl;
        cout << "4. Merge Sort (Ascending by Bill Amount)" << endl;
        cout << "5. Heap Sort (Ascending by Bill Amount)" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                r.accept();
                break;
            case 2: 
                cout << "Display which array?\n1. Original\n2. Sorted\nEnter option: ";
                cin >> dispChoice;
                if(dispChoice == 1) r.display(0);
                else if(dispChoice == 2) r.display(1);
                else cout << "Invalid option." << endl;
                break;    
            case 3:
                for(int i=0; i<n; i++) r.S[i] = r.M[i]; 
                r.pass_count = 0;
                r.quickSort(0, n - 1);
                cout << "\nTotal passes: " << r.pass_count << endl;
                break;
            case 4:
                for(int i=0; i<n; i++) r.S[i] = r.M[i];
                r.pass_count = 0;
                r.mergeSort(0, n - 1);
                cout << "\nTotal passes: " << r.pass_count << endl;
                break;
            case 5:
                for(int i=0; i<n; i++) r.S[i] = r.M[i];
                r.pass_count = 0;
                r.heapSort(n);
                cout << "\nTotal passes: " << r.pass_count << endl;
                break;
            case 6:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}