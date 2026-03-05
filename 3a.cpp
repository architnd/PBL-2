#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class Stack {
    int arr[100];
    int topIndex;
public:
    Stack() { topIndex = -1; }
    void push(int val) {
        if (topIndex >= 99) {
            cout << "Stack Overflow\n";
        } else {
            arr[++topIndex] = val;
        }
    }
    void pop() {
        if (topIndex < 0) {
            cout << "Stack Underflow\n";
        } else {
            topIndex--;
        }
    }
    int top() {
        if (topIndex < 0) {
            cout << "Stack Empty\n";
            return -1;
        }
        return arr[topIndex];
    }
    bool empty() {
        return topIndex < 0;
    }
};

class Queue {
    int arr[100];
    int frontIndex;
    int backIndex;
public:
    Queue() {
        frontIndex = 0;
        backIndex = 0;
    }
    void push(int val) {
        if (backIndex >= 100) {
            cout << "Queue Full\n";
        } else {
            arr[backIndex++] = val;
        }
    }
    void pop() {
        if (frontIndex == backIndex) {
            cout << "Queue Empty\n";
        } else {
            frontIndex++;
        }
    }
    int front() {
        if (frontIndex == backIndex) {
            cout << "Queue Empty\n";
            return -1;
        }
        return arr[frontIndex];
    }
    bool empty() {
        return frontIndex == backIndex;
    }
};

class usernode{
    int id;
    string name;
    usernode* next;
public:
    usernode() : id(0), name(""), next(NULL) {}
    friend class Facebook;
};

class Facebook{
    usernode * head[50];
    int n, v;
    char choice;
    int visited[50];
    Stack s;
    Queue q;
public:
    Facebook(int n1) : n(n1) {
        for(int i=0; i<50; i++){
            head[i] = new usernode();
            head[i]->id = i;
            head[i]->name = "User"+to_string(i);
        }
    }
    void acceptName();
    void createAdjacencyLists();
    void displayAdjacencyLists();
    void DFS();
    void DFS(int v);
    void DFS_nr(int v);
    void BFS(int v);
};

void Facebook::acceptName(){
    for(int i=0; i<n; i++){
        int id;
        string name;
        cout << "Name of user " << (i+1) << ": ";
        cin >> name;
        head[i]->name = name;
    }
}

void Facebook::createAdjacencyLists(){
    for (int i = 0; i < n; i++){
        usernode* temp = head[i];
        cout << "Is there a adjacent vertex to user " << head[i]->id << "," << head[i]->name << "? (y/n): ";
        cin >> choice;
        if(choice == 'y'){
            do{
            cout << "Enter adjacent vertex id: ";
            cin >> v;
            if(v == i){
                cout << "Self loop are not allowed" << endl;
                } else {
                    usernode* curr = new usernode();
                    curr->id = v;
                    curr->name = head[v]->name;
                    curr->next = NULL;
                    temp->next = curr;
                    temp = temp->next;
                }
                cout << "Do you want to add more adjacent vertices to user " << head[i]->id << "," << head[i]->name << "? (y/n): ";
                cin >> choice;
            }while(choice == 'y');
        }
    }
}

void Facebook::displayAdjacencyLists(){
    cout << "\nAdjacency Lists:" << endl;
    for(int i=0; i<n; i++){
        usernode* temp = head[i];
        cout << head[i]->id << "," << head[i]->name << " -> ";
        temp = temp->next;
        while(temp != NULL){
            cout << head[temp->id]->id << "," << head[temp->id]->name << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
}

void Facebook::DFS(){
    for (int i =0; i<n; i++){
        visited[i] = 0;
    }
    cout << "Accept starting id for DFS: ";
    cin >> v;
    DFS(v);
    cout << "NULL" << endl;
}

void Facebook::DFS(int v){
    visited[v] = 1;
    cout << head[v]->id << "," << head[v]->name << " -> ";
    usernode* temp = head[v]->next;
    while(temp != NULL){
        if(visited[temp->id] == 0){
            DFS(temp->id);
        }
        temp = temp->next;
    }
}

void Facebook::DFS_nr(int v){
    for(int i=0; i<n; i++){
        visited[i] = 0;
    }
    cout << "Accept starting id for DFS: ";
    cin >> v;
    s.push(v); 
    visited[v] = 1;
    do{
        v = s.top();
        s.pop();
        cout << head[v]->id << "," << head[v]->name << " -> ";
        usernode* temp = head[v]->next;
        while(temp != NULL){
            if(visited[temp->id] == 0){
                s.push(temp->id);
                visited[temp->id] = 1;
            }
            temp = temp->next;
        }
    }while(!s.empty());
    cout << "NULL" << endl;
}

void Facebook::BFS(int v){
    for(int i=0; i<n; i++){
        visited[i] = 0;
    }
    cout << "Accept starting id for BFS: ";
    cin >> v;
    q.push(v);
    visited[v] = 1;
    while(!q.empty()){
        v = q.front();
        q.pop();
        cout << head[v]->id << "," << head[v]->name << " -> ";
        usernode* temp = head[v]->next;
        while(temp != NULL){
            if(visited[temp->id] == 0){
                q.push(temp->id);
                visited[temp->id] = 1;
            }
            temp = temp->next;
        }
    }
    cout << "NULL" << endl;
}

int main(){
    int n, choice;
    cout << "Enter number of users (max 50): ";
    cin >> n;  
    if(n <= 0 || n > 50){
        cout << "Invalid number! Must be between 1 and 50." << endl;
        return 1;
    }
    Facebook fb(n);
    while(true){
        cout << "\nMENU:" << endl;
        cout << "1. Accept User Names" << endl;
        cout << "2. Create Adjacency Lists" << endl;
        cout << "3. Display Adjacency Lists" << endl;
        cout << "4. DFS Traversal recursive" << endl;
        cout << "5. DFS Traversal non-recursive" << endl;
        cout << "6. BFS Traversal" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice){
            case 1:
                fb.acceptName();
                break;
            case 2:
                fb.createAdjacencyLists();
                break;
            case 3:
                fb.displayAdjacencyLists();
                break;
            case 4:
                fb.DFS();
                break;
            case 5:
                fb.DFS_nr(0);
                break;
            case 6:
                fb.BFS(0);
                break;
            case 7:
                cout << "Exiting program." << endl;
                return 0;  
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}
