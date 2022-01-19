#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>

using ll = long long;
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    hash<string> hash_f;
    string s;
    unsigned int m = (1u << 10);
    vector<int> v(m);
    ll hash_v;
    unsigned int id;
    unsigned int item;
    ll mx = 4294967296;
    cout << mx << '\n';
    unsigned int cnt;
    double e;

    while (getline(cin, s)){
        if (s.empty()){
            break;
        }
        hash_v = hash_f(s);
        id = hash_v >> 20u;
        item = hash_v & ((1u << 20u) - 1);
        v[id] = max(v[id], 20 - int(log2(item)));

        e = 0;
        cnt = 0;

        for (int i = 0; i < m; ++i){
            e += 1.0 / (1u << v[i]);
            if (v[i] == 0){
                ++cnt;
            }
        }
        e = 1.0 / e;
        e = e * 0.673 * m * m;

        if (e <= 2.5 * m){
            if (cnt != 0){
                e = m * log(double (m) / cnt);
            }
        } else if (e > (1.0 / 30) * mx){
            e = -(mx * log(1.0 - e / mx));
        }

    }
    cout << int(e) << '\n';
    return 0;
}