#include <iostream>
#include <algorithm>
using namespace std;

const int SZ = 205;
const int dx[] = {-1,1,0,0};
const int dy[] = {0,0,-1,1};
int N, M, board[SZ][SZ], ans;
bool vis[SZ][SZ];

bool oom(int x, int y) {return x < 0 || y < 0 || x >= N || y >= M;}

void dfs(int cnt, int sum, int x, int y) {
    if(cnt == 4) {
        ans = max(ans, sum);
        return;
    }

    for(int dir=0; dir<4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if(!oom(nx, ny) && !vis[nx][ny]) {
            vis[nx][ny] = true;
            dfs(cnt + 1, sum + board[nx][ny], nx, ny);
            vis[nx][ny] = false;
        }
    }
}

void others(int x, int y) {
    if(!oom(x-1, y) && !oom(x+1, y) && !oom(x, y+1))
        ans = max(ans, board[x][y] + board[x-1][y] + board[x+1][y] + board[x][y+1]);
    if(!oom(x-1, y) && !oom(x+1, y) && !oom(x, y-1))
        ans = max(ans, board[x][y] + board[x-1][y] + board[x+1][y] + board[x][y-1]);
    if(!oom(x+1, y) && !oom(x, y+1) && !oom(x, y-1))
        ans = max(ans, board[x][y] + board[x+1][y] + board[x][y+1] + board[x][y-1]);
    if(!oom(x-1, y) && !oom(x, y+1) && !oom(x, y-1))
        ans = max(ans, board[x][y] + board[x-1][y] + board[x][y+1] + board[x][y-1]);
}

int main() {
    cin >> N >> M;
    for(int i=0; i<N; i++)
        for(int j=0; j<M; j++)
            cin >> board[i][j];
    
    for(int i=0; i<N; i++)
        for(int j=0; j<M; j++) {
            vis[i][j] = true;
            others(i,j);
            dfs(1, board[i][j], i, j);
            vis[i][j] = false;
        }

    cout << ans;
    return 0;
}