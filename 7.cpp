//Largest common subsequence
#include <iostream>
#include <string>
using namespace std;
class sequence
{
    string s1, s2, lcs;
    int M[100][100];
    int B[100][100];
public:
    sequence(){
        s1 = "";
        s2 = "";
    }
    void input();
    void solve();
    void display();
};
void sequence::input(){
    cout << "Enter first string: ";
    cin >> s1;
    cout << "Enter second string: ";
    cin >> s2;
}
void sequence::solve(){
    int m = s1.length();
    int n = s2.length();
    for (int i = 0; i <= m; i++){
        M[i][0] = 0;
    }
    for (int j = 0; j <= n; j++){
        M[0][j] = 0;
    }
    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++){
            if (s1[i - 1] == s2[j - 1]){
                M[i][j] = M[i - 1][j - 1] + 1;
                B[i][j] = 0;
            }
            else if (M[i - 1][j] >= M[i][j - 1]){
                M[i][j] = M[i -1][j];
                B[i][j] = 1;
            } else {
                M[i][j] = M[i][j - 1];
                B[i][j] = -1;
            }
        }
    }
    while(m > 0 && n > 0){
        if (B[m][n] == 0){
            lcs = s1[m - 1] + lcs;
            m--;
            n--;
        }
        else if (B[m][n] == 1){
            m--;
        }
        else if (B[m][n] == -1){
            n--;
        }
    }
    cout << "\nSolved LCS.\n";
}
void sequence::display(){
    int m = s1.length();
    int n = s2.length();
    cout << "\nLCS Matrix:\n";
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++){
            cout << M[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\nDirection Matrix:\n";
    for (int i = 0; i <= m; i++){
        for (int j = 0; j <= n; j++){
            cout << B[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\nLongest Common Subsequence: " << lcs << "\n";
}
int main(){
    sequence s;
    while(true){
        cout << "\nMenu:\n";
        cout << "1. Input Strings\n";
        cout << "2. Solve LCS\n";
        cout << "3. Display\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice){
            case 1:
                s.input();
                break;
            case 2:
                s.solve();
                break;
            case 3:
                s.display();
                break;
            case 4:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
    s.input();
    s.solve();
}