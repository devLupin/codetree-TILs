#include <bits/stdc++.h>
using namespace std;

const int SZ = 105;
const int dx[] = {0,-1,0,1};
const int dy[] = {1,0,-1,0};
int n, x, y, d, g;
bool board[SZ][SZ];
vector<int> dir;

int count() {
    int ret = 0;

    for(int i=0; i<SZ; i++)
        for(int j=0; j<SZ; j++)
            ret += board[i][j] && board[i+1][j] && board[i][j+1] && board[i+1][j+1];
    
    return ret;
}

void make() {
    int sz = dir.size();
    for(int i=sz-1; i>=0; i--) {
        int nd = (dir[i] + 1) % 4;

        x += dx[nd];
        y += dy[nd];
        board[x][y] = true;

        dir.push_back(nd);
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    
    while(n--) {
        dir.clear();

        cin >> x >> y >> d >> g;

        board[x][y] = true;

        x += dx[d];
        y += dy[d];
        board[x][y] = true;

        dir.push_back(d);
        while(g--) make();
    }

    cout << count();
    return 0;
}