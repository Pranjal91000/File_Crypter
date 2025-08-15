#include "huffman.hpp"
using namespace std;

// Huffman Tree Node
struct Node {
    char ch;
    int freq;
    shared_ptr<Node> left, right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue
struct Compare {
    bool operator()(shared_ptr<Node> left, shared_ptr<Node> right) {
        return left->freq > right->freq;
    }
};

// Simulate progress
void simulateProgress(const string& processName) {
    cout << processName << " in Progress: ";
    for (int i = 0; i <= 100; i += 10) {
        cout << "\r" << processName << " in Progress: ";
        cout << string(i / 2, '#') << string(50 - i / 2, ' ') << i << "%";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    cout << endl;
}

// Build Huffman tree and codes
void buildHuffmanTree(const string& input, unordered_map<char, string>& huffmanCode) {
    unordered_map<char, int> freqMap;
    for (char ch : input) {
        freqMap[ch]++;
    }

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> pq;
    for (auto& entry : freqMap) {
        pq.push(make_shared<Node>(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto sum = make_shared<Node>('\0', left->freq + right->freq);
        sum->left = left;
        sum->right = right;
        pq.push(sum);
    }

    auto root = pq.top();

    // Lambda to build the code table
    std::function<void(shared_ptr<Node>, string)> buildCodeTable = [&](shared_ptr<Node> node, string code) {
        if (!node->left && !node->right) {
            huffmanCode[node->ch] = code;
            return;
        }
        if (node->left) buildCodeTable(node->left, code + "0");
        if (node->right) buildCodeTable(node->right, code + "1");
    };

    buildCodeTable(root, "");
}

void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::in);
    if (!inFile) {
        cerr << "Error: Cannot open input file." << endl;
        return;
    }

    string input((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    unordered_map<char, string> huffmanCode;
    buildHuffmanTree(input, huffmanCode);

    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "Error: Cannot create output file." << endl;
        return;
    }

    string encodedData;
    for (char ch : input) {
        encodedData += huffmanCode[ch];
    }

    outFile << huffmanCode.size() << '\n';
    for (const auto& pair : huffmanCode) {
        outFile << pair.first << ' ' << pair.second << '\n';
    }

    outFile << encodedData;
    outFile.close();

    simulateProgress("Compression");
    cout << "File compressed successfully to: " << outputFile << endl;
}

void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Error: Cannot open compressed file." << endl;
        return;
    }

    size_t mapSize;
    inFile >> mapSize;
    unordered_map<string, char> reverseCode;
    for (size_t i = 0; i < mapSize; ++i) {
        char ch;
        string code;
        inFile >> ch >> code;
        reverseCode[code] = ch;
    }

    inFile.ignore(); // Ignore newline character after the map

    string encodedData((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    string decodedData, temp;
    for (char bit : encodedData) {
        temp += bit;
        if (reverseCode.count(temp)) {
            decodedData += reverseCode[temp];
            temp.clear();
        }
    }

    ofstream outFile(outputFile, ios::out);
    if (!outFile) {
        cerr << "Error: Cannot create decompressed file." << endl;
        return;
    }

    outFile << decodedData;
    outFile.close();

    simulateProgress("Decompression");
    cout << "File decompressed successfully to: " << outputFile << endl;
}
