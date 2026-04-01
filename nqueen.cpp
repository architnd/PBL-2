#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class NQueensSolver {
private:
    int n;            
    int x[100] = {0};      // x[k] = i means a queen is at row k, column i
    int solution_count;

public:
    NQueensSolver(int size) {
        n = size;
        solution_count = 0;
    }
    void printBoard() {
        cout << "Solution " << solution_count+1 << ":\n";
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (x[i] == j) {
                    cout << "@ ";
                } else {
                    cout << "* ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
        solution_count++;
    }

    bool place(int k, int i) {
        for (int j = 1; j <= k - 1; j++) {
            if ((x[j] == i) || (abs(x[j] - i) == abs(j - k))) {
                return false;
            }
        }
        return true;
    }

    void nQueen(int k) {
        for (int i = 1; i <= n; i++) {
            if (place(k, i)) {
                x[k] = i;
                if (k == n) {
                    printBoard();
                } else {

                    nQueen(k + 1);
                }
            }
        }
    }

    void solve() {
        nQueen(1);
        
        if (solution_count == 0) {
            cout << "No solutions exist for n = " << n << ".\n";
        } else {
            cout << "Total solutions found: " << solution_count << "\n";
        }
    }
};

int main() {
    int n;
    cout << "Enter the number of queens (n): ";
    cin >> n;

    NQueensSolver solver(n);
    solver.solve();

    return 0;
}