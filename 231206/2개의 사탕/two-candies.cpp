#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int, int>;

const int dy[] = { -1,1,0,0 };
const int dx[] = { 0,0,-1,1 };
vector<string> board;
int n, m, ans = 11;

char get_first(int dir, pii red, pii blue) {
    int ry, rx, by, bx;
    tie(ry, rx) = red;
    tie(by, bx) = blue;

    if (dir == 0) return ry < by ? 'R' : 'B';
    else if (dir == 1) return ry > by ? 'R' : 'B';
    else if (dir == 2) return rx < bx ? 'R' : 'B';
    else return rx > bx ? 'R' : 'B';
}

bool oom(int y, int x) { return y < 0 || x < 0 || y >= n || x >= n; }

vector<string> move_candy(int dir, pii pos, vector<string> prev) {
    int y, x;
    tie(y, x) = pos;
    char c = prev[y][x];
    prev[y][x] = '.';

    while (true) {
        y += dy[dir];
        x += dx[dir];

        if (prev[y][x] == 'O') break;
        if (oom(y, x) || prev[y][x] != '.') {
            y -= dy[dir];
            x -= dx[dir];
            break;
        }
    }

    if (prev[y][x] != 'O')
        prev[y][x] = c;

    return prev;
}

bool check(vector<string> tmp, char c) {
    for (int i = 0; i < tmp.size(); i++)
        for (int j = 0; j < tmp[i].size(); j++)
            if (tmp[i][j] == c) return true;
    return false;
}

vector<string> move(int cnt, int dir, vector<string> prev) {
    pii red, blue;

    for (int i = 0; i < prev.size(); i++)
        for (int j = 0; j < prev[i].size(); j++) {
            if (prev[i][j] == 'R') red = { i, j };
            else if (prev[i][j] == 'B') blue = { i, j };
        }

    char first = get_first(dir, red, blue);

    if (first == 'R') {
        prev = move_candy(dir, red, prev);
        prev = move_candy(dir, blue, prev);
    }
    else {
        prev = move_candy(dir, blue, prev);
        prev = move_candy(dir, red, prev);
    }

    bool r_chk = check(prev, 'R');
    bool b_chk = check(prev, 'B');

    if (!r_chk && !b_chk) return { "" };
    else if (r_chk && b_chk) return prev;
    else if (!r_chk) {
        ans = min(ans, cnt);
        return { "" };
    }
    return { "" };
}

void solve(int cnt, vector<string> prev) {
    if (cnt > 10 && cnt > ans) return;

    for (int dir = 0; dir < 4; dir++) {
        auto nxt = move(cnt, dir, prev);
        if (nxt[0] == "") continue;
        solve(cnt + 1, nxt);
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> n >> m;
    for (int x = 0; x < n; x++) {
        string s;
        cin >> s;
        board.push_back(s);
    }

    solve(1, board);
    cout << ans;

    return 0;
}