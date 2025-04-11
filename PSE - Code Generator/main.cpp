#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#define OP_LOAD_CHAR 1
#define OP_DONE 255

std::string sanitizeName(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (std::isalnum(ch) || ch == '_') {
            result += ch;
        } else {
            result += '_';
        }
    }
    return result;
}

std::vector<unsigned char> encodeToBytecode(const std::string& input, unsigned char key = 0x55) {
    std::vector<unsigned char> bytecode;
    for (char ch : input) {
        bytecode.push_back(OP_LOAD_CHAR);
        bytecode.push_back(key);
        bytecode.push_back(ch ^ key);
    }
    bytecode.push_back(OP_DONE);
    return bytecode;
}

void generateCppSections(const std::map<std::string, std::vector<unsigned char>>& strings) {
    std::cout << "#include <map>\n#include <vector>\n#include <string>\n\n";

    // Declare sections
    for (const auto& [name, bytecode] : strings) {
        std::string varName = sanitizeName(name) + "_section";
        std::cout << "const unsigned char " << varName << "[] = { ";
        for (size_t i = 0; i < bytecode.size(); ++i) {
            std::cout << "0x" << std::hex << std::setw(2)
                      << std::setfill('0') << (int)bytecode[i];
            if (i != bytecode.size() - 1) std::cout << ", ";
        }
        std::cout << " };\n";
    }

    std::cout << "\nstd::map<std::string, const unsigned char*> stringSections = {\n";
    for (const auto& [name, _] : strings) {
        std::string varName = sanitizeName(name) + "_section";
        std::cout << "    { \"" << name << "\", " << varName << " },\n";
    }
    std::cout << "};\n";

    std::cout << "\nstd::map<std::string, size_t> stringSizes = {\n";
    for (const auto& [name, bytecode] : strings) {
        std::string varName = sanitizeName(name) + "_section";
        std::cout << "    { \"" << name << "\", sizeof(" << varName << ") },\n";
    }
    std::cout << "};\n";
}

int main() {
    std::map<std::string, std::vector<unsigned char>> stringMap;
    std::string inputName, inputString, keyStr;
    unsigned int key = 0x55;

    while (true) {
        std::cout << "Enter string name (or empty to finish): ";
        std::getline(std::cin, inputName);
        if (inputName.empty()) break;

        std::cout << "Enter string value: ";
        std::getline(std::cin, inputString);

        std::cout << "Enter XOR key (0-255, default=85): ";
        std::getline(std::cin, keyStr);
        key = keyStr.empty() ? 0x55 : std::stoi(keyStr) & 0xFF;

        stringMap[inputName] = encodeToBytecode(inputString, static_cast<unsigned char>(key));
    }

    std::cout << "\n// ===== GENERATED C++ CODE =====\n\n";
    generateCppSections(stringMap);
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
