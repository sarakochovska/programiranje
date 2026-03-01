#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct Node {
    unsigned char ch;
    int freq;
    Node* left;
    Node* right;

    Node(unsigned char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }

    Node(Node* l, Node* r) {
        ch = 0;
        freq = l->freq + r->freq;
        left = l;
        right = r;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root, string code, string codes[256]) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->ch] = code;
        return;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

int main() {

    ifstream infile("anna.txt", ios::binary);
    if (!infile.is_open()) {
        cout << "Failed to open anna.txt\n";
        return 1;
    }

    // =====================
    // 🔹 COUNT FREQUENCIES
    // =====================
    int freq[256] = {0};
    unsigned char c;

    while (infile.read((char*)&c, 1)) {
        freq[c]++;
    }

    infile.clear();
    infile.seekg(0);

    // =====================
    // 🔹 BUILD HUFFMAN TREE
    // =====================
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq.push(new Node((unsigned char)i, freq[i]));
        }
    }

    if (pq.empty()) {
        cout << "File is empty\n";
        return 1;
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }

    Node* root = pq.top();

    // =====================
    // 🔹 GENERATE CODES
    // =====================
    string codes[256];
    generateCodes(root, "", codes);

    // =====================
    // 🔹 ENCODE FILE
    // =====================
    ofstream outfile("anna_encoded.bin", ios::binary);
    ofstream bitfile("anna_bits.txt");

    // Save frequency table for decoding
    for (int i = 0; i < 256; i++) {
        outfile.write((char*)&freq[i], sizeof(int));
    }

    unsigned char buffer = 0;
    int bitCount = 0;

    while (infile.read((char*)&c, 1)) {
        string code = codes[c];

        for (char bit : code) {

            bitfile << bit; // readable bits

            buffer <<= 1;
            if (bit == '1')
                buffer |= 1;

            bitCount++;

            if (bitCount == 8) {
                outfile.write((char*)&buffer, 1);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        outfile.write((char*)&buffer, 1);
    }

    infile.close();
    outfile.close();
    bitfile.close();

    cout << "Compression complete.\n";

    // =====================
    // 🔹 DECODE FILE
    // =====================
    ifstream encoded("anna_encoded.bin", ios::binary);
    ofstream decoded("anna_decoded.txt", ios::binary);

    // Read frequency table
    int readFreq[256];
    for (int i = 0; i < 256; i++) {
        encoded.read((char*)&readFreq[i], sizeof(int));
    }

    // Rebuild Huffman tree
    priority_queue<Node*, vector<Node*>, Compare> pq2;

    for (int i = 0; i < 256; i++) {
        if (readFreq[i] > 0) {
            pq2.push(new Node((unsigned char)i, readFreq[i]));
        }
    }

    while (pq2.size() > 1) {
        Node* left = pq2.top(); pq2.pop();
        Node* right = pq2.top(); pq2.pop();
        pq2.push(new Node(left, right));
    }

    Node* decodeRoot = pq2.top();
    Node* current = decodeRoot;

    // Count total characters
    int totalChars = 0;
    for (int i = 0; i < 256; i++)
        totalChars += readFreq[i];

    unsigned char byte;
    int decodedCount = 0;

    while (encoded.read((char*)&byte, 1) && decodedCount < totalChars) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;

            if (bit == 0)
                current = current->left;
            else
                current = current->right;

            if (!current->left && !current->right) {
                decoded.write((char*)&current->ch, 1);
                decodedCount++;
                current = decodeRoot;
            }

            if (decodedCount >= totalChars)
                break;
        }
    }

    encoded.close();
    decoded.close();

    cout << "Decompression complete.\n";
    cout << "Created:\n";
    cout << " - anna_encoded.bin\n";
    cout << " - anna_bits.txt\n";
    cout << " - anna_decoded.txt\n";

    return 0;
}