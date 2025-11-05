# Proyecto: PruebaD

Este repositorio contiene varios ejercicios en C/C++ y un script en JavaScript (Node.js) usados para practicar manipulación de cadenas, aritmética con enteros grandes y análisis léxico simple.

Estructura principal
- `problema 1/` — `FormatoFEN.cpp` : validador de cadenas FEN (ajedrez).
- `problema 2/` — `polinomio.cpp` (C++) y `polinomio.js` (Node.js): generación de coeficientes de (x+1)^n usando el triángulo de Pascal, muestra del polinomio y evaluación paso a paso. Mide tiempos y escribe resultados para n=100.
- `problema 3/` — `problema3.cpp` : reconocedor (expresiones regulares) para notación científica, IPv4 y correos.
- `problema 4/` — `problema4.cpp`, `example.c`, `translate.bat` : traductor sencillo de palabras reservadas de C a español (preserva literales y comentarios) y un ejemplo de uso.

Requisitos
- Windows (las instrucciones usan PowerShell; los comandos de compilación usan g++ del paquete TDM-GCC presente en el sistema).
- Node.js (para ejecutar `polinomio.js`): comprueba con `node -v`.
- g++ (TDM-GCC o similar) para compilar los .cpp. En tu entorno se usa `C:\TDM-GCC-64\bin\g++.exe`.

Notas sobre PowerShell
- PowerShell maneja redirecciones y comillas de forma distinta a cmd.exe. Para pasar contenido por stdin a un ejecutable nativo sin problemas se recomienda usar `Get-Content -Raw 'ruta' | & 'ruta\\a\\exe' > 'salida'` o, cuando el programa acepta un argumento de salida, pasar directamente la ruta de salida como segundo argumento.

1) `problema 1/` — `FormatoFEN.cpp`

Qué hace
- Valida si una cadena está en notación FEN válida. Comprueba los 6 campos (posición de piezas, color activo, enroque, casilla de peón al paso, medio-movimiento y movimiento completo) con reglas básicas del estándar.

Cómo compilar
```powershell
& 'C:\TDM-GCC-64\bin\g++.exe' -std=c++17 -O2 -o "FormatoFEN.exe" "problema 1\FormatoFEN.cpp"
```

Cómo ejecutar
```powershell
.\FormatoFEN.exe
# El programa tiene una función main con casos de prueba; imprime si cada FEN es válido.
```

2) `problema 2/` — `polinomio.cpp` y `polinomio.js`

Resumen
- Ambos implementan la generación de coeficientes del polinomio (x+1)^n usando el triángulo de Pascal (o la relación multiplicativa de los binomios). Muestran el polinomio y realizan la evaluación por pasos f(x) = (x+1)^n usando enteros grandes.
- `polinomio.js` (Node.js) fue creado y probado: usa `BigInt` de JavaScript, imprime cada término y suma parcial, mide tiempos (generación, construcción de cadena, evaluación) y, por defecto, cuando `n==100` escribe `resultados_n100.txt` en la carpeta donde está el script.

`polinomio.js` — cómo usar
```powershell
# Ejecutar (por ejemplo n=5, x=2):
node "problema 2\polinomio.js" 5 2

# Para n=100 (se crea el archivo de resultados junto al script):
node "problema 2\polinomio.js" 100 2

# Forzar nombre/ubicación de salida:
node "problema 2\polinomio.js" 100 2 --out "C:\ruta\mis_resultados.txt"
```

Notas
- Por defecto `polinomio.js` escribirá `resultados_n100.txt` en la misma carpeta que `polinomio.js` (usa `__dirname`).
- El script usa `BigInt`, por lo que requiere Node.js moderno (v12+).

`polinomio.cpp` — cómo compilar (si existe y quieres usar la versión C++)
```powershell
& 'C:\TDM-GCC-64\bin\g++.exe' -std=c++17 -O2 -o "problema2_polinomio.exe" "problema 2\polinomio.cpp"
# Ejecutar (dependiendo de la implementación del main en el .cpp):
.\problema2_polinomio.exe
```

3) `problema 3/` — `problema3.cpp`

Qué hace
- Programa que detecta si cadenas están en notación científica, si son direcciones IPv4 válidas o correos electrónicos válidos (usa `std::regex`).

Compilar y ejecutar
```powershell
& 'C:\TDM-GCC-64\bin\g++.exe' -std=c++17 -O2 -o "recognizer.exe" "problema 3\problema3.cpp"
# Ejecutar con argumentos o por stdin según la implementación (ejemplo):
.\recognizer.exe "3.14e-2"
```

4) `problema 4/` — traductor y utilidades

Archivos principales
- `keyword_translator.cpp` : traduce palabras reservadas C → español conservando literales y comentarios.
- `example.c` : ejemplo de código para probar el traductor.
- `translate.bat` : script por lotes para ejecutar `keyword_translator.exe` por doble‑clic o arrastrando un `.c` sobre el .bat.

Compilar el traductor
```powershell
& 'C:\TDM-GCC-64\bin\g++.exe' -std=c++17 -O2 -o "problema4_keyword_translator.exe" "problema 4\keyword_translator.cpp"
```

Ejemplos de ejecución (PowerShell)
```powershell
# Traducir y guardar en archivo destino (forma robusta):
& 'C:\Users\Hector\Desktop\PruebaD\problema 4\keyword_translator.exe' 'C:\Users\Hector\Desktop\PruebaD\problema 4\example.c' 'C:\Users\Hector\Desktop\PruebaD\problema 4\example_translated.c'

# Alternativa con pipeline (evita problemas de redirección en PowerShell):
Get-Content -Raw 'problema 4\example.c' | & 'C:\Users\Hector\Desktop\PruebaD\problema 4\keyword_translator.exe' > 'problema 4\example_translated.c'

# O usar el .bat arrastrando el .c sobre translate.bat
```

Comportamiento del traductor
- Hace una pasada léxica sencilla: mantiene intactos literales de cadena (`"..."`), literales de carácter (`'...'`) y comentarios (`//`, `/* ... */`).
- Reemplaza únicamente tokens que sean exactamente palabras reservadas del lenguaje C por su traducción en el mapa incluido en el código.

Archivos de salida generados durante la sesión

- `problema 4/example_translated.c` — ejemplo de salida del traductor (si lo ejecutaste previamente).

