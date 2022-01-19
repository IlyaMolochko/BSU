#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>
#include <unordered_map>

using ll = long long;
using namespace std;

unsigned int h(string &s) {
    unsigned int hs = 0x811c9dc5;
    for (int i = 0; i < s.size(); ++i) {
        hs ^= s[i];
        hs *= 0x01000193;
    }
    return hs;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    hash<string> hash_f;
    string s;
    ll m = (1u << 18);
    vector<uint8_t> v(m);
    size_t hash_v;
    unsigned int id;
    unsigned int item;
    unsigned int cnt = 0;
    double e;
    uint8_t r;
    while (cin >> s) {
        hash_v = hash_f(s);
        id = hash_v >> 14u;
        item = hash_v & (~(hash_v - 1));
        r = log2(item) + 1;
        if (v[id] == 0) {
            ++cnt;
            v[id] = r;
            e += 1.0 / (1u << r);
        } else if (r > v[id]) {
            e += 1.0 / (1u << r);
            e -= 1.0 / (1u << v[id]);
            v[id] = r;
        }
    }
    if (cnt != 0){
        e = m * log(double (m) / double(m - cnt));
    }
    cout << int(e) << '\n';
    return 0;
}