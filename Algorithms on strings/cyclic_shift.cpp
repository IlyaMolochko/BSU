#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int get_shift(string &s, int n) {
    vector<int> p(s.size());
    int k = 0;
    int answ = -1;
    for (int i = 1; i < s.size(); ++i) {
        while (k > 0 and s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            ++k;
        }
        p[i] = k;
        if (p[i] == n) {
            answ = p.size() - i - 1;
        }
    }
    return answ;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    string s, t;
    cin >> n >> s >> t;
    string str = s + '$' + t + t;
    cout << get_shift(str, n) << '\n';
    return 0;
}
