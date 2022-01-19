#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void get_order(vector<int> &s, vector<int> &order) {
    vector<int> cnt(102);
    for (int number : s) {
        ++cnt[number + 1];
    }
    for (int j = 1; j < 102; ++j) {
        cnt[j] += cnt[j - 1];
    }
    for (int i = s.size() - 1; i >= 0; --i) {
        order[--cnt[s[i] + 1]] = i;
    }
}

void get_classes(vector<int> &s, vector<int> &order, vector<int> &classes) {
    classes[order[0]] = 0;
    for (int i = 1; i < s.size(); ++i) {
        if (s[order[i]] == s[order[i - 1]]) {
            classes[order[i]] = classes[order[i - 1]];
        } else {
            classes[order[i]] = classes[order[i - 1]] + 1;
        }
    }
}

void update_order(vector<int> &order, vector<int> &classes, int k) {
    vector<int> cnt(order.size());
    vector<int> new_order(order.size());
    for (int i = 0; i < order.size(); ++i) {
        ++cnt[classes[i]];
    }
    for (int i = 1; i < order.size(); ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = order.size() - 1; i >= 0; --i) {
        int start = (order.size() + order[i] - k) % order.size();
        int cl = classes[start];
        new_order[--cnt[cl]] = start;
    }
    order = move(new_order);
}

void update_classes(vector<int> &order, vector<int> &classes, int k) {
    vector<int> new_classes(order.size());
    for (int i = 1; i < order.size(); ++i) {
        int cur = order[i];
        int prev = order[i - 1];
        int mid = (cur + k) % order.size();
        int mid_prev = (prev + k) % order.size();
        if (classes[cur] != classes[prev] or classes[mid] != classes[mid_prev]) {
            new_classes[cur] = new_classes[prev] + 1;
        } else {
            new_classes[cur] = new_classes[prev];
        }
    }
    classes = move(new_classes);
}

int solution(vector<int> &s, vector<int> &order) {
    get_order(s, order);
    vector<int> classes(s.size());
    get_classes(s, order, classes);
    int k = 1;
    while (k < s.size()) {
        update_order(order, classes, k);
        update_classes(order, classes, k);
        k <<= 1;
    }
    k = 0;
    int answ = 0;
    for (int i = 0; i < s.size() - 1; ++i) {
        int pos_i = classes[i];
        int pos_j = order[pos_i - 1];
        while (s[i + k] == s[pos_j + k]) {
            ++k;
        }
        answ += k;
        k = max(k - 1, 0);
    }
    return answ;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> s(n + 1);
        for (int i = 0; i < n; ++i) {
            cin >> s[i];
        }
        s[n] = -1;
        vector<int> order(s.size());
        cout << solution(s, order) << '\n';
    }
    return 0;
}