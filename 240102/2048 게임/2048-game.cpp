#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using vvi = vector<vector<int>>;

int ans, n, m;
const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

bool oom(int x, int y) { return x < 0 || y < 0 || x >= n || y >= n; }

void print(vvi board) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
}

vvi move_up(vvi board) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] != 0) continue;

            int x = i + 1;
            while (!oom(x, j) && board[x][j] == 0) x++;
            if (!oom(x, j)) swap(board[x][j], board[i][j]);
        }
    }
    return board;
}
vvi move_down(vvi board) {
    for (int i = n - 2; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 0) continue;
            
            int x = i + 1;
            while (!oom(x, j) && board[x][j] == 0) x++;
            if (!oom(x - 1, j)) swap(board[x - 1][j], board[i][j]);
        }
    }
    return board;
}
vvi move_left(vvi board) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (board[i][j] != 0) continue;

            int y = j + 1;
            while (!oom(i, y) && board[i][y] == 0) y++;
            if (!oom(i, y)) swap(board[i][y], board[i][j]);
        }
    }
    return board;
}
vvi move_right(vvi board) {
    for (int i = 0; i < n; i++) {
        for (int j = n - 2; j >= 0; j--) {
            if (board[i][j] == 0) continue;

            int y = j + 1;
            while (!oom(i, y) && board[i][y] == 0) y++;
            if (!oom(i, y - 1)) swap(board[i][y - 1], board[i][j]);
        }
    }
    return board;
}

vvi add_up(vvi board) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == board[i + 1][j]) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
            }
        }
    }

    return board;
}
vvi add_down(vvi board) {
    for (int i = n - 2; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == board[i + 1][j]) {
                board[i + 1][j] *= 2;
                board[i][j] = 0;
            }
        }
    }

    return board;
}
vvi add_left(vvi board) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (board[i][j] == board[i][j + 1]) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
            }
        }
    }

    return board;
}
vvi add_right(vvi board) {
    for (int i = 0; i < n; i++) {
        for (int j = n - 2; j >= 0; j--) {
            if (board[i][j] == board[i][j + 1]) {
                board[i][j + 1] *= 2;
                board[i][j] = 0;
            }
        }
    }

    return board;
}

vvi move(int dir, vvi board) {

    if (dir == 0) {
        board = move_up(board);
        board = add_up(board);
        board = move_up(board);
    }
    else if (dir == 1) {
        board = move_down(board);
        board = add_down(board);
        board = move_down(board);
    }

    else if (dir == 2) {
        board = move_left(board);
        board = add_left(board);
        board = move_left(board);
    }

    else {
        board = move_right(board);
        board = add_right(board);
        board = move_right(board);
    }

    return board;
}

void dfs(int cnt, vvi board) {
    if (cnt == 5) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ans = max(board[i][j], ans);
        return;
    }

    for (int dir = 0; dir < 4; dir++) {
        vvi nxt = move(dir, board);
        dfs(cnt + 1, nxt);
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> n;
    
    vvi board;
    for (int i = 0; i < n; i++)
        board.push_back(vector<int>());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> m;
            board[i].push_back(m);
        }

    dfs(0, board);
    cout << ans;

    return 0;
}