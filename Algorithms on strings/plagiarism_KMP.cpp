#include <iostream>
#include <vector>

using namespace std;

int id(char c) {
    if (c >= 'a' and c <= 'z') {
        return c - 'a';
    } else {
        return 26 + c - 'A';
    }
}

bool is_uppercase(char c) {
    return c >= 'A' and c <= 'Z';
}

void get_array(string &s, vector<int> &f) {
    f.resize(s.size());
    vector<int> pos(52, -1);
    for (int i = 0; i < s.size(); ++i) {
        if (pos[id(s[i])] == -1) {
            f[i] = i + 1;
        } else {
            f[i] = i + 1 - pos[id(s[i])];
        }
        pos[id(s[i])] = i + 1;
    }
}

void p_function(string &s, vector<int> &f, size_t n) {
    vector<int> p(s.size());
    int k = 0;
    int answ = -1;
    for (int i = 1; i < s.size(); ++i) {
        while (k > 0 and (is_uppercase(s[i]) ^ is_uppercase(s[k]) or
               f[k] != min(k + 1, f[i]) or
                is_uppercase(s[i]) and is_uppercase(s[k]) and s[i] != s[k])) {
            k = p[k - 1];
        }
        if (s[i] == '$') {
            k = 0;
        } else if ((is_uppercase(s[i]) | is_uppercase(s[k])) == false
            and f[k] == min(k + 1, f[i])
            or is_uppercase(s[i]) and is_uppercase(s[k]) and s[i] == s[k]) {
            ++k;
        }
        p[i] = k;
        if (p[i] == n) {
            answ = i - 2 * n + 1;
            break;
        }
    }
    if (answ == -1) {
        cout << "Looks like something new." << '\n';
    } else {
        cout << "Plagiarism was detected at position " << answ << '.' << '\n';
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string s, p;
    cin >> s >> p;
    string t = p + "$" + s;
    vector<int> f;
    get_array(t, f);
    p_function(t, f, p.size());
    return 0;
}
