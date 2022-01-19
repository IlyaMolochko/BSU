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
    freopen("triples.in", "r", stdin);
    freopen("triples.out", "w", stdout);
    int t;
    cin >> t;
    int n, m;
    int p = 1000000007;
    int rank;
    random_device rd;
    uniform_int_distribution<ll> g(1, p - 1);
    mt19937_64 gen(rd());
    string s;
    while (t--){
        cin >> n >> m;
        vector<pair<int, int>> v;
        vector<vector<ll>> A(2 * n + m, vector<ll>(2 * n + m));
        for (int i = 0; i < n; ++i){
            A[2 * i][2 * i + 1] = g(gen);
            A[2 * i + 1][2 * i] = -A[2 * i][2 * i + 1];
            cin >> s;
            for (int j = 0; j < m; ++j){
                if (s[j] == '1'){
                    A[2 * i][2 * n + j] = g(gen);
                    A[2 * n + j][2 * i] = -A[2 * i][2 * n + j];
                    A[2 * i + 1][2 * n + j] = g(gen);
                    A[2 * n + j][2 * i + 1] = -A[2 * i + 1][2 * n + j];
                }
            }
        }
        rank = 0;
        ll index, inverse;
        for (int k = 0; k < 2 * n + m; ++k){
            index = -1;
            for (int i = k; i < 2 * n + m; ++i){
                if (A[i][k]){
                    index = i;
                    break;
                }
            }
            if (index == -1){
                continue;
            }
            ++rank;
            if (index != k){
                swap(A[k], A[index]);
            }
            for (int i = 0; i < 2 * n + m; ++i){
                if (i != k){
                    inverse = (A[i][k] * pow(A[k][k], p - 2, p)) % p;
                    for (int j = k; j < 2 * n + m; ++j){
                        A[i][j] = (A[i][j] - (A[k][j] * inverse) % p) % p;
                    }
                }
            }
        }
        cout << rank / 2 - n << '\n';
    }
    return 0;
}