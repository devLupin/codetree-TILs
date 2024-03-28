#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;

// ↑, ↖, ←, ↙, ↓, ↘, →, ↗
const int dx[] = { -1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,-1,-1,-1,0,1,1,1 };
const int ddx[] = { -1,0,1,0 };
const int ddy[] = { 0,-1,0,1 };

struct info { 
    int d, die; 
    bool alive, stun;
};

int M, T, px, py, ans, cmp = -1;
vector<info> monster[5][5], next_monster[5][5];
bool vis[7][7];
vector<int> routes;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > 4 || y > 4; }

bool Check(int x, int y) {
    if (OOM(x, y)) return false;
    if (x == px && y == py) return false;
    for (auto& nxt : monster[x][y])
        if (!nxt.alive) return false;
    return true;
}

void NextMonster() {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            monster[i][j] = next_monster[i][j];
            next_monster[i][j].clear();
        }
    }
}

void Copy() {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            for (auto& nxt : monster[i][j]) {
                if (!nxt.alive) continue;
                monster[i][j].push_back({nxt.d, 0, true, true});
            }
        }
    }
}

void MoveMonster() {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            for (auto& nxt : monster[i][j]) {
                if (nxt.stun || !nxt.alive) {
                    next_monster[i][j].push_back(nxt);
                    continue;
                }

                auto [x, y, dir] = make_tuple(i, j, nxt.d);
                int ndir = -1;

                for (int i = 0; i < 8; i++) {
                    int cdir = (dir + i) % 8;
                    if (Check(x + dx[cdir], y + dy[cdir])) {
                        ndir = cdir;
                        break;
                    }
                }

                if (ndir >= 0) {
                    nxt.d = ndir;
                    tie(x, y) = make_pair(x + dx[ndir], y + dy[ndir]);
                    next_monster[x][y].push_back(nxt);
                }
            }
        }
    }

    NextMonster();
}

void dfs(int x, int y, int cnt, int sum, vector<int> v) {
    if (cnt == 3) {
        if (cmp < sum) {
            cmp = sum;
            routes = v;
        }

        return;
    }

    // ↑, ←, ↓, →
    for (int dir : {0, 2, 4, 6}) {
        auto [nx, ny] = make_pair(x + dx[dir], y + dy[dir]);
        if (OOM(nx, ny) || vis[nx][ny]) continue;

        int sz = monster[nx][ny].size();

        v.push_back(dir);
        vis[nx][ny] = true;
        dfs(nx, ny, cnt + 1, sum + sz, v);
        v.pop_back();
        vis[nx][ny] = false;
    }
}

void MovePackman() {
    fill(&vis[1][1], &vis[5][5], false);

    vis[px][py] = true;
    dfs(px, py, 0, 0, {});

    // ↑, ↖, ←, ↙, ↓, ↘, →, ↗
    for (int& dir : routes) {
        px += dx[dir];
        py += dy[dir];

        for (auto& nxt : monster[px][py]) {
            if (nxt.stun) continue;
            nxt.die = 3;
            nxt.alive = false;
        }
    }
    
    cmp = -1;
}

void Done() {
    int cnt = 0;

    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            for (auto& nxt : monster[i][j]) {
                if (nxt.die > 0) nxt.die--;
                if (nxt.stun) nxt.stun = false;

                if (nxt.alive) cnt++;
                
                if (!nxt.alive && nxt.die == 0) continue;
                next_monster[i][j].push_back(nxt);
            }
        }
    }

    NextMonster();
    ans = cnt;
}

void Print() {
    cout << "\n\n";

    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            for (auto& nxt : monster[i][j]) {
                cout << '[' << i << ", " << j << "] - ";
                cout << ((nxt.alive) ? "alive" : "die") << ", ";
                cout << ((nxt.stun) ? "stun" : "no-stun") << "\n";
            }
        }
    }
}

int main(void)
{
    ios::sync_with_stdio(false); 
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> M >> T >> px >> py;
    for (int i = 0; i < M; i++) {
        int x, y, d;
        cin >> x >> y >> d;
        monster[x][y].push_back({ d - 1, 0, true, false });
    }

    while (T--) {
        Copy();
        MoveMonster();
        MovePackman();
        Done();
    }

    cout << ans;
    return 0;
}