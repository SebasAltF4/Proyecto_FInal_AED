# Proyecto_FInal_AED

**Estructura de datos Suffix Array – Análisis y Visualización**

Este proyecto implementa y analiza diferentes algoritmos para la construcción de arreglos de sufijos (*Suffix Arrays*), una estructura de datos fundamental en bioinformática, compresión de datos y búsqueda de patrones en texto.

Incluye:  
- Implementación de algoritmos en C++.
- Medición y análisis de rendimiento (tiempo y memoria).
- Visualización de resultados mediante gráficos.
- Una página web interactiva para mostrar el funcionamiento y armado del Suffix Array.

---

## 📂 Contenido

- **algorithms/** – Implementaciones de los algoritmos:
  - Prefix Doubling
  - SA-IS
  - Variación simplificada de SA-IS para fines didácticos
- **data/** – Resultados de las pruebas en formato CSV (tiempos, memoria).
- **Suffix_array_graphics/** – Scripts para generar gráficos de rendimiento.
- **web_visualizer/** – Página web en JavaScript/HTML que permite al usuario ingresar una palabra y visualizar cómo se construye el arreglo de sufijos.

---

## 📊 Lo que se hizo

1. **Implementación de algoritmos:**
   - Prefix Doubling: construcción de Suffix Array en O(n log n).
   - SA-IS: algoritmo lineal O(n) altamente eficiente.
   - Versión simplificada de SA-IS para fines de visualización.

2. **Análisis de rendimiento:**
   - Medición de tiempo de ejecución y memoria pico consumida para cadenas de distintos tamaños (1 000 hasta 2 000 000 caracteres).
   - Generación de gráficos comparativos contra referencias O(1), O(log n), O(n), O(n log n), O(n²).

3. **Página web interactiva:**
   - Permite al usuario ingresar una palabra y observar:
     - Los sufijos ordenados lexicográficamente.
     - La posición (rank) y longitud de cada sufijo.
     - La construcción paso a paso del Suffix Array.

---

## 🚀 Cómo usar

### Ejecutar algoritmos (C++)

```bash
# Compilar Prefix Doubling
g++ -std=c++17 algorithms/prefix_doubling.cpp -o prefix_doubling.exe -lpsapi

# Ejecutar
./prefix_doubling.exe

# Compilar SA-IS
g++ -std=c++17 algorithms/sais.cpp -o sais.exe -lpsapi

# Ejecutar
./sais.exe
