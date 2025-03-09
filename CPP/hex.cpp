#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

// Function to set up hex table for ASCII codes
unordered_map<char, string> setupHexTable() {
    unordered_map<char, string> hexTable;
    for (int i = 0; i < 128; ++i) {
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << i;
        hexTable[static_cast<char>(i)] = ss.str();
    }
    return hexTable;
}

// Function to read hex data and convert it to ASCII string
string readHexData(const string& hexData) {
    stringstream ss(hexData);
    string output;
    string hexByte;
    while (ss >> hexByte) {
        char ch = static_cast<char>(stoi(hexByte, nullptr, 16));
        output += ch;
    }
    return output;
}

int main() {
    // Set up hex table
    auto hexTable = setupHexTable();

    // Display hex table
    cout << "Hex table for ASCII codes:" << endl;
    for (const auto& [ch, hexCode] : hexTable) {
        cout << ch << ": " << hexCode << endl;
    }

    // Read hex data
    string hexData;
    cout << "Enter hex data (space-separated): ";
    getline(cin, hexData);

    // Convert hex data to ASCII string
    string asciiString = readHexData(hexData);
    cout << "Converted ASCII string: " << asciiString << endl;

    return 0;
}
