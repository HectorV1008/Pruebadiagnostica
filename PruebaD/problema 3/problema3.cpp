#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

static std::string trim(const std::string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool isScientific(const std::string &s) {
    // Requiere presencia de exponente (e o E)
    static const std::regex sci(R"(^[+-]?(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)$)", std::regex::optimize);
    return std::regex_match(s, sci);
}

bool isIPv4(const std::string &s) {
    static const std::regex ipv4(R"(^(?:(25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)\.){3}(25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)$)", std::regex::optimize);
    return std::regex_match(s, ipv4);
}

bool isEmail(const std::string &s) {
    // Simple and practical email regex (doesn't cover 100% of RFC but is suitable)
    static const std::regex email(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)", std::regex::icase | std::regex::optimize);
    return std::regex_match(s, email);
}

int main(int argc, char** argv) {
    std::vector<std::string> inputs;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) inputs.emplace_back(argv[i]);
    } else {
        std::cout << "Ingrese cadenas (una por línea). Ctrl+D/Ctrl+Z para terminar.\n";
        std::string line;
        while (std::getline(std::cin, line)) {
            line = trim(line);
            if (!line.empty()) inputs.push_back(line);
        }
    }

    for (const auto &raw : inputs) {
        std::string s = trim(raw);
        if (s.empty()) continue;
        bool sci = isScientific(s);
        bool ip = isIPv4(s);
        bool mail = isEmail(s);

        std::cout << "Input: '" << s << "' -> ";
        bool any = false;
        if (sci) { std::cout << "ScientificNotation"; any = true; }
        if (ip)  { if (any) std::cout << ", "; std::cout << "IPv4"; any = true; }
        if (mail){ if (any) std::cout << ", "; std::cout << "Email"; any = true; }
        if (!any) std::cout << "(no match)";
        std::cout << std::endl;
    }

    return 0;
}
