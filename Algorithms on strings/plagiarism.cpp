//#include "profile.h"

#include <algorithm>
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

struct st {
    int64_t h1;
    int64_t h2;
    char c;
    int i;
};

bool check(string &s, string &p, int l) {
    vector<char> dict(52, ' ');
    for (int i = 0; i < p.size(); ++i) {
        if (is_uppercase(s[i + l])
            and is_uppercase(p[i])
            and s[i + l] != p[i]) {
            return false;
        }
        if (dict[id(s[i + l])] == ' ') {
            dict[id(s[i + l])] = p[i];
        } else if (dict[id(s[i + l])] != p[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
//    LOG_DURATION("time");
    string s, p;
    cin >> s >> p;
    if (p.size() > s.size()) {
        cout << "Looks like something new." << '\n';
        return 0;
    }
    pair<int64_t, int64_t> mod = {1'000'000'009, 1'000'000'007};
    pair<int64_t, int64_t> x = {7340033, 263};
    vector<st> letters;
    letters.reserve(52);
    vector<st> h(s.size());
    vector<int> chars(52, -1);
    pair<int64_t, int64_t> d = {1, 1};
    for (int i = 0; i < p.size(); ++i) {
        if (chars[id(p[i])] == -1) {
            chars[id(p[i])] = letters.size();
            letters.push_back({0, 0, p[i], i});
        }
        if (i == 0) {
            letters[0] = {1, 1, p[i], i};
        } else {
            letters[chars[id(p[i])]].h1 = (letters[chars[id(p[i])]].h1 + d.first * x.first) % mod.first;
            letters[chars[id(p[i])]].h2 = (letters[chars[id(p[i])]].h2 + d.second * x.second) % mod.second;
            d.first = (d.first * x.first) % mod.first;
            d.second = (d.second * x.second) % mod.second;
        }
    }
    vector<pair<int64_t, int64_t>> deg;
    deg.reserve(s.size());
    deg.emplace_back(1, 1);
    for (int i = 1; i < s.size(); ++i) {
        deg.emplace_back((deg[i - 1].first * x.first) % mod.first,
                         (deg[i - 1].second * x.second) % mod.second);
    }
    h.back() = {s.back(), s.back(), s.back()};
    {
//        LOG_DURATION("hash");
        for (int i = static_cast<int>(s.size()) - 2; i >= 0; --i) {
            h[i].c = s[i];
            h[i].h1 = (h[i + 1].h1 * x.first + s[i]) % mod.first;
            h[i].h2 = (h[i + 1].h2 * x.second + s[i]) % mod.second;
        }
    }
    bool answ;
    for (int i = p.size() - 1; i < s.size(); ++i) {
        st sm = {0, 0};
        st sum = {0, 0};
        answ = true;
        vector<char> dict(52, ' ');
        for (auto &letter: letters) {
            int index = letter.i + i + 1 - p.size();
            if (is_uppercase(s[index])
                and is_uppercase(letter.c)
                and s[index] != letter.c
                    or (is_uppercase(s[index]) ^ is_uppercase(letter.c))) {
                answ = false;
                break;
            }
            if (dict[id(s[index])] != ' ' and dict[id(s[index])] != letter.c) {
                answ = false;
                break;
            } else if (dict[id(s[index])] == ' ') {
                dict[id(s[index])] = letter.c;
            }
            sm.h1 = (sm.h1 + letter.h1 * s[index]) % mod.first;
            sm.h2 = (sm.h2 + letter.h2 * s[index]) % mod.second;
        }
        if (answ) {
            int index = i + 1 - p.size();
            if (index + p.size() < s.size()) {
                sum.h1 = (h[index].h1 - h[index + p.size()].h1 * deg[p.size()].first) % mod.first;
                sum.h2 = (h[index].h2 - h[index + p.size()].h2 * deg[p.size()].second) % mod.second;
            } else {
                sum.h1 = h[index].h1;
                sum.h2 = h[index].h2;
            }
            if (sum.h1 < 0) {
                sum.h1 += mod.first;
            }
            if (sum.h2 < 0) {
                sum.h2 += mod.second;
            }
            if (sum.h1 == sm.h1
                and sum.h2 == sm.h2
                and check(s, p, i + 1 - p.size())
                ) {
                cout << "Plagiarism was detected at position " << i + 2 - p.size() << '.' << '\n';
                return 0;
            }
        }
    }
    cout << "Looks like something new." << '\n';
    return 0;
}

