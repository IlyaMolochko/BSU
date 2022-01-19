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

bool is_prime(ll n){
    ll a;
    ll x;
    ll u = n - 1;
    ll t = 0;
    bool f = true;
    while (!(u & 1u) and u > 0){
        ++t;
        u >>= 1u;
    }
    random_device rd;
    uniform_int_distribution<ll> g(2, n - 1);
    mt19937_64 gen(rd());
    for (int k = 0; k < 10; ++k){
        a = g(gen);
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
            return f;
        }
    }
    return f;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("carmichael.in", "r", stdin);
    freopen("carmichael.out", "w", stdout);
    int q;
    cin >> q;
    ll n;
    ll a;
    string answ;
    for (int i = 0; i < q; ++i){
        cin >> n;
        if (n < 561 or !(n & 1u) or is_prime(n)){
            cout << "No" << '\n';
        } else {
            answ = "Yes";
            random_device rd;
            uniform_int_distribution<ll> g(2, n - 1);
            mt19937_64 gen(rd());
            for (int k = 0; k < 10; ){
                a = g(gen);
                if (gcd(a, n) == 1){
                    ++k;
                    if (pow(a, n, n) != a){
                        answ = "No";
                        break;
                    }
                }
            }
            cout << answ << '\n';
        }
    }
    return 0;
}