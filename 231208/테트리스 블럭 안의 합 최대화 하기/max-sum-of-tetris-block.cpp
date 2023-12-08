#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int,int>;

int n, m, ans, A[205][205];
bool vis[205][205];

const int dx[] = {-1,1,0,0};
const int dy[] = {0,0,-1,1};

bool oom(int x, int y) {return x < 0 || y < 0 || x >= n || y >= m;}

void dfs(int x, int y, int cnt, int sum) {
    if(cnt == 4) {
        ans = max(ans, sum);
        return;
    }

    for(int dir=0; dir<4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if(oom(nx, ny) || vis[nx][ny]) continue;

        vis[nx][ny] = true;
        dfs(nx, ny, cnt+1, sum+A[nx][ny]);
        vis[nx][ny] = false;
    }
}

void block(int x, int y) {
    int x1, x2, x3, y1, y2, y3;

    tie(x1, y1) = make_pair(x-1, y);
    tie(x2, y2) = make_pair(x+1, y);
    tie(x3, y3) = make_pair(x, y+1);
    if(!oom(x1, y1) && !oom(x2, y2) && !oom(x3, y3))
        ans = max(ans, A[x][y]+A[x1][y1]+A[x2][y2]+A[x3][y3]);

    tie(x1, y1) = make_pair(x, y-1);
    tie(x2, y2) = make_pair(x, y+1);
    tie(x3, y3) = make_pair(x+1, y);
    if(!oom(x1, y1) && !oom(x2, y2) && !oom(x3, y3))
        ans = max(ans, A[x][y]+A[x1][y1]+A[x2][y2]+A[x3][y3]);

    tie(x1, y1) = make_pair(x, y-1);
    tie(x2, y2) = make_pair(x-1, y);
    tie(x3, y3) = make_pair(x+1, y);
    if(!oom(x1, y1) && !oom(x2, y2) && !oom(x3, y3))
        ans = max(ans, A[x][y]+A[x1][y1]+A[x2][y2]+A[x3][y3]);
    
    tie(x1, y1) = make_pair(x, y-1);
    tie(x2, y2) = make_pair(x, y+1);
    tie(x3, y3) = make_pair(x-1, y);
    if(!oom(x1, y1) && !oom(x2, y2) && !oom(x3, y3))
        ans = max(ans, A[x][y]+A[x1][y1]+A[x2][y2]+A[x3][y3]);
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> n >> m;
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            cin >> A[i][j];

    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++) {
            dfs(i, j, 0, 0);
            block(i, j);
        }

    cout << ans;
    
    return 0;
}