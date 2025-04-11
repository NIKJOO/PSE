#include <iostream>
#include <string>
#include <map>
#define OP_LOAD_CHAR 1
#define OP_DONE 255
#include <map>
#include <vector>
#include <string>

#include <map>
#include <vector>
#include <string>

const unsigned char str1_section[] = { 0x01, 0x54, 0x00, 0x01, 0x54, 0x3c, 0x01, 0x54, 0x3d, 0x01, 0x54, 0x27, 0x01, 0x54, 0x74, 0x01, 0x54, 0x3d, 0x01, 0x54, 0x27, 0xff };
const unsigned char str2_section[] = { 0x01, 0x30, 0x7d, 0x01, 0x30, 0x49, 0x01, 0x30, 0x10, 0x01, 0x30, 0x63, 0x01, 0x30, 0x55, 0x01, 0x30, 0x53, 0x01, 0x30, 0x42, 0x01, 0x30, 0x55, 0x01, 0x30, 0x44, 0x01, 0x30, 0x10, 0x01, 0x30, 0x7d, 0x01, 0x30, 0x55, 0x01, 0x30, 0x43, 0x01, 0x30, 0x43, 0x01, 0x30, 0x51, 0x01, 0x30, 0x57, 0x01, 0x30, 0x55, 0xff };

std::map<std::string, const unsigned char*> stringSections = {
    { "str1", str1_section },
    { "str2", str2_section },
};

std::map<std::string, size_t> stringSizes = {
    { "str1", sizeof(str1_section) },
    { "str2", sizeof(str2_section) },
};

std::string DecryptStrings(const unsigned char* bytecode, size_t length) {
    std::string result;
    size_t ip = 0;

    while (ip < length) {
        unsigned char opcode = bytecode[ip++];
        switch (opcode) {
            case OP_LOAD_CHAR: {
                if (ip + 1 >= length) return result;
                unsigned char key = bytecode[ip++];
                unsigned char encChar = bytecode[ip++];
                result += (encChar ^ key);
                break;
            }
            case OP_DONE:
                return result;
            default:
                std::cerr << "Unknown opcode: " << (int)opcode << "\n";
                return "";
        }
    }

    return result;
}

int main() {
    for (const auto& [name, section] : stringSections) {
        size_t length = stringSizes[name];
        std::string decoded = DecryptStrings(section, length);
        std::cout << name << ": " << decoded << "\n";
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
