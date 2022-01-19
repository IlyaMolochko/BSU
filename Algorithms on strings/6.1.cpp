#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int get_dif(string_view s) {
    int l = 0;
    int r = 0;
    int mx = 0;
    vector<int> z(s.size());
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
        mx = max(mx, z[i]);
    }
    return s.size() - mx;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> s;
    string_view t(s);
    int answ = 0;
    for (int i = t.size() - 1; i >= 0; --i) {
        answ += get_dif(t.substr(i));
    }
    cout << answ << '\n';
    return 0;
}
