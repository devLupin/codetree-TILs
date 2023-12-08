#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int,int>;

int n, m, x, y, k, d;
int board[25][25];
int dice[7];

const int dx[] = {0,0,0,-1,1};
const int dy[] = {0,1,-1,0,0};
const int BOTTOM = 3, TOP = 1;

bool oom(int x, int y) {return x < 0 || y < 0 || x >= n || y >= m;}

void rolling(int dir) {
    int tmp;

    if(dir == 1) {
        tmp = dice[6];
        dice[6] = dice[1];
        dice[1] = dice[5];
        dice[5] = dice[3];
        dice[3] = tmp;
    }
    else if(dir == 2) {
        tmp = dice[5];
        dice[5] = dice[1];
        dice[1] = dice[6];
        dice[6] = dice[3];
        dice[3] = tmp;
    }
    else if(dir == 3) {
        tmp = dice[4];
        dice[4] = dice[1];
        dice[1] = dice[2];
        dice[2] = dice[3];
        dice[3] = tmp;
    }
    else {
        tmp = dice[2];
        dice[2] = dice[1];
        dice[1] = dice[4];
        dice[4] = dice[3];
        dice[3] = tmp;
    }
}

int move(int dir) {
    if(board[x][y] == 0) board[x][y] = dice[BOTTOM];
    else dice[BOTTOM] = board[x][y];

    if(oom(x+dx[dir], y+dy[dir])) return -1;

    rolling(dir);
    x += dx[dir];
    y += dy[dir];

    return dice[TOP];
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> n >> m >> x >> y >> k;
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            cin >> board[i][j];
    
    while(k--) {
        cin >> d;
        int ans = move(d);
        if(ans < 0) continue;
        cout << ans << '\n';
    }
    
    return 0;
}