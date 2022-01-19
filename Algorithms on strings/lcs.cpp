#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void LCS(vector<int> &a, vector<int> &b) {
    vector<vector<int>> dp(a.size(), vector<int>(b.size()));
    int mx = 0;
    for (int i = 0; i < b.size(); ++i) {
        mx = max(mx, 0 + (b[i] == a[0]));
        dp[0][i] = mx;
    }
    for (int i = 1; i < a.size(); ++i) {
        if (a[i] == b[0]) {
            dp[i][0] = 1;
        } else {
            dp[i][0] = dp[i - 1][0];
            for (int j = 1; j < b.size(); ++j) {
                if (a[i] == b[j]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max({dp[i][j - 1], dp[i - 1][j - 1], dp[i - 1][j]});
                }
            }
        }
    }
    for (auto &row : dp) {
        for (auto i : row) {
            cout << i << ' ';
        }
        cout << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &i : a) {
        cin >> i;
    }
    vector<int> b(n);
    for (auto &i : b) {
        cin >> i;
    }
    LCS(a, b);
    return 0;
}