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

void get_matrix(vector<bitset<4000>> &A, int n){
    string str, answ;
    for (int i = 0; i < n; ++i){
        cin >> str;
        answ = "";
        HexToBin(str, answ);
        A.emplace_back(answ);
        for (int j = 0; j < 4 * len - n; ++j){
            A.back().reset(j);
        }
    }
}

bitset<4000> operator*(vector<bitset<4000>> &A, bitset<4000> &v){
    bitset<4000> answ;
    for (int i = 0; i < A.size(); ++i){
        answ[4 * len - i - 1] = (A[i] & v).count() % 2;
    }
    return answ;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);
    int n;
    cin >> n;
    len = n / 4 + (n % 4 != 0);
    string str, answ;
    vector<bitset<4000>> A, B, C;
    get_matrix(A, n);
    get_matrix(B, n);
    get_matrix(C, n);
    bitset<4000> v;
    random_device rd;
    uniform_int_distribution<int> g(0, 1);
    mt19937_64 gen(rd());
    bitset<4000> res;
    for (int i = 0; i < 25; ++i){
        for (int j = 4 * len; j > 1; --j){
            v[j] = g(gen);
        }
        res = B * v;
        res = A * res;
        if (res != C * v){
            cout << "NO" << '\n';
            return 0;
        }
    }
    cout << "YES" << '\n';
    return 0;
}