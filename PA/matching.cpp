#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>

using ll = long long;
using namespace std;

ll pow(ll a, ll u, ll n){
    ll x = 1;
    while (u > 0){
        if (u & 1u){
            x = (x * a) % n;
        }
        a = (a * a) % n;
        u >>= 1u;
    }
    return x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("matching.in", "r", stdin);
    freopen("matching.out", "w", stdout);
    int n, m;
    int p = 1000000007;
    random_device rd;
    uniform_int_distribution<ll> g(1, p - 1);
    mt19937_64 gen(rd());
    cin >> n >> m;
    if (n % 2 == 1 or m < n){
        for (int i = 0; i < m; ++i){
            cout << "NO" << '\n';
        }
        return 0;
    }
    vector<pair<int, int>> v(m);
    vector<vector<ll>> A(n, vector<ll>(n));
    vector<vector<ll>> B(n, vector<ll>(n));

    for (int i = 0; i < n; ++i){
        B[i][i] = 1;
    }
    for (auto &i :v){
        cin >> i.first >> i.second;
        --i.first;
        --i.second;
        A[i.first][i.second] = g(gen);
        A[i.second][i.first] = -A[i.first][i.second];
    }
    ll index = -1, inverse;
    for (int k = 0; k < n; ++k){
        index = -1;
        for (int i = k; i < n; ++i){
            if (A[i][k]){
                index = i;
                break;
            }
        }
        if (index == -1){
            for (int i = 0; i < m; ++i){
                cout << "NO" << '\n';
            }
            return 0;
        }
        if (index != k){
            swap(A[k], A[index]);
            swap(B[k], B[index]);
        }
        for (int i = 0; i < n; ++i){
            if (i != k){
                inverse = (A[i][k] * pow(A[k][k], p - 2, p)) % p;
                for (int j = k; j < n; ++j){
                    A[i][j] = (A[i][j] - (A[k][j] * inverse) % p) % p;
                }
                for (int j = 0; j < n; ++j){
                    B[i][j] = (B[i][j] - (B[k][j] * inverse) % p) % p;
                }
            }
        }
    }
    if (index == -1){
        for (int i = 0; i < m; ++i){
            cout << "NO" << '\n';
        }
        return 0;
    }
    for (auto &i : v){
        if (B[i.first][i.second]){
            cout << "YES" << '\n';
        } else {
            cout << "NO" << '\n';
        }
    }
    return 0;
}