#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "BinTree.h"
#include <queue>
#include <unordered_map>
#include <utility>

template <typename T>
class HuffTree {
public:
    BinTree<T>* root;

    // 修改后的构造函数，接受 unordered_map<char, int> 作为参数
    HuffTree(const std::unordered_map<char, int>& frequencies) {
        auto cmp = [](BinTree<std::pair<char, int>>* a, BinTree<std::pair<char, int>>* b) {
            return a->data.second > b->data.second;
        };
        std::priority_queue<BinTree<std::pair<char, int>>*, std::vector<BinTree<std::pair<char, int>>*>, decltype(cmp)> pq(cmp);

        // 将频率表中的每个字符与频率创建为一个 BinTree 节点
        for (auto& pair : frequencies) {
            pq.push(new BinTree<std::pair<char, int>>(std::make_pair(pair.first, pair.second)));
        }

        // 构建 Huffman 树
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            int combinedFreq = left->data.second + right->data.second;
            BinTree<std::pair<char, int>>* newNode = new BinTree<std::pair<char, int>>(std::make_pair('\0', combinedFreq));
            newNode->left = left;
            newNode->right = right;
            pq.push(newNode);
        }
        root = pq.top();
    }

    ~HuffTree() {
        delete root;
    }
};

#endif
