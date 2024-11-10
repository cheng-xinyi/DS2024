// Code to read "I-have-a-dream.txt" and calculate character frequencies
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