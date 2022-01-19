#include <algorithm>
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;
int N = 1 << 18;
double PI = acos(-1);
vector<int> rev(N);

int bitrev(int x) {
    int y = 0;
    for (int i = 0; i < 18; ++i) {
        y = 2 * y + ((x & (1 << i)) > 0);
    }
    return y;
}

vector<complex<double>> fft(vector<complex<double>> &a, bool inv) {
    vector<complex<double>> b(N);
    for (int i = 0; i < N; ++i) {
        b[i] = a[rev[i]];
    }
    for (int len = 2; len <= N; len <<= 1) {
        complex<double> w(cos(2 * PI / len), sin(2 * PI / len * (inv ? -1 : 1)));
        for (int i = 0; i < N; i += len) {
            complex<double> wi(1, 0);
            for (int j = 0; j < len / 2; ++j) {
                complex<double> tmp = b[i + j + len / 2] * wi;
                b[i + j + len / 2] = b[i + j] - tmp;
                b[i + j] += tmp;
                wi *= w;
            }
        }
    }
    return b;
}

vector<int> multiply(vector<int> &a, vector<int> &b) {
    vector<complex<double>> fa(N), fb(N);
    for (int i = 0; i < a.size(); ++i) {
        fa[i].real(a[i]);
    }
    for (int i = 0; i < b.size(); ++i) {
        fb[i].real(b[i]);
    }
    fa = fft(fa, false);
    fb = fft(fb, false);
    for (int i = 0; i < N; ++i) {
        fa[i] *= fb[i];
    }
    fa = fft(fa, true);
    vector<int> coeffs(N);
    for (int i = 0; i < N; ++i) {
        coeffs[i] = round(fa[i].real() / N);
    }
    int n = coeffs.size() - 1;
    while (n > 0 and coeffs[n] == 0) {
        --n;
        coeffs.pop_back();
    }
//    cout << coeffs.size() << '\n';
//    for (long coeff : coeffs) {
//        cout << coeff << ' ';
//    }
    return coeffs;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    for (int i = 0; i < N; ++i) {
        rev[i] = bitrev(i);
    }
    string s, p;
    cin >> s >> p;
    vector<int> dist(s.size() + p.size(), p.size());
    for (char c = 'a'; c <= 'z'; ++c) {
        vector<int> a(s.size());
        vector<int> b(p.size());
        bool f1 = false;
        bool f2 = false;
        for (int i = 0; i < a.size(); ++i) {
            if (s[i] == c) {
                a[i] = 1;
                f1 = true;
            }
            if (i < p.size() and p[i] == c) {
                b[p.size() - 1 - i] = 1;
                f2 = true;
            }
        }
        if (f1 and f2) {
            auto res = multiply(a, b);
            for (int i = 0; i < min(res.size(), dist.size()); ++i) {
                dist[i] -= res[i];
            }
        }
    }
    for (int i = p.size() - 1; i < dist.size() - p.size(); ++i) {
        cout << dist[i] << ' ';
    }
    return 0;
}