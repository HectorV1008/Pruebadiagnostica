#include <iostream>
#include <string>
#include <vector>
#include <sstream> // Para std::istringstream
#include <cctype>  // Para std::isdigit
#include <stdexcept> // Para std::stoi

// --- Prototipos de funciones auxiliares ---

// Función para dividir una cadena por un delimitador
std::vector<std::string> split(const std::string& s, char delimiter);

// Funciones de validación para cada uno de los 6 campos FEN
bool validarPiezas(const std::string& campo);
bool validarColorActivo(const std::string& campo);
bool validarEnroque(const std::string& campo);
bool validarPeonPaso(const std::string& campo, char colorActivo);
bool validarMedioMovimiento(const std::string& campo);
bool validarMovimientoCompleto(const std::string& campo);

// --- Función principal de validación ---

/**
 * @brief Valida si una cadena dada está en notación FEN válida.
 * @param fen La cadena FEN a validar.
 * @return true si la cadena es un FEN válido, false en caso contrario.
 */
bool esFenValido(const std::string& fen) {
    // 1. Dividir la cadena en sus 6 campos
    std::vector<std::string> campos = split(fen, ' ');
    if (campos.size() != 6) {
        // Un FEN debe tener exactamente 6 campos separados por espacios
        return false;
    }

    // 2. Validar el campo 2 (Color Activo) primero, ya que el campo 4 depende de él.
    if (!validarColorActivo(campos[1])) {
        return false;
    }
    char colorActivo = campos[1][0]; // 'w' o 'b'

    // 3. Validar cada campo secuencialmente
    return validarPiezas(campos[0]) &&
           // campos[1] ya fue validado
           validarEnroque(campos[2]) &&
           validarPeonPaso(campos[3], colorActivo) &&
           validarMedioMovimiento(campos[4]) &&
           validarMovimientoCompleto(campos[5]);
}

// --- Implementación de funciones auxiliares ---

/**
 * @brief Divide una cadena en subcadenas basadas en un delimitador.
 */
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Campo 1: Posición de las piezas (ej: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")
 */
bool validarPiezas(const std::string& campo) {
    std::vector<std::string> filas = split(campo, '/');
    if (filas.size() != 8) {
        // Debe haber exactamente 8 filas (ranks)
        return false;
    }

    const std::string piezasValidas = "pnbrqkPNBRQK";

    for (const std::string& fila : filas) {
        if (fila.empty()) return false; // Una fila no puede estar vacía

        int anchuraFila = 0;
        for (char c : fila) {
            if (std::isdigit(c)) {
                if (c == '0') return false; // '0' no es un número válido
                anchuraFila += (c - '0'); // '1' se convierte en 1, '8' en 8
            } else if (piezasValidas.find(c) != std::string::npos) {
                anchuraFila++;
            } else {
                return false; // Carácter inválido
            }
        }

        if (anchuraFila != 8) {
            // La suma de piezas y números de cada fila debe ser 8
            return false;
        }
    }
    return true;
}

/**
 * @brief Campo 2: Color activo (ej: "w")
 */
bool validarColorActivo(const std::string& campo) {
    return campo == "w" || campo == "b";
}

/**
 * @brief Campo 3: Disponibilidad de enroque (ej: "KQkq" o "-")
 */
bool validarEnroque(const std::string& campo) {
    if (campo == "-") return true;
    if (campo.empty() || campo.length() > 4) return false;

    // La forma canónica es "KQkq". Verificamos que solo contenga
    // esos caracteres, sin duplicados y en el orden correcto.
    std::string campoOrdenado = "";
    if (campo.find('K') != std::string::npos) campoOrdenado += 'K';
    if (campo.find('Q') != std::string::npos) campoOrdenado += 'Q';
    if (campo.find('k') != std::string::npos) campoOrdenado += 'k';
    if (campo.find('q') != std::string::npos) campoOrdenado += 'q';

    // El campo original debe ser idéntico al campo construido y ordenado
    return campo == campoOrdenado;
}

/**
 * @brief Campo 4: Casilla de captura al paso (ej: "e3" o "-")
 */
bool validarPeonPaso(const std::string& campo, char colorActivo) {
    if (campo == "-") return true;
    if (campo.length() != 2) return false;

    char fila = campo[0];
    char rango = campo[1];

    if (fila < 'a' || fila > 'h') return false; // Fila inválida

    // La casilla de captura al paso depende del turno del jugador
    if (colorActivo == 'w') {
        // Si mueven las blancas, la casilla debe estar en el rango 6
        return rango == '6';
    } else {
        // Si mueven las negras, la casilla debe estar en el rango 3
        return rango == '3';
    }
}

/**
 * @brief Campo 5: Reloj de medio movimiento (ej: "0")
 */
bool validarMedioMovimiento(const std::string& campo) {
    if (campo.empty()) return false;
    for (char c : campo) {
        if (!std::isdigit(c)) return false; // Debe ser un número
    }
    
    // El estándar FEN lo define como un entero no negativo.
    // La comprobación de dígitos es suficiente, ya que no puede ser negativo.
    return true;
}

/**
 * @brief Campo 6: Número de movimiento completo (ej: "1")
 */
bool validarMovimientoCompleto(const std::string& campo) {
    if (campo.empty()) return false;
    for (char c : campo) {
        if (!std::isdigit(c)) return false;
    }

    // Debe ser un número *positivo* (>= 1)
    try {
        int valor = std::stoi(campo);
        return valor >= 1;
    } catch (...) {
        // En caso de desbordamiento (overflow) o un número mal formado como "0"
        return false;
    }
}

// --- Función main para probar el validador ---

int main() {
    // Casos de prueba
    std::string fenInicio = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string fenMedioJuego = "r1b1kbnr/p1ppqppp/1pn5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4";
    std::string fenInvalidoCampos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0"; // Faltan campos
    std::string fenInvalidoPiezas = "rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // Fila suma 9
    std::string fenInvalidoEnroque = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk - 0 1"; // Orden enroque mal
    std::string fenInvalidoPeonPaso = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"; // Peón paso en rango 3, pero mueven negras (correcto)
    std::string fenInvalidoPeonPaso2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e4 0 1"; // Peón paso en rango 4 (incorrecto)
    std::string fenInvalidoMovimiento = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"; // Movimiento 0 (inválido)

    std::cout << "Probando FEN...\n\n";

    std::cout << "'" << fenInicio << "': " 
              << (esFenValido(fenInicio) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenMedioJuego << "': "
              << (esFenValido(fenMedioJuego) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenInvalidoCampos << "': "
              << (esFenValido(fenInvalidoCampos) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenInvalidoPiezas << "': "
              << (esFenValido(fenInvalidoPiezas) ? "Válido" : "Inválido") << std::endl;
              
    std::cout << "'" << fenInvalidoEnroque << "': "
              << (esFenValido(fenInvalidoEnroque) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenInvalidoPeonPaso << "': "
              << (esFenValido(fenInvalidoPeonPaso) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenInvalidoPeonPaso2 << "': "
              << (esFenValido(fenInvalidoPeonPaso2) ? "Válido" : "Inválido") << std::endl;

    std::cout << "'" << fenInvalidoMovimiento << "': "
              << (esFenValido(fenInvalidoMovimiento) ? "Válido" : "Inválido") << std::endl;

    return 0;
}