#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <stack>
#include <cstring>
#include <climits>

using namespace std;

#define ROW 6
#define COL 6

typedef pair<int, int> Pair;
typedef pair<int, pair<int, int>> pPair;

struct cell {
    int parent_i, parent_j;
    int f, g, h;
};

bool isRange(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isBlock(int grid[ROW][COL], int row, int col) {
    return grid[row][col] == 1;
}

bool isFinish(int row, int col, Pair pnt_finish) {
    return row == pnt_finish.first && col == pnt_finish.second;
}

int calculateH(int row, int col, Pair pnt_finish) {
    return (abs(row - pnt_finish.first) + abs(col - pnt_finish.second)) * 10;
}

void printPath(cell pntAtributes[ROW][COL], Pair pnt_start, Pair pnt_finish, int myArea[ROW][COL]) {
    int row = pnt_finish.first;
    int col = pnt_finish.second;
    stack<Pair> Path;

    while (!(pntAtributes[row][col].parent_i == row && pntAtributes[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = pntAtributes[row][col].parent_i;
        int temp_col = pntAtributes[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    Path.push(make_pair(row, col));

    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        myArea[p.first][p.second] = 3;
    }

    myArea[pnt_start.first][pnt_start.second] = 8;
    myArea[pnt_finish.first][pnt_finish.second] = 9;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (myArea[i][j] == 8) cout << "A ";
            else if (myArea[i][j] == 9) cout << "B ";
            else if (myArea[i][j] == 3) cout << "* ";
            else if (myArea[i][j] == 0) cout << "X ";
            else cout << ". ";
        }
        cout << "\n";
    }
}

void aStarAlgoritm(int myArea[ROW][COL], Pair pnt_start, Pair pnt_finish) {
    if (!isRange(pnt_start.first, pnt_start.second) || !isRange(pnt_finish.first, pnt_finish.second)) return;
    if (!isBlock(myArea, pnt_start.first, pnt_start.second) || !isBlock(myArea, pnt_finish.first, pnt_finish.second)) return;
    if (isFinish(pnt_start.first, pnt_start.second, pnt_finish)) return;

    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
    cell pntAtributes[ROW][COL];

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            pntAtributes[i][j].parent_i = -1;
            pntAtributes[i][j].parent_j = -1;
            pntAtributes[i][j].f = INT_MAX;
            pntAtributes[i][j].g = INT_MAX;
            pntAtributes[i][j].h = INT_MAX;
        }
    }

    int i = pnt_start.first, j = pnt_start.second;
    pntAtributes[i][j].parent_i = i;
    pntAtributes[i][j].parent_j = j;
    pntAtributes[i][j].f = 0;
    pntAtributes[i][j].g = 0;
    pntAtributes[i][j].h = 0;

    set<pPair> openList;
    openList.insert(make_pair(0, make_pair(i, j)));

    int dRow[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dCol[] = {0, 0, 1, -1, 1, -1, 1, -1};

    while (!openList.empty()) {
        pPair p = *openList.begin();
        openList.erase(openList.begin());
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        for (int k = 0; k < 8; k++) {
            int di = dRow[k];
            int dj = dCol[k];
            if (isRange(i + di, j + dj)) {
                if (isFinish(i + di, j + dj, pnt_finish)) {
                    pntAtributes[i + di][j + dj].parent_i = i;
                    pntAtributes[i + di][j + dj].parent_j = j;
                    printPath(pntAtributes, pnt_start, pnt_finish, myArea);
                    return;
                } else if (!closedList[i + di][j + dj] && isBlock(myArea, i + di, j + dj)) {
                    int gNew = pntAtributes[i][j].g + 10;
                    int hNew = calculateH(i + di, j + dj, pnt_finish);
                    int fNew = gNew + hNew;

                    if (pntAtributes[i + di][j + dj].f == INT_MAX || pntAtributes[i + di][j + dj].f > fNew) {
                        openList.insert(make_pair(fNew, make_pair(i + di, j + dj)));
                        pntAtributes[i + di][j + dj].f = fNew;
                        pntAtributes[i + di][j + dj].g = gNew;
                        pntAtributes[i + di][j + dj].h = hNew;
                        pntAtributes[i + di][j + dj].parent_i = i;
                        pntAtributes[i + di][j + dj].parent_j = j;
                    }
                }
            }
        }
    }
}

int main() {
    int myArea[ROW][COL];
    ifstream potik("Lab6.txt");
    
    if (potik.is_open()) {
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                potik >> myArea[i][j];
            }
        }
    } else {
        int defaultArea[ROW][COL] = {
            {1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 0},
            {1, 1, 0, 1, 1, 1},
            {1, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 0, 1}
        };
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                myArea[i][j] = defaultArea[i][j];
            }
        }
    }
    
    Pair pnt_start = make_pair(0, 0);
    Pair pnt_finish = make_pair(5, 5);
    aStarAlgoritm(myArea, pnt_start, pnt_finish);
    
    return 0;
}