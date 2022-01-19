#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>

using ll = long long;
using namespace std;

int len;

void HexToBin(const string& hexdec, string &answ) {
    for (auto i : hexdec){
        if (i == '0'){
            answ += "0000";
        } else if (i == '1'){
            answ += "0001";
        } else if (i == '2'){
            answ += "0010";
        } else if (i == '3'){
            answ += "0011";
        } else if (i == '4'){
            answ += "0100";
        } else if (i == '5'){
            answ += "0101";
        } else if (i == '6'){
            answ += "0110";
        } else if (i == '7'){
            answ += "0111";
        } else if (i == '8'){
            answ += "1000";
        } else if (i == '9'){
            answ += "1001";
        } else if (i == 'A' or i == 'a'){
            answ += "1010";
        } else if (i == 'B' or i == 'b'){
            answ += "1011";
        } else if (i == 'C' or i == 'c'){
            answ += "1100";
        } else if (i == 'D' or i == 'd'){
            answ += "1101";
        } else if (i == 'E' or i == 'e'){
            answ += "1110";
        } else if (i == 'F' or i == 'f'){
            answ += "1111";
        }
    }
}

template <int N>
void get_matrix(vector<bitset<N>> &A, int n){
    string str, answ;
    for (int i = 0; i < n; ++i){
        cin >> str;
        answ = "";
        HexToBin(str, answ);
        A[i] = bitset<N>(answ);
        for (int j = 0; j < len - n; ++j){
            A[i].reset(j);
        }
    }
}
template <int N>
bitset<N> mul(vector<bitset<N>> &A, bitset<N> &v){
    bitset<N> answ;
    for (int i = 0; i < A.size(); ++i){
        answ[len - i - 1] = (A[i] & v).count() & 1u;
    }
    return answ;
}

template <int N>
void solution(int n){
    string str;
    random_device rd;
    uniform_int_distribution<int> g(0, 1);
    mt19937_64 gen(rd());

    int index, row = 0, column = 0, first;
    vector<bitset<N>> A(n), B(n), C(n);
    bitset<N> v;
    bitset<N> left_res, right_res, res;
    get_matrix<N>(A, n);
    get_matrix<N>(B, n);
    get_matrix<N>(C, n);
    for (int j = len - 1; j > 1; --j){
        v.set(j);
    }
    for (int i = 0; i < 30; ++i){
        left_res = mul<N>(B, v);
        left_res = mul<N>(A, left_res);
        right_res = mul<N>(C, v);
        res = left_res ^ right_res;
        index = res._Find_first();
        if (index < N){
            cout << "NO" << '\n';
            return;
            row = len - index;
            first = A[row - 1]._Find_first();
            if (first < N){
                bitset<N> tmp = B[len - first - 1];
                for (int j = A[row - 1]._Find_next(first); j < N; j = A[row - 1]._Find_next(j)){
                    tmp ^= B[len - j - 1];
                }
                column = len - (tmp ^ C[row - 1])._Find_first();
            } else {
                column = len - C[row - 1]._Find_first();
            }
            cout << "Yes" << '\n';
            cout << row << ' ' << column << '\n';
            return;
        }
        for (int j = len; j > 1; --j){
            v[j] = g(gen);
        }
    }
    cout << "YES" << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);
    int n = 1;

    while (true){
        cin >> n;
        if (!n){
            break;
        }
        len = (n / 4 + (n % 4 != 0)) * 4;
        if (n <= 128){
            solution<128>(n);
        } else {
            solution<4000>(n);
        }
        break;
    }
    return 0;
}