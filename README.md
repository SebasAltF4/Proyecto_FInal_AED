# Proyecto_Final_AED

**Estructura de datos Suffix Array – Análisis y Visualización**

Este proyecto implementa y analiza diferentes algoritmos para la construcción de arreglos de sufijos (*Suffix Arrays*), una estructura fundamental en bioinformática, compresión de datos y búsqueda de patrones en texto.

Incluye:  
- Implementaciones de algoritmos en C++ (Prefix Doubling, SA-IS y una variante simplificada).
- Medición y análisis de rendimiento (tiempo de ejecución y uso de memoria).
- Visualización de resultados mediante gráficos comparativos.
- Página web interactiva para mostrar el funcionamiento y construcción del Suffix Array paso a paso.

---

## 📂 Contenido

- **algorithms/** – Implementaciones en C++ de:
  - Prefix Doubling
  - SA-IS
  - Variante simplificada de SA-IS (didáctica)
- **data/** – Resultados de pruebas (CSV) con métricas de tiempo y memoria.
- **Suffix_array_graphics/** – Scripts para generar y visualizar gráficos de rendimiento.
- **web_visualizer/** – Página web en JavaScript/HTML que permite ingresar una palabra y visualizar de forma interactiva la construcción del Suffix Array.

---

## 📊 Lo que se hizo

1. **Implementación de algoritmos:**
   - Prefix Doubling: Construcción del Suffix Array en O(n log n).
   - SA-IS: Algoritmo eficiente en tiempo lineal O(n).
   - Variante simplificada de SA-IS para visualización didáctica.

2. **Análisis de rendimiento:**
   - Medición de tiempo de ejecución y memoria pico para cadenas de distintos tamaños (desde 1 000 hasta 2 000 000 caracteres).
   - Gráficos comparativos frente a funciones de referencia: O(1), O(log n), O(n), O(n log n), O(n²).

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

> **Nota:** El flag `-lpsapi` es necesario en Windows para medir el uso de memoria.

---

## Visualización web

Para probar la visualización interactiva, abre el archivo `Suffix_array_visualizator/frontend/index.html` en tu navegador favorito e ingresa cualquier palabra para ver cómo se arma y ordena el arreglo de sufijos paso a paso.

---

**Requisitos:**
- Compilador C++ compatible con C++17
- Python 3 (para ejecutar scripts de gráficos, si aplica)
- Navegador web (Chrome, Brave, Edge, etc)

