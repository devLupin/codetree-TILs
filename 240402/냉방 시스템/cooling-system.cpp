#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int,int>;

const int SZ = 25;
const int DIR_NUM = 4;
enum {LEFT, UP, RIGHT, DOWN};

// 좌, 상, 우, 하
const int dx[DIR_NUM] = {0,-1,0,1};
const int dy[DIR_NUM] = {-1,0,1,0};
// 좌상, 우상, 우하, 좌하
const int ddx[DIR_NUM] = {-1,-1,1,1};
const int ddy[DIR_NUM] = {-1,1,1,-1};

int N, M, K, A[SZ][SZ], B[SZ][SZ];
bool wall[SZ][SZ][4];
bool vis[SZ][SZ];

struct info {int x, y, d;};
vector<info> aircon;
vector<pii> office;


bool OOM(int x, int y) {return x < 1 || y < 1 || x > N || y > N;}

vector<int> DirList(int dir) {
    vector<int> dirs;

    if(dir == 0) dirs = {0, 3};
    if(dir == 1) dirs = {0, 1};
    if(dir == 2) dirs = {1, 2};
    if(dir == 3) dirs = {2, 3};

    return dirs;
}

// 에어컨 방향, 이동하려는 방향
bool Check(int x, int y, int dir, int ndir) {
    if (dir == LEFT) {
        if (ndir == 0) return !wall[x][y][UP] && !wall[x - 1][y][LEFT];
        if (ndir == 3) return !wall[x][y][DOWN] && !wall[x + 1][y][LEFT];
    }

    else  if (dir == UP) {
        if (ndir == 0) return !wall[x][y][LEFT] && !wall[x][y - 1][UP];
        if (ndir == 1) return !wall[x][y][RIGHT] && !wall[x][y + 1][UP];
    }

    else  if (dir == RIGHT) {
        if (ndir == 1) return !wall[x][y][UP] && !wall[x - 1][y][RIGHT];
        if (ndir == 2) return !wall[x][y][DOWN] && !wall[x + 1][y][RIGHT];
    }

    else  if (dir == DOWN) {
        if (ndir == 2) return !wall[x][y][RIGHT] && !wall[x][y + 1][DOWN];
        if (ndir == 3) return !wall[x][y][LEFT] && !wall[x][y - 1][DOWN];
    }
}

void Spread(int x, int y, int dir) {
    queue<info> q;

    auto ddir = DirList(dir);

    q.push({x, y, 5});
    vis[x][y] = true;
    A[x][y] += 5;

    while(!q.empty()) {
        info cur = q.front();
        q.pop();

        if(cur.d == 1) continue;

        int nx = cur.x + dx[dir];
        int ny = cur.y + dy[dir];
        int nc = cur.d - 1;

        if(!OOM(nx, ny) && !vis[nx][ny] && !wall[cur.x][cur.y][dir]) {
            q.push({nx, ny, nc});
            vis[nx][ny] = true;
            A[nx][ny] += nc;
        }

        for(int nd : ddir) {
            nx = cur.x + ddx[nd];
            ny = cur.y + ddy[nd];
            
            if(!OOM(nx, ny) && !vis[nx][ny] && Check(cur.x, cur.y, dir, nd)) {
                q.push({nx, ny, nc});
                vis[nx][ny] = true;
                A[nx][ny] += nc;
            }
        }
    }
}

void Cooling() {
    for(auto& cur : aircon) {
        int nx = cur.x + dx[cur.d];
        int ny = cur.y + dy[cur.d];
        
        fill(&vis[1][1], &vis[N+1][N+1], false);
        Spread(nx, ny, cur.d);
    }
}

void Mix() {
    for(int x=1; x<=N; x++) {
        for(int y=1; y<=N; y++) {
            for(int dir : {RIGHT, DOWN}) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];

                if(OOM(nx, ny) || wall[x][y][dir]) continue;

                int diff = abs(A[x][y] - A[nx][ny]) / 4;

                if(A[x][y] > A[nx][ny]) {
                    B[x][y] -= diff;
                    B[nx][ny] += diff;
                }
                else {
                    B[x][y] += diff;
                    B[nx][ny] -= diff;
                }
            }
        }
    }

    for(int x=1; x<=N; x++) {
        for(int y=1; y<=N; y++) {
            A[x][y] += B[x][y];
            B[x][y] = 0;
        }
    }
}

void Decrease() {
    A[1][1] = max(0, A[1][1] - 1);
    A[1][N] = max(0, A[1][N] - 1);
    A[N][1] = max(0, A[N][1] - 1);
    A[N][N] = max(0, A[N][N] - 1);

    for(int i=2; i<N; i++) {
        A[i][1] = max(0, A[i][1] - 1);
        A[i][N] = max(0, A[i][N] - 1);
    }
    for(int j=2; j<N; j++) {
        A[1][j] = max(0, A[1][j] - 1);
        A[N][j] = max(0, A[N][j] - 1);
    }
}

bool Satisfied() {
    for(auto& nxt : office)
        if(A[nxt.X][nxt.Y] < K) return false;
    return true;
}

void Print() {
    cout << "\n\n";
    for(int i=1; i<=N; i++) {
        for(int j=1; j<=N; j++)
            cout << A[i][j] << ' ';
        cout << '\n';
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;

    for(int i=1; i<=N; i++) {
        for(int j=1; j<=N; j++) {
            int cur;
            cin >> cur;

            if(cur == 1) office.push_back({i, j});
            else if(cur >= 2)
                aircon.push_back({i, j, cur - 2});
            
            A[i][j] = 0;
        }
    }

    for(int x, y, s; M--;) {
        cin >> x >> y >> s;

        if(s == 0) {
            wall[x][y][1] = true;
            wall[x-1][y][3] = true;
        }
        else {
            wall[x][y][0] = true;
            wall[x][y-1][2] = true;
        }
    }

    int k = 0, ans = -1;
    while(k++ <= 100) {
        Cooling();
        Mix();
        Decrease();
        if(Satisfied()) {
            ans = k;
            break;
        }
    }

    cout << ans;
    return 0;
}