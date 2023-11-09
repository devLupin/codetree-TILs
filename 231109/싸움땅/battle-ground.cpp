//#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

const int SZ = 25;
int N, M, K;

const int dy[] = { -1,0,1,0 };
const int dx[] = { 0,1,0,-1 };

int A[SZ][SZ];

struct player {
    int y, x, d, stat, gun;
};

vector<int> gun[SZ][SZ];
vector<player> info;
vector<int> ans;

void print() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++)
            cout << A[i][j] << ' ';
        cout << '\n';
    }
    cout << '\n';
}

bool oom(int y, int x) { return y < 1 || x < 1 || y > N || x > N; }

void set_gun(int idx, int y, int x) {
    if (gun[y][x].empty()) return;

    gun[y][x].push_back(info[idx].gun);
    sort(gun[y][x].begin(), gun[y][x].end());

    info[idx].gun = gun[y][x].back();
    gun[y][x].pop_back();
}

void move(int idx, int y, int x) {
    auto& cur = info[idx];
    info[idx].y = y;
    info[idx].x = x;
}

int get_fight_player(int idx, int y, int x) {
    for (int i = 0; i < info.size(); i++) {
        if (idx == i) continue;
        auto cmp = info[i];
        if (y == cmp.y && x == cmp.x) return i;
    }
}

void move_losser(int idx) {
    int y = info[idx].y;
    int x = info[idx].x;
    int d = info[idx].d;

    for (int i = 0; i < 4; i++) {
        int nd = (d + i) % 4;
        int ny = y + dy[nd];
        int nx = x + dx[nd];

        if (!oom(ny, nx) && A[ny][nx] == 0) {
            y = ny;
            x = nx;
            d = nd;
            break;
        }
    }
    info[idx].d = d;

    A[y][x] = idx + 1;
    move(idx, y, x);
    set_gun(idx, y, x);
}

void fight(int idx1, int idx2, int y, int x) {
    int stat1 = info[idx1].stat;
    int stat2 = info[idx2].stat;
    int score1 = stat1 + info[idx1].gun;
    int score2 = stat2 + info[idx2].gun;

    if (score1 == score2) {  // 동일한 공격력
        score1 = info[idx1].stat;
        score2 = info[idx2].stat;
    }

    int win_idx, loss_idx, score;

    if (score1 > score2) {
        win_idx = idx1;
        loss_idx = idx2;
    }
    else {
        win_idx = idx2;
        loss_idx = idx1;
    }

    ans[win_idx] += abs((stat1 + info[idx1].gun) - (stat2 + info[idx2].gun));

    // 총 내려놔라
    int loss_gun = info[loss_idx].gun;
    info[loss_idx].gun = 0;
    gun[y][x].push_back(loss_gun);

    A[info[loss_idx].y][info[loss_idx].x] = 0;
    A[info[win_idx].y][info[win_idx].x] = 0;

    A[y][x] = win_idx + 1;
    move(win_idx, y, x);
    set_gun(win_idx, y, x);

    move_losser(loss_idx);
}

void solve() {
    for (int i = 0; i < M; i++) {
        auto& cur = info[i];
        int d = cur.d;

        int ny = cur.y + dy[d];
        int nx = cur.x + dx[d];

        if (oom(ny, nx)) {
            d = (d < 2) ? (d + 2) : (d - 2);

            ny = cur.y + dy[d];
            nx = cur.x + dx[d];
        }
        info[i].d = d;

        A[cur.y][cur.x] = 0;
        move(i, ny, nx);

        if (A[ny][nx] > 0) {  // 맞짱
            int c_idx = get_fight_player(i, ny, nx);
            fight(i, c_idx, ny, nx);
        }
        else {
            A[ny][nx] = i + 1;
            set_gun(i, ny, nx);
        }
    }
}

int main(void) {
    //freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;
    
    for (int i = 1; i <= N; i++)
        for (int tmp, j = 1; j <= N; j++) {
            cin >> tmp;
            if (tmp > 0) gun[i][j].push_back(tmp);
        }

    for (int i = 0; i < M; i++) {
        int y, x, d, s;
        cin >> y >> x >> d >> s;
        info.push_back({ y,x,d,s,0 });
        ans.push_back(0);

        A[y][x] = ans.size();
    }

    while (K--) solve();
    for (auto nxt : ans) cout << nxt << ' ';
}