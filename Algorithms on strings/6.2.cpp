#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int z_function(string_view s, vector<int> &lengths) {
    int l = 0;
    int r = 0;
    vector<int> z(s.size());
    int mx = 0;
    for (int i = 1; i < s.size(); ++i) {
        if (r >= i) {
            z[i] = min(z[i - l], r - i + 1);
        }
        while (z[i] + i < s.size() and s[z[i]] == s[z[i] + i]) {
            ++z[i];
        }
        if (z[i] + i - 1 > r) {
            l = i;
            r = z[i] + i - 1;
        }
        if (z[i] <= i) {
            mx = max(mx, min(lengths[lengths.size() - s.size() + i], z[i]));
            lengths[lengths.size() - s.size()] = max(z[i], lengths[lengths.size() - s.size()]);
        } else {
            mx = max(mx, min(lengths[lengths.size() - s.size() + i], i));
            lengths[lengths.size() - s.size()] = max(i, lengths[lengths.size() - s.size()]);
        }
    }
    return mx;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> s;
    string_view t(s);
    int max_len = 0;
    vector<int> lengths(s.size());
    string_view answ;
    for (int i = t.size() - 1; i >= 0; --i) {
        int length = z_function(t.substr(i), lengths);
        if (length > max_len) {
            max_len = length;
            answ = t.substr(i, max_len);
        }
    }
    cout << answ << '\n';
    return 0;
}
