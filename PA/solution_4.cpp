#include <iostream>
#include <algorithm>
#include <random>

using ll = long long;
using namespace std;

ll mulmod(ll a, ll b, ll m){
    ll res = 0;
    a %= m;
    b %= m;
    while (a > 0){
        if (a & 1){
            res = (res + b) % m;
        }
        a >>= 1;
        if (b < m - b){
            b <<= 1;
        } else {
            b -= (m - b);
        }
    }
    return res;
}


ll pow(ll a, ll u, ll n){
    ll x = 1;
    while (u > 0){
        if (u & 1u){
            x = mulmod(x, a, n);
        }
        a = mulmod(a, a, n);
        u >>= 1u;
    }
    return x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("primality.in", "r", stdin);
    freopen("primality.out", "w", stdout);
    int q;
    cin >> q;
    ll n;
    ll a, u, t;
    ll x;
    bool f = true;
    for (int i = 0; i < q; ++i){
        cin >> n;
        if (n == 2){
            cout << "Yes" << '\n';
        } else if (n < 2 or n % 2 == 0){
            cout << "No" << '\n';
        } else {
            u = n - 1;
            t = 0;
            while (u % 2 == 0 and u > 0){
                ++t;
                u >>= 1u;
            }
            random_device rd;
            uniform_int_distribution<ll> g(2, n - 1);
            mt19937_64 gen(rd());
            for (int k = 0; k < 10; ++k){
                a = g(gen);
                if (gcd(a, n) != 1){
                    f = false;
                    cout << "No" << '\n';
                    break;
                }
                x = pow(a, u, n);
                if (x == 1 or x == n - 1){
                    f = true;
                } else {
                    f = false;
                }
                for (int j = 0; j < t and !f; ++j){
                    x = mulmod(x, x, n);
                    if (x == n - 1){
                        f = true;
                    }
                }
                if (!f){
                    cout << "No" << '\n';
                    break;
                }
            }
            if (f){
                cout << "Yes" << '\n';
            }
        }
    }
//    cout << mulmod(584613200290683, 584613200290683, 1000000000000007);
    return 0;
}

/*
2443941478529
7072239466207
584613200290683
 */