#include <iostream>
#include <string>

int stringToTruncatedInt(const std::string &floatStr)
{
    return static_cast<int>(std::stof(floatStr));
}

int stringToIntAutoBase(const std::string &numStr)
{
    return std::stoi(numStr, nullptr, 0);
}

int main()
{

    // Examples
    std::string s1 = "123.45";
    std::string s2 = "-99.999";
    std::string s3 = "0.5";
    std::string s4 = "-0.5";
    std::string s5 = "1.23e2";    // 1.23 * 10^2 = 123.0
    std::string s6 = "-44.56e-1"; // -4.56 * 10^-1 = -0.456
    std::string s7 = "100";       // Integer represented as float
    std::string s8 = "3.14f";     // 'f' suffix is ignored by stod but parsed ok
    std::string s9 = "  -25.9 ";  // Leading/trailing whitespace okay for stod

    std::cout << "--- Valid Conversions ---" << std::endl;
    const std::string testStrings[] = {s1, s2, s3, s4, s5, s6, s7, s8, s9};

    for (const auto &str : testStrings)
    {
        int result = stringToTruncatedInt(str);
        std::cout << "'" << str << "' -> " << result << std::endl;
        std::cout << "----------" << std::endl;
    }

    // --- Test Cases ---
    const char *testStrings2[] = {
        // Decimal
        "123",
        "-456",
        "0",
        "+789", // Explicit plus sign is often allowed by stoi/strtol
        // Octal (leading 0)
        "0177", // 1*8^2 + 7*8^1 + 7*8^0 = 64 + 56 + 7 = 127
        "-010", // -(1*8^1 + 0*8^0) = -8
        "00",   // 0
        // Hexadecimal (leading 0x or 0X)
        "0xFF",      // 15*16^1 + 15*16^0 = 240 + 15 = 255
        "-0X1a",     // -(1*16^1 + 10*16^0) = -(16 + 10) = -26
        "0x0",       // 0
        "  -0x20  ", // With whitespace (handled by trim) -> -32
    };

    for (const auto &str : testStrings2)
    {
        std::cout << "Input: \"" << str << "\"" << std::endl;
        int result = stringToIntAutoBase(str);
        std::cout << "Output: " << result << " (Decimal)" << std::endl;
        std::cout << "---------------------" << std::endl;
    }

    return 0;
}