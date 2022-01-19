#include <iostream>
#include <algorithm>
#include <random>
#include <bitset>

using ll = long long;
using namespace std;

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
    int len = n / 4 + (n % 4 != 0);
    for (int i = 0; i < n; ++i){
        cin >> str;
        answ = "";
        HexToBin(str, answ);
        A[i] = bitset<N>(answ);
        for (int j = 0; j < 4 * len - n; ++j){
            A[i].reset(j);
        }
    }
}
template <int N>
bitset<N> mul(vector<bitset<N>> &A, bitset<N> &v, int n){
    int len = n / 4 + (n % 4 != 0);
    bitset<N> answ;
    for (int i = 0; i < A.size(); ++i){
        answ[4 * len - i - 1] = (A[i] & v).count() & 1u;
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
    int len = n / 4 + (n % 4 != 0);
    for (int i = 0; i < 14; ++i){
        for (int j = 4 * len; j >= 0; --j){
            v[j] = g(gen);
        }
        left_res = mul<N>(B, v, n);
        left_res = mul<N>(A, left_res, n);
        right_res = mul<N>(C, v, n);
        res = left_res ^ right_res;
        index = res._Find_first();
        if (index != N){
            row = 4 * len - index;
            first = A[row - 1]._Find_first();
            if (first != N){
                bitset<N> tmp = B[4 * len - first - 1];
                for (int j = A[row - 1]._Find_next(first); j < N; j = A[row - 1]._Find_next(j)){
                    tmp ^= B[4 * len - j - 1];
                }
                tmp ^= C[row - 1];
                first = tmp._Find_first();
                column = 4 * len - first;
            } else {
                column = 4 * len - C[row - 1]._Find_first();
            }
            cout << "Yes" << '\n';
            cout << row << ' ' << column << '\n';
            return;
        }
    }
    cout << "No" << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("element.in", "r", stdin);
    freopen("element.out", "w", stdout);
    int n = 1;
    while (true){
        cin >> n;
        if (!n){
            break;
        }
        if (n <= 32){
            solution<32>(n);
        } else if (n <= 64){
            solution<64>(n);
        } else if (n <= 128){
            solution<128>(n);
        } else if (n <= 256){
            solution<256>(n);
        } else if (n <= 512){
            solution<512>(n);
        } else if (n <= 768){
            solution<768>(n);
        } else if (n <= 1024){
            solution<1024>(n);
        } else if (n <= 1280){
            solution<1280>(n);
        }  else if (n <= 1536){
            solution<1536>(n);
        } else if (n <= 1792){
            solution<1792>(n);
        }  else if (n <= 2048){
            solution<2048>(n);
        } else if (n <= 2304){
            solution<2304>(n);
        }  else if (n <= 2560){
            solution<2560>(n);
        } else if (n <= 2816){
            solution<2816>(n);
        }  else if (n <= 3072){
            solution<3072>(n);
        } else if (n <= 3328){
            solution<3328>(n);
        }  else if (n <= 3584){
            solution<3584>(n);
        } else if (n <= 4000){
            solution<4000>(n);
        }

    }
    return 0;
}