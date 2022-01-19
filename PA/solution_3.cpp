#include <iostream>
#include <algorithm>
#include <random>

using ll = long long;
using namespace std;

//ll mulmod(ll a, ll b, ll m){
//    ll res = 0;
//    a %= m;
//    b %= m;
//    while (a > 0){
//        if (a & 1){
//            res = (res + b) % m;
//        }
//        a >>= 1;
//        if (b < m - b){
//            b <<= 1;
//        } else {
//            b -= (m - b);
//        }
//    }
//    return res;
//}


ll pow(ll a, ll u, ll n){
    ll x = 1;
    while (u > 0){
        if (u & 1u){
//            x = mulmod(x, a, n);
            x = (x * a) % n;
        }
//        a = mulmod(a, a, n);
        a = (a * a) % n;
        u >>= 1u;
    }
    return x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("primes.in", "r", stdin);
    freopen("roots.out", "w", stdout);
    int q;
    cin >> q;
    ll n;
    ll p;
    ll d = 2;
    bool f = false;
    ll answ = -1;
    ll a;
    for (int i = 0; i < q; ++i){
        cin >> n;
        if (n == 2){
            cout << 1 << '\n';
        } else {
            vector<ll> v;
            p = n - 1;
            d = 2;
            if (p % d == 0){
                v.push_back(d);
            }
            while (p % d == 0 and p > 0){
                p /= d;
            }
            ++d;
            while (p >= d * d){
                if (p % d == 0){
                    v.push_back(d);
                }
                while (p % d == 0 and p > 0){
                    p /= d;
                }
                d += 2;
            }
            if (p > 1){
                v.push_back(p);
            }
            answ = -1;
            random_device rd;
            uniform_int_distribution<ll> g(2, n - 1);
            mt19937_64 gen(rd());
            for (ll j = 2; j < n; ++j){
                a = g(gen);
                f = (pow(a, n - 1, n) == 1);
                if (f){
                    for (auto item : v){
                        if (pow(a, (n - 1) / item, n) == 1){
                            f = false;
                            break;
                        }
                    }
                }
                if (f){
                    answ = a;
                    break;
                }
            }
            cout << answ << '\n';
        }
    }
    return 0;
}