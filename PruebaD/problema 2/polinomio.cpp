#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdint>

// Implementación simple de BigInt en base 1e9 (header-only, portable)
// Suficiente para este ejercicio (suma, multiplicación, comparación e impresión)
class BigInt {
public:
    using u32 = uint32_t;
    using u64 = uint64_t;
    static const u32 BASE = 1000000000u; // 1e9

    BigInt(u64 v = 0) { fromUInt(v); }

    void fromUInt(u64 v) {
        a.clear();
        while (v > 0) {
            a.push_back(static_cast<u32>(v % BASE));
            v /= BASE;
        }
    }

    bool isZero() const { return a.empty(); }

    std::string toString() const {
        if (isZero()) return "0";
        std::ostringstream oss;
        int n = static_cast<int>(a.size());
        oss << a[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            oss << std::setw(9) << std::setfill('0') << a[i];
        }
        return oss.str();
    }

    // addition
    BigInt& operator+=(const BigInt& o) {
        u64 carry = 0;
        size_t n = std::max(a.size(), o.a.size());
        if (a.size() < n) a.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            u64 sum = carry + a[i] + (i < o.a.size() ? o.a[i] : 0);
            a[i] = static_cast<u32>(sum % BASE);
            carry = sum / BASE;
        }
        if (carry) a.push_back(static_cast<u32>(carry));
        normalize();
        return *this;
    }

    friend BigInt operator+(BigInt a, const BigInt& b) { a += b; return a; }

    // multiplication (schoolbook)
    friend BigInt operator*(const BigInt& x, const BigInt& y) {
        if (x.isZero() || y.isZero()) return BigInt(0);
        std::vector<u64> tmp(x.a.size() + y.a.size());
        for (size_t i = 0; i < x.a.size(); ++i) {
            u64 carry = 0;
            for (size_t j = 0; j < y.a.size() || carry; ++j) {
                u64 cur = tmp[i + j] + carry + u64(x.a[i]) * (j < y.a.size() ? u64(y.a[j]) : 0);
                tmp[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        BigInt res;
        res.a.resize(tmp.size());
        for (size_t i = 0; i < tmp.size(); ++i) res.a[i] = static_cast<u32>(tmp[i]);
        res.normalize();
        return res;
    }

    BigInt& operator*=(const BigInt& o) { *this = (*this) * o; return *this; }

    // compare
    int cmp(const BigInt& o) const {
        if (a.size() != o.a.size()) return a.size() < o.a.size() ? -1 : 1;
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            if (a[i] != o.a[i]) return a[i] < o.a[i] ? -1 : 1;
        }
        return 0;
    }

    bool operator==(const BigInt& o) const { return cmp(o) == 0; }
    bool operator!=(const BigInt& o) const { return cmp(o) != 0; }
    bool operator<(const BigInt& o) const { return cmp(o) < 0; }
    bool operator>(const BigInt& o) const { return cmp(o) > 0; }

private:
    std::vector<u32> a; // least-significant chunk first
    void normalize() {
        while (!a.empty() && a.back() == 0) a.pop_back();
    }
};

// stream operator
static inline std::ostream& operator<<(std::ostream& os, const BigInt& v) {
    os << v.toString();
    return os;
}

// Potencia rápida que devuelve BigInt (exponenciación binaria)
BigInt ipow(long long base, int exp) {
    BigInt resultado = BigInt(1);
    if (exp <= 0) return resultado;
    BigInt b = BigInt(static_cast<unsigned long long>(base));
    while (exp > 0) {
        if (exp & 1) resultado *= b;
        b *= b;
        exp >>= 1;
    }
    return resultado;
}

// Genera los coeficientes de (x+1)^n usando el triángulo de Pascal
std::vector<BigInt> generarCoeficientes(int n) {
    if (n == 0) return { BigInt(1) };

    std::vector<BigInt> filaAnterior = { BigInt(1) };
    std::vector<BigInt> filaActual;

    for (int i = 1; i <= n; ++i) {
        filaActual.resize(i + 1);
        filaActual[0] = BigInt(1);
        for (int j = 1; j < i; ++j) {
            filaActual[j] = filaAnterior[j - 1] + filaAnterior[j];
        }
        filaActual[i] = BigInt(1);
        filaAnterior = filaActual;
    }

    return filaActual;
}

// Muestra el polinomio (x+1)^n de forma legible
void mostrarPolinomio(const std::vector<BigInt>& coefs) {
    int n = static_cast<int>(coefs.size()) - 1;
    if (n < 0) return;

    std::cout << "f(x) = ";
    for (int i = 0; i <= n; ++i) {
        BigInt c = coefs[i];
        int potencia = n - i;

        if (c > BigInt(1) || potencia == 0) {
            std::cout << c;
        } else if (c == BigInt(1) && potencia > 0) {
            // don't print 1
        } else if (c == BigInt(0)) {
            continue;
        }

        if (potencia > 0) {
            if (c > BigInt(1)) std::cout << "*";
            std::cout << "x";
            if (potencia > 1) std::cout << "^" << potencia;
        }

        if (i < n) std::cout << " + ";
    }
    std::cout << std::endl;
}

// Calcula el polinomio para x dado (muestra pasos) y devuelve el resultado
BigInt calcularPolinomio(const std::vector<BigInt>& coefs, int x) {
    int n = static_cast<int>(coefs.size()) - 1;
    BigInt sumaTotal = BigInt(0);

    std::cout << "\nCalculando f(" << x << ") paso a paso:" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (int i = 0; i <= n; ++i) {
        BigInt c = coefs[i];
        int potencia = n - i;

        BigInt valorPotencia = ipow(x, potencia);
        BigInt valorTermino = c * valorPotencia;

        sumaTotal += valorTermino;

        std::cout << "   Término (" << c << "*x^" << potencia << "): ";
        std::cout << c << " * (" << x << "^" << potencia << ") = ";
        std::cout << c << " * " << valorPotencia << " = " << valorTermino << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Suma total = " << sumaTotal << std::endl << std::endl;

    BigInt comprobacion = ipow(x + 1, n);
    std::cout << "Comprobación: (" << x << " + 1)^" << n << " = " << (x + 1) << "^" << n << " = " << comprobacion << std::endl;
    if (sumaTotal == comprobacion) {
        std::cout << "(El resultado coincide)" << std::endl;
    } else {
        std::cout << "(Error: El resultado NO coincide, posible desbordamiento)" << std::endl;
    }

    return sumaTotal;
}


int main() {
    int n = -1;
    int x = 0;

    std::cout << "Generador de Polinomio (x+1)^n" << std::endl;
    std::cout << "==============================" << std::endl;

    while (n < 0) {
        std::cout << "\n2a) Ingrese el grado del polinomio (n, entero no negativo): ";
        std::cin >> n;
        if (n < 0) std::cout << "Error: 'n' debe ser 0 o mayor." << std::endl;
    }

    // 1. Generar coeficientes (medir tiempo)
    auto t_gen_start = std::chrono::high_resolution_clock::now();
    std::vector<BigInt> coeficientes = generarCoeficientes(n);
    auto t_gen_end = std::chrono::high_resolution_clock::now();

    std::cout << "   Coeficientes generados (Fila " << n << " de Pascal): { ";
    for (int i = 0; i < static_cast<int>(coeficientes.size()); ++i) {
        std::cout << coeficientes[i] << (i == static_cast<int>(coeficientes.size()) - 1 ? "" : ", ");
    }
    std::cout << " }" << std::endl;

    // 2. Mostrar polinomio (medir tiempo)
    std::cout << "\n2b) Polinomio generado:" << std::endl;
    auto t_show_start = std::chrono::high_resolution_clock::now();
    mostrarPolinomio(coeficientes);
    auto t_show_end = std::chrono::high_resolution_clock::now();

    // 3. Calcular por pasos (medir tiempo)
    std::cout << "\n2c) Ingrese el valor de 'x' para calcular f(x): ";
    std::cin >> x;
    auto t_calc_start = std::chrono::high_resolution_clock::now();
    BigInt resultado = calcularPolinomio(coeficientes, x);
    auto t_calc_end = std::chrono::high_resolution_clock::now();

    // Escribir resultados y tiempos si n==100
    if (n == 100) {
        std::ofstream out("resultados_n100.txt");
        if (out) {
            using ms = std::chrono::duration<double, std::milli>;
            out << "Resultados para n=100, x=" << x << "\n";
            out << "Tiempo generacion (ms): " << ms(t_gen_end - t_gen_start).count() << "\n";
            out << "Tiempo mostrar (ms): " << ms(t_show_end - t_show_start).count() << "\n";
            out << "Tiempo calcular (ms): " << ms(t_calc_end - t_calc_start).count() << "\n";
            out << "Resultado f(" << x << ") = " << resultado << "\n";
            out.close();
            std::cout << "Tiempos y resultado escritos en 'resultados_n100.txt'" << std::endl;
        } else {
            std::cout << "No se pudo abrir 'resultados_n100.txt' para escritura." << std::endl;
        }
    }

    return 0;
}
