//#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

struct Node {
    int y, x, c;
};

const int dy[] = { -1,1,0,0 };
const int dx[] = { 0,0,-1,1 };
const int ddy[] = { -1,-1,1,1 };
const int ddx[] = { -1,1,1,-1 };

int n, m, k, c, ans;
int A[25][25];

void print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << A[i][j] << "\t";
        cout << '\n';
    }

    cout << '\n';
}

bool oom(int y, int x) { return y < 0 || x < 0 || y >= n || x >= n; }

void growth() {
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (A[y][x] > 0) {
                for (int dir = 0; dir < 4; dir++) {
                    int ny = y + dy[dir];
                    int nx = x + dx[dir];

                    if (!oom(ny, nx) && A[ny][nx] > 0) A[y][x]++;
                }
            }
        }
    }
}

void breed() {
    int tmp[25][25] = { 0, };

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (A[y][x] > 0) {
                int cnt = 0;
                
                for (int dir = 0; dir < 4; dir++) {
                    int ny = y + dy[dir];
                    int nx = x + dx[dir];

                    if (!oom(ny, nx) && A[ny][nx] == 0) cnt++;
                }

                if (cnt > 0) {
                    for (int dir = 0; dir < 4; dir++) {
                        int ny = y + dy[dir];
                        int nx = x + dx[dir];

                        if (!oom(ny, nx) && A[ny][nx] == 0) tmp[ny][nx] += A[y][x] / cnt;
                    }
                }
            }
        }
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (tmp[y][x] > 0) A[y][x] = tmp[y][x];
        }
    }
}

void spread() {
    int mx, sy, sx;
    mx = 0;
    sy = sx = -1;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (A[y][x] > 0) {
                int cmp = A[y][x];
                
                for (int dir = 0; dir < 4; dir++) {
                    int ny = y;
                    int nx = x;
                    for(int i=0; i<k; i++) {
                        ny += ddy[dir];
                        nx += ddx[dir];

                        if (oom(ny, nx)) break;
                        if (A[ny][nx] <= 0) break;

                        cmp += A[ny][nx];
                    }
                }

                if (cmp > mx) {
                    mx = cmp;
                    sy = y;
                    sx = x;
                }
            }
        }
    }

    ans += mx;
    
    A[sy][sx] = -2 - c;
    for (int dir = 0; dir < 4; dir++) {
        int ny = sy;
        int nx = sx;

        for (int i = 0; i < k; i++) {
            ny += ddy[dir];
            nx += ddx[dir];

            if (oom(ny, nx)) break;

            if (A[ny][nx] == 0 || A[ny][nx] <= -2) {
                A[ny][nx] = -2 - c;
                break;
            }
            if (A[ny][nx] == -1) break;

            A[ny][nx] = -2 - c;
        }
    }
}

void update() {
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (A[y][x] == -2) A[y][x] = 0;
            else if (A[y][x] < -2) A[y][x]++;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    //freopen("input.txt", "r", stdin);

    cin >> n >> m >> k >> c;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];

    while (m--) {
        growth();
        breed();
        spread();
        update();
    }
    cout << ans;

    return 0;
}