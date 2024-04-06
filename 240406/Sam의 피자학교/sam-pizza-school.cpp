#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int,int>;

//k 이하가 되는 최소 연산 횟수를 출력
//최댓값과 최솟값의 차이 k
int N, K, H;  // 100, 5
deque<int> pizza[105];
int updat[105][105];

const int dx[] = {-1, 0};
const int dy[] = {0, +1};

void Print() {
    for(int i=H; i>=0; i--) {
        for(int nxt : pizza[i]) cout << nxt << ' ';
        cout << '\n';
    }
    cout << "\n";
}

void AddFlour() {
    int mn = *min_element(pizza[0].begin(), pizza[0].end());

    for(int& nxt : pizza[0])
        if(nxt == mn) nxt++;
    
    return;
}

void Rolling() {
    H = 1;
    pizza[H].push_back(pizza[0].front());
    pizza[0].pop_front();
    int len = pizza[H].size();

    while(pizza[0].size() - pizza[H].size() >= H + 1) {
        vector<deque<int>> v;
        int cnt = pizza[H].size();

        for(int j=0; j<cnt; j++) {
            deque<int> tmp;

            for(int i=H; i>=0; i--) {
                tmp.push_front(pizza[i].front());
                pizza[i].pop_front();    
            }

            v.push_back(tmp);
        }

        for(int i=0; i<v.size(); i++)
            pizza[cnt--] = v[i];

        H = v.size();
    }
}

void Update() {
    for(int i=H; i>=0; i--) {
        int sz = pizza[i].size();
        for(int j=0; j<sz; j++) {
            for(int k=0; k<2; k++) {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if(nx < 0 || ny < 0 || nx > H || ny >= sz) continue;

                int diff = abs(pizza[i][j] - pizza[nx][ny]) / 5;

                if(pizza[i][j] > pizza[nx][ny]) {
                    updat[i][j] -= diff;
                    updat[nx][ny] += diff;
                }
                else {
                    updat[i][j] += diff;
                    updat[nx][ny] -= diff;
                }
            }
        }
    }

    // 상하좌우 밀가루 갱신
    for(int i=H; i>=0; i--) {
        for(int j=0; j<pizza[i].size(); j++) {
            pizza[i][j] += updat[i][j];
            updat[i][j] = 0;
        }
    }
}

void Flatten() {
    deque<int> nxt;
    int len = pizza[H].size();
    
    for(int j=0; j<len; j++) {
        for(int i=0; i<=H; i++) {
            nxt.push_back(pizza[i].front());
            pizza[i].pop_front();
        }
    }

    while(!pizza[0].empty()) {
        nxt.push_back(pizza[0].front());
        pizza[0].pop_front();
    }

    pizza[0] = nxt;
}

void Pulling() {
    Update();
    Flatten();
    H = 0;
}

void FoldHalf() {
    int sz = pizza[0].size() / 2;
    
    // 2층으로
    for(int i=0; i<sz; i++) {
        pizza[1].push_front(pizza[0].front());
        pizza[0].pop_front();
    }
    
    // 4층으로
    sz /= 2;
    for(int i=0; i<sz; i++) {
        pizza[2].push_front(pizza[1].front());
        pizza[1].pop_front();
    }
    for(int i=0; i<sz; i++) {
        pizza[3].push_front(pizza[0].front());
        pizza[0].pop_front();
    }

    H = 3;
}

bool Check() {
    int mn = *min_element(pizza[0].begin(), pizza[0].end());
    int mx = *max_element(pizza[0].begin(), pizza[0].end());
    return (mx - mn) <= K;
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> K;
    for(int x, i=0; i<N; i++) {
        cin >> x;
        pizza[0].push_back(x);
    }

    int ans = 0;

    while(!Check()) {
        AddFlour();
        Rolling();
        Pulling();
        FoldHalf();
        Pulling();
        ans++;
    }
    
    cout << ans;
    return 0;
}