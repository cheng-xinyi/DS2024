#include <iostream>
#include <fstream>
#include <unordered_map>
#include "HuffmanCode.h"
#include "HuffTree.h"

int main() {
    // Read "I-have-a-dream.txt" and calculate character frequencies
    std::ifstream file("I-have-a-dream.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::unordered_map<char, int> frequencies;
    char ch;
    while (file.get(ch)) {
        if (std::isalpha(ch)) {
            ch = std::tolower(ch);
            frequencies[ch]++;
        }
    }

    file.close();

    // Build Huffman Tree
    HuffTree<std::pair<char, int>> huffTree(frequencies);

    // Generate Huffman Codes
    HuffmanCode huffCode(huffTree);

    // Output Huffman code for each character
    std::cout << "Character Codes:" << std::endl;
    for (const auto &pair : frequencies) {
        char character = pair.first;
        std::string code = huffCode.encode(std::string(1, character));
        std::cout << character << ": " << code << std::endl;
    }

    // Encode the word "dream"
    std::string word = "dream";
    std::string encodedWord = huffCode.encode(word);
    std::cout << "Encoded 'dream': " << encodedWord << std::endl;

    // Encode another word "have"
    word = "have";
    encodedWord = huffCode.encode(word);
    std::cout << "Encoded 'have': " << encodedWord << std::endl;

    return 0;
}
