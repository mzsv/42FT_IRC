#include <bits/stdc++.h>

using namespace std;

#define ALL(x) (x).begin(),(x).end()

template<typename T> inline bool checkmin(T &a, T b) { if (b < a) { a = b; return 1; } return 0; }
template<typename T> inline bool checkmax(T &a, T b) { if (b > a) { a = b; return 1; } return 0; }

typedef long long ll;

vector<vector<int>> check(int s, int n) {
    for (int p = 0; p < 2; ++p) {
        vector<bool> v(n + 1, 0);
        vector<vector<int>> ans(s + 1);
        bool ok = true;
        int j = s;
        for (int i = 0; i < 2; ++i) {
            int y = (i + p) % 2;
            while (y <= s && j >= 0) {
                int z = n - j - y;
                if (z < 0) {
                    ok = 0;
                    break;
                }
                if (v[z]) {
                    ok = 0;
                    break;
                }
                v[z] = true;
                ans[j] = vector<int>({j, y, z});
                y += 2;
                j--;
            }
            if (!ok) {
                break;
            }
        }
        if (ok) {
            return ans;
        }
    }
    return vector<vector<int>>();
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> ans(1, vector<int>({0, 0, n}));
    for (int i = 1; i <= n; ++i) {
        vector<vector<int>> tmp = check(i, n);
        if (tmp.size()) {
            ans = tmp;
        }
    }
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i][0] << ' ' << ans[i][1] << ' ' << ans[i][2] << endl;
    }
}