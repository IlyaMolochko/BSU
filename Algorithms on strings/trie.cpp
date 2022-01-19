#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct TrieNode {
    TrieNode *parent;
    vector<TrieNode *> children;
    TrieNode *suf_link;
    TrieNode *output_link;
    char x;
    bool terminal;

    TrieNode() {
        parent = nullptr;
        children.assign(26, nullptr);
        suf_link = nullptr;
        output_link = nullptr;
        terminal = false;
    }
};


struct Trie {
    TrieNode *root;

    Trie() {
        root = new TrieNode();
        root->parent = root;
        root->suf_link = root;
    }

    void build(vector<string> &words) {
        for (auto &word : words) {
            TrieNode *node = root;
            for (auto c : word) {
                if (node->children[c - 'a'] == nullptr) {
                    node->children[c - 'a'] = new TrieNode();
                    node->children[c - 'a']->parent = node;
                    node->children[c - 'a']->x = c;
                }
                node = node->children[c - 'a'];
            }
            node->terminal = true;
        }
    }

    void build_suf_link() {
        queue<TrieNode *> q;
        q.push(root);
        while (!q.empty()) {
            TrieNode *node = q.front();
            q.pop();
            for (int i = 0; i < 26; ++i) {
                if (node->children[i]) {
                    TrieNode *suf_link = node->suf_link;
                    while (suf_link != root and
                           suf_link->children[i] == nullptr) {
                        suf_link = suf_link->suf_link;
                    }
                    if (suf_link->children[i] and
                        suf_link != node) {
                        node->children[i]->suf_link = suf_link->children[i];
                    } else {
                        node->children[i]->suf_link = root;
                    }
                    q.push(node->children[i]);
                }
            }
        }
    }

    void build_output_link() {
        queue<TrieNode*> q;
        root->output_link = root;
        q.push(root);
        while (!q.empty()) {
            TrieNode *node = q.front();
            q.pop();
            TrieNode *suf = node->suf_link;
            while (suf != root and !suf->terminal) {
                suf = suf->suf_link;
            }
            if (suf->terminal and suf != node) {
                node->output_link = suf;
            }
            for (int i = 0; i < 26; ++i) {
                if (node->children[i]) {
                    q.push(node->children[i]);
                }
            }
        }
    }
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}