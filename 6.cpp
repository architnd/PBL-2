//Knapsack
#include<iostream>
using namespace std;

class Knapsack
{ 
    int n;
    int weight[100];
    int value[100];
    int W;
    int B[100][100];
public:
    Knapsack(){
        W = 0;
        n = 0;
        weight[0] = 0;
        value[0] = 0;
    }
    void insert_items(Knapsack &);
    void set_capacity();
    void solve(Knapsack &);
    void display_objects_included(Knapsack &);
};
void Knapsack::insert_items(Knapsack &items){
    char ch;
    int w, v;
    do{
        if (items.n >= 99){
            cout << "Maximum item limit reached." << endl;
            break;
        }
        cout << "Enter weight: ";
        cin >> w;
        cout << "Enter Profit: ";
        cin >> v;
        items.n++;
        items.weight[items.n] = w;
        items.value[items.n] = v;
        cout << "Do you want to insert another element? (Y/N): ";
        cin >> ch;
    } while(ch == 'Y' || ch == 'y');
}
void Knapsack::set_capacity(){
    cout << "Enter Knapsack Capacity: ";
    cin >> W;
}
void Knapsack::solve(Knapsack &items){
    if (items.n == 0 || W == 0){
        cout << "Data missing or Capacity is 0" << endl;
        return;
    }
    for (int w = 0; w <= W; w++){
        B[0][w] = 0;
    }
    for (int i = 1; i <= items.n; i++){
        B[i][0] = 0;
    }
    for (int i = 1; i <= items.n; i++){
        for (int w = 0; w <= W; w++){
            if (items.weight[i] <= w){
                if (items.value[i] + B[i - 1][w - items.weight[i]] > B[i - 1][w]){
                    B[i][w] = items.value[i] + B[i - 1][w - items.weight[i]];
                } else{
                    B[i][w] = B[i - 1][w];
                }
            } else{
                B[i][w] = B[i - 1][w];
            }
        }
    }
    cout << "Knapsack Solved Successfully\n" << "\n";
    cout << "Knapsack Table:\n";
    for (int i = 0; i <= items.n; i++){
        for (int w = 0; w <= W; w++){
            cout << B[i][w] << " ";
        }
        cout << "\n";
    }
    cout << "\nMaximum Profit: " << B[items.n][W] << "\n";
}
void Knapsack::display_objects_included(Knapsack &items){
    int i = items.n;
    int k = W;
    int arr[100] = {0};
    
    while (i > 0 && k > 0) {
        if (B[i][k] != B[i - 1][k]) {
            arr[i] = 1;
            k = k - items.weight[i];
            i = i - 1;
        } 
        else {
            i = i - 1;
        }
    }
    
    cout << "\nItems included in the Knapsack:\n";
    for (int j = 1; j <= items.n; j++) {
        if (arr[j] == 1) {
            cout << "Item: " << j << " ,Weight: " << items.weight[j] << " ,Value: " << items.value[j] << "\n";
        }
    }
    cout << "\nArray form: \n";
    for (int j = 1; j <= items.n; j++) {
        cout << arr[j] << " ";
    }
    cout << "\n";
}
int main(){
    Knapsack items;
    Knapsack knapsack;
    int choice;
    while (true){
        cout << "\nMENU:\n";
        cout << "1. Insert Items\n";
        cout << "2. Set Knapsack Capacity\n";
        cout << "3. Solve Knapsack\n";
        cout << "4. Display Included Items\n";
        cout << "5. Exit\n";
        cout << "Enter your Choice: ";
        cin >> choice;
        switch (choice){
        case 1:
            knapsack.insert_items(items);
            break;
        case 2:
            knapsack.set_capacity();
            break;
        case 3:
            knapsack.solve(items);
            break;
        case 4:
            knapsack.display_objects_included(items);
            break;
        case 5:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid Choice" << endl;
        }
    }
    return 0;
}