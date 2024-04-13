#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 25;
const int MAX_M = 7;
const int DIR_NUM = 4;

const int dx[DIR_NUM] = { 1,0,-1,0 };
const int dy[DIR_NUM] = { 0,1,0,-1 };

const int ddx[DIR_NUM] = { 0,-1,0,1 };
const int ddy[DIR_NUM] = { 1,0,-1,0 };

int N, M, K, ans;
int board[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
vector<deque<pii>> group;
int rx, ry, rd, rd_cnt;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Search(int x, int y, deque<pii> cur) {
    if (board[x][y] == 3) {
        group.push_back(cur);
        return;
    }

    vector<pii> tmp;

    for (int dir = 0; dir < DIR_NUM; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (!OOM(nx, ny) && board[nx][ny] > 0 && !vis[nx][ny])
            tmp.push_back({ board[nx][ny], dir });
    }

    sort(tmp.begin(), tmp.end());

    int dir = tmp[0].second;
    auto [nx, ny] = make_pair(x + dx[dir], y + dy[dir]);

    cur.push_back({ nx, ny });
    vis[nx][ny] = true;
    Search(nx, ny, cur);
}

void MakeG() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 1) {
                vis[i][j] = true;
                Search(i, j, { {i,j} });
            }
}

void MoveG() {
    for (auto& cur : group) {
        auto [hx, hy] = cur.front();
        auto [tx, ty] = cur.back();
        cur.pop_back();

        board[tx][ty] = 4;

        for (int k = 0; k < DIR_NUM; k++) {
            int nx = hx + dx[k];
            int ny = hy + dy[k];

            if (!OOM(nx, ny) && board[nx][ny] >= 3) {
                cur.push_front({nx, ny});
                break;
            }
        }

        tie(hx, hy) = cur.front();
        tie(tx, ty) = cur.back();
        
        for (int i = 1; i <= cur.size() - 1; i++) {
            pii pos = cur[i];
            board[pos.X][pos.Y] = 2;
        }
        board[hx][hy] = 1;
        board[tx][ty] = 3;
    }
}

tiii Round() {
    auto [x, y, d] = make_tuple(rx, ry, rd);

    rx += dx[rd];
    ry += dy[rd];
    rd_cnt++;

    if (rd_cnt == N) {
        rx -= dx[rd];
        ry -= dy[rd];

        rd = (rd + 1 + DIR_NUM) % DIR_NUM;
        rd_cnt = 0;
    }

    return make_tuple(x, y, d);
}

void Attack(int x, int y, int d) {
    bool flag = false;

    while (!OOM(x, y)) {
        if (board[x][y] > 0 && board[x][y] < 4) {
            flag = true;
            break;
        }
        x += ddx[d];
        y += ddy[d];
    }

    if (flag)
        for (auto& cur : group) {
            int num = 1;
            for (pii pos : cur) {
                if (x == pos.X && y == pos.Y) {
                    pii head = cur.front();
                    pii tail = cur.back();
                    cur.pop_front();
                    cur.pop_back();

                    swap(board[head.X][head.Y], board[tail.X][tail.Y]);
                    cur.push_back(head);
                    cur.push_front(tail);

                    ans += num * num;
                    return;
                }
                num++;
            }
        }
}

void Init() {
    group.clear();
    fill(&vis[0][0], &vis[N][N], false);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> board[i][j];

    while (K--) {
        MakeG();
        MoveG();
        auto [x, y, d] = Round();
        Attack(x, y, d);
        Init();
    }
    
    cout << ans;
    return 0;
}