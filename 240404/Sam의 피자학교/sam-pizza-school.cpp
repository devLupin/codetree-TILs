#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int N, K, H = 1;
deque<int> dq[55];
int arr[55][55];

// 밀가루의 양의 최댓값과 최솟값의 차이가 k 이하가 되는 최소 연산 횟수를 출력하세요.

void Print() {
    cout << "\n\n";
    for (int i = H + 1; i > 0; i--) {
        for (auto& nxt : dq[i])
            cout << nxt << ' ';
        cout << '\n';
    }
}

void AddMin() {
    int cmp = *min_element(dq[1].begin(), dq[1].end());

    for (int& x : dq[1])
        if (x == cmp) x++;

    return;
}

void Rolling() {
    dq[H + 1].push_back(dq[H].front());
    dq[H].pop_front();

    while (true) {
        if (dq[1].size() - H < H) break;

        int len = H;
        while (len--) {
            for (int i = H; i > 0; i--) {
                dq[i + 1].push_front(dq[i].front());
                dq[i].pop_front();
            }
        }

        H++;
    }

    return;
}

void Pull() {
    for (int i = H; i > 0; i--) {
        int y_len = dq[i].size();

        for (int j = 0; j < y_len; j++) {
            int x = i;
            int y = j;

            if (y + 1 < y_len) {
                int nx = x;
                int ny = y + 1;
                int diff = abs(dq[x][y] - dq[nx][ny]) / 5;

                if (dq[x][y] > dq[nx][ny]) {
                    arr[x][y] -= diff;
                    arr[nx][ny] += diff;
                }
                else {
                    arr[x][y] += diff;
                    arr[nx][ny] -= diff;
                }
            }
            if (x - 1 > 0) {
                int nx = x - 1;
                int ny = y;
                int diff = abs(dq[x][y] - dq[nx][ny]) / 5;

                if (dq[x][y] > dq[nx][ny]) {
                    arr[x][y] -= diff;
                    arr[nx][ny] += diff;
                }
                else {
                    arr[x][y] += diff;
                    arr[nx][ny] -= diff;
                }
            }
        }
    }

    for (int i = H; i > 0; i--) {
        for (int j = 0; j < dq[i].size(); j++) {
            dq[i][j] += arr[i][j];
            arr[i][j] = 0;
        }
    }

    return;
}

void Flatten() {
    deque<int> tmp;
    int len = dq[1].size();

    for (int i = 0; i < len; i++) {
        for (int j = 1; j <= H; j++) {
            if (!dq[j].empty()) {
                tmp.push_back(dq[j].front());
                dq[j].pop_front();
            }
        }
    }

    dq[1] = tmp;
    H = 1;
    return;
}

void FoldHalf() {
    int len = dq[1].size() / 2;

    for(int i=0; i<len; i++) {
        dq[2].push_front(dq[1].front());
        dq[1].pop_front();
    }

    len /= 2;
    
    for (int i = 0; i < len; i++) {
        dq[3].push_front(dq[2].front());
        dq[2].pop_front();
    }
    for (int i = 0; i < len; i++) {
        dq[4].push_front(dq[1].front());
        dq[1].pop_front();
    }

    H = 4;
    return;
}

bool Run() {
    int mx = *max_element(dq[1].begin(), dq[1].end());
    int mn = *min_element(dq[1].begin(), dq[1].end());

    return (mx - mn) > K;
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> K;
    for (int x, i = 0; i < N; i++) {
        cin >> x;
        dq[1].push_back(x);
    }

    int ans = 0;
    while (Run()) {
        AddMin();
        Rolling();
        Pull();
        Flatten();
        FoldHalf();
        Pull();
        Flatten();
        ans++;
    }

    cout << ans;
    return 0;
}