#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

using ll = long long;
using namespace std;


void dfs(vector<vector<int>> &g, int i, vector<bool> &visited, vector<int> &order, vector<int> &v) {
    visited[i] = true;
    vector<int> st;
    st.push_back(i);
    int top;
    while (!st.empty()) {
        top = st.back();
        if (g[top].size() > 0 and g[top].size() - 1 >= v[top] and !visited[g[top][v[top]]]) {
            visited[g[top][v[top]]] = true;
            st.push_back(g[top][v[top]]);
            ++v[top];
        } else if (g[top].size() > 0 and g[top].size() - 1 >= v[top]) {
            ++v[top];
        } else {
            order.push_back(top);
            st.pop_back();
        }
    }
}

void get_components(vector<vector<int>> &r, int i, vector<bool> &visited, vector<int> &c, int num, vector<int> &cnt, vector<int> &v) {
    visited[i] = true;
    c[i] = num;
    cnt.push_back(1);
    vector<int> st;
    st.push_back(i);
    int top;
    while (!st.empty()) {
        top = st.back();
        if (!r[top].empty() and r[top].size() - 1 >= v[top] and !visited[r[top][v[top]]]) {
            visited[r[top][v[top]]] = true;
            c[r[top][v[top]]] = num;
            ++cnt.back();
            st.push_back(r[top][v[top]]);
            ++v[top];
        } else if (!r[top].empty() and r[top].size() - 1 >= v[top]) {
            ++v[top];
        } else {
            st.pop_back();
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("reachability.in", "r", stdin);
    freopen("reachability.out", "w", stdout);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n, vector<int>());
    vector<vector<int>> r(n, vector<int>());
    vector<bool> visited(n, false);
    vector<int> order;

    int u, v;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v;
        g[--u].push_back(--v);
        r[v].push_back(u);
    }

    vector<int> pos(g.size());
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(g, i, visited, order, pos);
        }
    }

    vector<int> c(n);
    vector<int> cnt;
    visited.assign(n, false);
    pos.assign(n, 0);
    int num = 0, index;
    for (int i = 0; i < n; ++i) {
        index = order[n - i - 1];
        if (!visited[index]) {
            get_components(r, index, visited, c, num, cnt, pos);
            ++num;
        }
    }


    vector<vector<int>> g_cond = vector<vector<int>>(num, vector<int>());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            if (c[i] != c[g[i][j]]) {
                g_cond[c[i]].push_back(c[g[i][j]]);
            }
        }
    }

    vector<double> w(num);
    vector<double> ws(num);
    random_device rd;
    uniform_real_distribution<double> d(0, 1);
    mt19937_64 gen(rd());

    for (int it = 0; it < 100; ++it){
        for (int i = 0; i < num; ++i){
            w[i] = d(gen);
            for (int j = 1; j < cnt[i]; ++j){
                w[i] = min(w[i], d(gen));
            }
        }
        for (int i = g_cond.size() - 1; i >= 0; --i){
            for (int j = 0; j < g_cond[i].size(); ++j){
                w[i] = min(w[i], w[g_cond[i][j]]);
            }
        }
        for (int i = 0; i < num; ++i){
            ws[i] += log(1.0 - w[i]);
        }
    }

    for (int i = 0; i < num; ++i){
        cnt[i] = round(-100.0 / ws[i]);
    }

    for (int i = 0; i < n; ++i){
        cout << cnt[c[i]] << '\n';
    }
    return 0;
}