#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<stack>
#include<queue>
using namespace std;
class usernode{
    int id;
    string name;
    usernode* next;
public:
    usernode() : id(0), name("") {}
    friend class Facebook;
};
class Facebook{
    usernode * head[50];
    int n, v;
    char choice;
    int visited[50];
    stack<int> s;
    queue<int> q;
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
    for(int i=0; i<n; i++){
        if(visited[i] == 0 && i != v){
            DFS(i);
        }
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
        for (int i=n-1; i>=0; i--){
            if(visited[i] == 0 && i != v){
                s.push(i);
                visited[i] = 1;
            }
        }
    }while(!s.empty());
    cout << "NULL" << endl;
}
#if 0
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
        for(int i=0; i<n; i++){
            if
        }
    }
}
#endif
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
        cout << "6. Exit" << endl;
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
                cout << "Exiting program." << endl;
                return 0;  
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}