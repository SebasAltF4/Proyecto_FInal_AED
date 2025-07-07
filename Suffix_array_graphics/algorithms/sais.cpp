#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <direct.h>
#include <sys/stat.h>
#endif

// Funciones de sistema
void createDataFolder() {
#ifdef _WIN32
    struct _stat info;
    if (_stat("data", &info) != 0) {
        _mkdir("data"); // Crear carpeta "data" si no existe
    }
#else
    mkdir("data", 0777); // Crear carpeta "data" en sistemas Unix-like
#endif
}

size_t getPeakMemoryUsageKB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.PeakWorkingSetSize / 1024; // Retorna memoria pico en KB
#else
    return 0; // No se implementa en otros sistemas
#endif
}

// Función para generar una cadena aleatoria
std::string generate_random_string(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charset.size() - 1);

    std::string result(length, '\0');  // Reservar espacio de antemano para evitar múltiples realocaciones
    for (int i = 0; i < length; ++i) {
        result[i] = charset[dis(gen)];
    }
    return result;
}

// SA-IS optimizado
void getBuckets(const std::vector<int>& s, std::vector<int>& bucket, int sigma, bool end) {
    std::fill(bucket.begin(), bucket.end(), 0); // Limpiar el bucket
    for (int c : s) bucket[c]++;
    int sum = 0;
    for (int i = 0; i < sigma; ++i) {
        sum += bucket[i];
        bucket[i] = end ? sum : (sum - bucket[i]); // Calcular los buckets
    }
}

void induceSAl(const std::vector<int>& s, std::vector<int>& sa, std::vector<int>& bucket, int sigma, const std::vector<bool>& t) {
    getBuckets(s, bucket, sigma, false);
    for (int i = 0; i < sa.size(); ++i) {
        int j = sa[i] - 1;
        if (j >= 0 && !t[j])
            sa[bucket[s[j]]++] = j;
    }
}

void induceSAs(const std::vector<int>& s, std::vector<int>& sa, std::vector<int>& bucket, int sigma, const std::vector<bool>& t) {
    getBuckets(s, bucket, sigma, true);
    for (int i = sa.size() - 1; i >= 0; --i) {
        int j = sa[i] - 1;
        if (j >= 0 && t[j])
            sa[--bucket[s[j]]] = j;
    }
}

// Implementación de SA-IS
std::vector<int> sais(const std::string& str, int sigma = 256) {
    int n = str.size();
    if (n == 0) return {}; // Si la cadena está vacía, retornar

    std::vector<int> s(n + 1);
    for (int i = 0; i < n; ++i) {
        s[i] = static_cast<unsigned char>(str[i]); // Convertir cada carácter a valor entero
    }
    s[n] = 0; // Agregar un sentinel

    int m = s.size();
    std::vector<int> sa(m, -1);
    std::vector<bool> t(m);
    t[m - 1] = true; // El último carácter es siempre S-type
    for (int i = m - 2; i >= 0; --i)
        t[i] = (s[i] < s[i + 1]) || (s[i] == s[i + 1] && t[i + 1]); // Determinar los tipos de caracteres

    std::vector<int> bucket(sigma, 0);
    getBuckets(s, bucket, sigma, true); // Llenar los buckets
    for (int i = 0; i < m; ++i)
        sa[i] = -1; // Inicializar SA

    // Colocar los sufijos LMS (Leftmost S-type)
    for (int i = 1; i < m; ++i)
        if (!t[i - 1] && t[i])
            sa[--bucket[s[i]]] = i;

    induceSAl(s, sa, bucket, sigma, t); // Inducir L-type
    induceSAs(s, sa, bucket, sigma, t); // Inducir S-type

    return std::vector<int>(sa.begin() + 1, sa.end()); // Eliminar el sentinel
}

// Función principal
int main() {
    createDataFolder(); // Crear la carpeta de datos

    std::ofstream csv("data/resultados_sais.csv");
    if (!csv.is_open()) {
        std::cerr << "Error al abrir el archivo CSV.\n";
        return 1;
    }
    csv << "Tamaño,Tiempo(s),Memoria Pico (KB)\n";

    // Tamaños de cadenas para probar
    std::vector<int> sizes = {
        1000, 10000, 30000, 60000, 100000,
        200000, 400000, 600000, 800000,
        1000000, 1250000, 1500000, 2000000
    };

    for (int size : sizes) {
        if (size == 0) {
            csv << "0,0,0\n";
            std::cout << "Tamaño: 0, Tiempo: 0 s, Memoria pico: 0 KB\n";
            continue;
        }

        std::string input = generate_random_string(size); // Generar cadena aleatoria
        std::cout << "Procesando tamaño: " << size << "...";

        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> sa = sais(input); // Ejecutar SA-IS
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        size_t peak_memory = getPeakMemoryUsageKB(); // Obtener memoria pico

        std::cout << " Listo. Tiempo: " << duration.count() << " s, Memoria: " << peak_memory << " KB\n";
        csv << size << "," << duration.count() << "," << peak_memory << "\n";
    }

    csv.close(); // Cerrar archivo CSV
    std::cout << "CSV guardado exitosamente en ./data/resultados_sais.csv\n";
    return 0;
}
