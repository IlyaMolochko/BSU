#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    string u, w;
    cin >> u >> w;
    int I, D, S;
    cin >> I >> D >> S;
    vector<vector<int>> d(n + 1, vector<int>(m + 1));
    for (int i = 1; i < n + 1; ++i) {
        d[i][0] = d[i - 1][0] + D;
    }
    for (int j = 1; j < m + 1; ++j) {
        d[0][j] = d[0][j - 1] + I;
    }
    for (int i = 1; i < n + 1; ++i) {
        for (int j = 1; j < m + 1; ++j) {
            if (u[i - 1] != w[j - 1]) {
                d[i][j] = min({d[i - 1][j] + D, d[i][j - 1] + I, d[i - 1][j - 1] + S});
            } else {
                d[i][j] = d[i - 1][j - 1];
            }
        }
    }
    cout << d.back().back() << '\n';
    return 0;
}