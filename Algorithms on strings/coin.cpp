#include <iostream>
#include <vector>

using namespace std;

void p_function(string &s, vector<int> &p) {
    int k = 0;
    for (int i = 1; i < s.size(); ++i) {
        while (k > 0 and s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            ++k;
        }
        p[i] = k;
    }
}

void print(__int128 x) {
    if (x > 9) {
    	print(x / 10);
    }
    putchar(x % 10 + '0');
}

int main(){
	ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("coin.in", "r", stdin);
    freopen("coin.out", "w", stdout);
    string s;
    cin >> s;
    vector<int> p(s.size());
    p_function(s, p);
    vector<__int128> pw(s.size() + 1);
    pw[0] = 1;
    for (int i = 1; i <= s.size(); ++i) {
    	pw[i] = (pw[i - 1] << 1);
    }
    int i = s.size();
    __int128 answ = 0;
    while (i > 0) {
    	answ += pw[i];
    	i = p[i - 1];
    }
    print(answ);
	return 0;
}