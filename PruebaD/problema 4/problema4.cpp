#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <sstream>

// Traductor simple de palabras reservadas de C a español.
// Lee código C desde memoria (string) o desde un fichero y reemplaza
// solamente identificadores que coinciden exactamente con palabras reservadas.
// No modifica literales de cadena/char ni comentarios.

static const std::unordered_map<std::string, std::string> translations = {
    {"auto","automático"}, {"break","romper"}, {"case","caso"}, {"char","carácter"},
    {"const","constante"}, {"continue","continuar"}, {"default","por_defecto"}, {"do","hacer"},
    {"double","doble"}, {"else","sino"}, {"enum","enumeración"}, {"extern","externo"},
    {"float","flotante"}, {"for","para"}, {"goto","ir_a"}, {"if","si"},
    {"inline","en_linea"}, {"int","entero"}, {"long","largo"}, {"register","registro"},
    {"restrict","restringido"}, {"return","retornar"}, {"short","corto"}, {"signed","con_signo"},
    {"sizeof","tamano"}, {"static","estatico"}, {"struct","estructura"}, {"switch","segun"},
    {"typedef","tipo"}, {"union","union"}, {"unsigned","sin_signo"}, {"void","vacio"},
    {"volatile","volatile"}, {"while","mientras"}, {"_Bool","booleano"}, {"_Complex","complejo"},
    {"_Imaginary","imaginario"}
};

// Detecta un identificador (letra o '_' seguido de letras/dígitos/_)
static inline bool isIdentStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_'; }
static inline bool isIdentBody(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

std::string translateKeywords(const std::string &src) {
    std::string out;
    out.reserve(src.size());

    size_t i = 0, n = src.size();
    while (i < n) {
        char c = src[i];
        // Strings
        if (c == '"' || c == '\'') {
            char quote = c;
            out.push_back(c);
            ++i;
            while (i < n) {
                char d = src[i];
                out.push_back(d);
                ++i;
                if (d == '\\') { // escape: copy next char too if any
                    if (i < n) { out.push_back(src[i]); ++i; }
                } else if (d == quote) {
                    break;
                }
            }
            continue;
        }

        // Comments
        if (c == '/' && i + 1 < n) {
            char d = src[i+1];
            if (d == '/') {
                // line comment
                out.append(src, i, 2);
                i += 2;
                while (i < n && src[i] != '\n') { out.push_back(src[i]); ++i; }
                continue;
            } else if (d == '*') {
                // block comment
                out.append(src, i, 2);
                i += 2;
                while (i + 1 < n) {
                    if (src[i] == '*' && src[i+1] == '/') { out.append("*/"); i += 2; break; }
                    out.push_back(src[i]); ++i;
                }
                continue;
            }
        }

        // Identifier
        if (isIdentStart(c)) {
            size_t j = i + 1;
            while (j < n && isIdentBody(src[j])) ++j;
            std::string token = src.substr(i, j - i);
            auto it = translations.find(token);
            if (it != translations.end()) {
                out += it->second; // reemplaza por traducción
            } else {
                out += token; // copia tal cual
            }
            i = j;
            continue;
        }

        // cualquier otro caracter
        out.push_back(c);
        ++i;
    }

    return out;
}

int main(int argc, char** argv) {
    std::string src;
    if (argc >= 2) {
        // leer fichero especificado
        std::ifstream in(argv[1], std::ios::binary);
        if (!in) {
            std::cerr << "No se pudo abrir fichero: " << argv[1] << std::endl;
            return 1;
        }
        std::ostringstream ss;
        ss << in.rdbuf();
        src = ss.str();
    } else {
        // leer desde stdin
        std::ostringstream ss;
        ss << std::cin.rdbuf();
        src = ss.str();
    }

    std::string out = translateKeywords(src);

    // Si se pasó un segundo argumento, escribir a fichero de salida
    if (argc >= 3) {
        std::ofstream o(argv[2], std::ios::binary);
        if (!o) {
            std::cerr << "No se pudo crear fichero de salida: " << argv[2] << std::endl;
            return 2;
        }
        o << out;
    } else {
        // escribir a stdout
        std::cout << out;
    }

    return 0;
}
