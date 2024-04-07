#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int,int>;

const int SZ = 30;
const int dx[] = {-1,1,0,0};
const int dy[] = {0,0,-1,1};

int N, board[SZ][SZ], next_board[SZ][SZ], Group[SZ][SZ];
bool vis[SZ][SZ];

vector<int> group_num;   // 그룹을 구성하는 숫자
vector<int> num_cols;       // 그룹을 구성하는 숫자의 개수
map<pii, int> num_adjs;      // i 그룹과 j 그룹의 맞닿은 변의 수

bool OOM(int x, int y) {return x < 0 || y < 0 || x >= N || y >= N;}

int GroupBFS(int x, int y, int num) {
    int cnt = 1;
    int target = board[x][y];
    queue<pii> q;

    q.push(make_pair(x, y));
    vis[x][y] = true;
    Group[x][y] = num;

    while(!q.empty()) {
        pii cur = q.front();
        q.pop();

        for(int dir=0; dir<4; dir++) {
            int nx = cur.X + dx[dir];
            int ny = cur.Y + dy[dir];

            if(!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] == target) {
                q.push(make_pair(nx, ny));
                vis[nx][ny] = true;
                Group[nx][ny] = num;
                cnt++;
            }
        }
    }

    return cnt;
}

void MakeAdj(int x, int y) {
    int target = Group[x][y];
    queue<pii> q;

    q.push(make_pair(x, y));
    vis[x][y] = true;

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();

        for (int dir=0; dir<4; dir++) {
            int nx = cur.X + dx[dir];
            int ny = cur.Y + dy[dir];

            if (!OOM(nx, ny)) {
                if(!vis[nx][ny] && Group[nx][ny] == target) {
                    q.push(make_pair(nx, ny));
                    vis[nx][ny] = true;
                }
                else if(!vis[nx][ny] && Group[nx][ny] != target) {
                    int nxt = Group[nx][ny];
                    num_adjs[make_pair(target, nxt)]++;
                }
            }
        }
    }
}

void Adjacency() {
    fill(&vis[0][0], &vis[N][N], false);

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            if(!vis[i][j]) MakeAdj(i, j);
}

void MakeGroup() {
    int num = 0;

    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            if(!vis[i][j]) { 
                int cols = GroupBFS(i, j, num++);
                num_cols.push_back(cols);
                group_num.push_back(board[i][j]);
            }
        }
    }
}

int GetScore() {
    int ret = 0;

    for(auto nxt : num_adjs) {
        int from = nxt.first.X;
        int to = nxt.first.Y;
        int adj = nxt.second;
        int f_cols = num_cols[from];
        int t_cols = num_cols[to];

        ret += (f_cols + t_cols) * group_num[from] * group_num[to] * adj;
    }

    return ret;
}

void Init() {
    group_num.clear();
    num_cols.clear();
    num_adjs.clear();

    fill(&next_board[0][0], &next_board[N][N], 0);
    fill(&Group[0][0], &Group[N][N], 0);
    fill(&vis[0][0], &vis[N][N], false);
}

void Solve() {
    Init();
    MakeGroup();
    Adjacency();
}

void RotateCross(int mid) {
    int i, j;

    j = 0;
    for(i = 0; i < N; i++) next_board[mid][j++] = board[i][mid];

    i = 0;
    for(j = N - 1; j >= 0; j--) next_board[i++][mid] = board[mid][j];
}

void RotateRange(int sx, int sy, int mid) {
    for(int x = sx; x < sx + mid; x++) {
        for(int y = sy; y < sy + mid; y++) {
            int nx = y - sy, ny = mid - x + sx - 1;
            next_board[nx + sx][ny + sy] = board[x][y];
        }
    }
}

void Rotate() {
    int mid = N / 2;

    RotateCross(mid);
    RotateRange(0, 0, mid);
    RotateRange(0, mid + 1, mid);
    RotateRange(mid + 1, 0, mid);
    RotateRange(mid + 1, mid + 1, mid);

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            board[i][j] = next_board[i][j];
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N;
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            cin >> board[i][j];
    
    Solve();
    int ans = GetScore();

    for(int i=0; i<3; i++) {
        Rotate();
        Solve();
        ans += GetScore();
    }

    cout << ans;
    return 0;
}