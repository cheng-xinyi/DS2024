#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include "HuffTree.h"
#include "Bitmap.h"
#include <unordered_map>
#include <string>

class HuffmanCode {
public:
    std::unordered_map<char, std::string> codes;

    HuffmanCode(HuffTree<std::pair<char, int>>& tree) {
        generateCodes(tree.root, "");
    }

    void generateCodes(BinTree<std::pair<char, int>>* node, const std::string& prefix) {
        if (node->isLeaf()) {
            codes[node->data.first] = prefix;
            return;
        }
        if (node->left) generateCodes(node->left, prefix + "0");
        if (node->right) generateCodes(node->right, prefix + "1");
    }

    std::string encode(const std::string& text) {
        std::string encoded = "";
        for (char ch : text) {
            encoded += codes[ch];
        }
        return encoded;
    }
};

#endif
