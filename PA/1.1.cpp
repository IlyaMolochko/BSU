#include <algorithm>
#include <bitset>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

void hexToBin(string &hex, string &bin) {
    for (auto c : hex) {
        if (c == '0') {
            bin += "0000";
        } else if (c == '1') {
            bin += "0001";
        } else if (c == '2') {
            bin += "0010";
        } else if (c == '3') {
            bin += "0011";
        } else if (c == '4') {
            bin += "0100";
        } else if (c == '5') {
            bin += "0101";
        } else if (c == '6') {
            bin += "0110";
        } else if (c == '7') {
            bin += "0111";
        } else if (c == '8') {
            bin += "1000";
        } else if (c == '9') {
            bin += "1001";
        } else if (c == 'A' or c == 'a') {
            bin += "1010";
        } else if (c == 'B' or c == 'b') {
            bin += "1011";
        } else if (c == 'C' or c == 'c') {
            bin += "1100";
        } else if (c == 'D' or c == 'd') {
            bin += "1101";
        } else if (c == 'E' or c == 'e') {
            bin += "1110";
        } else {
            bin += "1111";
        }
    }
}

template<auto N>
void matrix(vector<bitset<N>> &A, int n) {
    string hex;
    string bin;
    auto len = n / 4 + (n % 4 != 0);
    for (auto i = 0; i < n; ++i) {
        cin >> hex;
        bin.clear();
        hexToBin(hex, bin);
        A.emplace_back(bin);
        for (auto j = 0; j < 4 * len - n; ++j) {
            A.back().reset(j);
        }
    }
}

template<size_t N>
bitset<N> operator*(vector<bitset<N>> &A, bitset<N> &v) {
    bitset<N> res;
    auto n = A.size();
    auto len = n / 4 + (n % 4 != 0);
    for (auto i = 0; i < n; ++i) {
        res[4 * len - i - 1] = (A[i] & v).count() % 2;
    }
    return res;
}

template<auto N>
void solution(int n){
    default_random_engine gen;
    gen.seed(time(nullptr));
    uniform_int_distribution<int> x(0, 1);
    vector<bitset<N>> A, B, C;
    matrix(A, n);
    matrix(B, n);
    matrix(C, n);
    bitset<N> v, res;
    auto len = n / 4 + (n % 4 != 0);
    for (auto i = 0; i < 20; ++i){
        for (auto j = 0; j < 4 * len; ++j){
            v[j] = x(gen);
        }
        res = B * v;
        res = A * res;
        if (res != C * v){
            cout << "NO" << '\n';
            return;
        }
    }
    cout << "YES" << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);
    int n;
    cin >> n;
    if (n <= 64){
        solution<64>(n);
    } else if (n <= 512){
        solution<512>(n);
    } else if (n <= 1024){
        solution<1024>(n);
    } else if (n <= 2048){
        solution<2048>(n);
    } else {
        solution<4000>(n);
    }
    return 0;
}