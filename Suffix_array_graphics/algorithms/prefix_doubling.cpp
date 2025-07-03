#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <direct.h>
#include <sys/stat.h>
#endif

using namespace std;

// Crea la carpeta ./data si no existe
void createDataFolder() {
#ifdef _WIN32
    struct _stat info;
    if (_stat("data", &info) != 0) {
        _mkdir("data");
    }
#else
    mkdir("data", 0777);
#endif
}

// Genera un string aleatorio de longitud 'length'
string generate_random_string(int length) {
    const string charset = "abcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, charset.size() - 1);

    string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i)
        result += charset[dis(gen)];
    return result;
}

// Devuelve el uso pico de memoria (en KB)
size_t getPeakMemoryUsageKB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.PeakWorkingSetSize / 1024;
#else
    return 0;
#endif
}

// Algoritmo de construcción del suffix array usando Prefix Doubling
vector<int> build_suffix_array_prefix_doubling(const string& s) {
    int n = s.size();
    vector<int> sa(n), rank(n), temp(n);

    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank[i] = s[i];
    }

    for (int k = 1; k < n; k *= 2) {
        auto compare = [&](int i, int j) {
            if (rank[i] != rank[j]) return rank[i] < rank[j];
            int ri = (i + k < n) ? rank[i + k] : -1;
            int rj = (j + k < n) ? rank[j + k] : -1;
            return ri < rj;
        };
        sort(sa.begin(), sa.end(), compare);

        temp[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            temp[sa[i]] = temp[sa[i - 1]] + (compare(sa[i - 1], sa[i]) ? 1 : 0);
        rank = temp;
    }

    return sa;
}

int main() {
    createDataFolder();

    ofstream csv("data/resultados_prefix_doubling.csv");
    if (!csv.is_open()) {
        cerr << "Error al abrir el archivo CSV.\n";
        return 1;
    }
    csv << "Tamaño,Tiempo(s),Memoria Pico (KB)\n";

    vector<int> sizes = {
        1000, 10000, 30000, 60000, 100000, 
        200000, 400000, 600000, 800000, 
        1000000, 1250000, 1500000, 2000000
    };


    for (int size : sizes) {
        string input = generate_random_string(size);

        auto start = chrono::high_resolution_clock::now();
        vector<int> sa = build_suffix_array_prefix_doubling(input);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        size_t peak_memory = getPeakMemoryUsageKB();

        cout << "Tamaño: " << size
             << ", Tiempo: " << duration.count()
             << " s, Memoria pico: " << peak_memory << " KB" << endl;

        csv << size << "," << duration.count() << "," << peak_memory << "\n";
    }

    csv.close();
    cout << "CSV guardado exitosamente en ./data/resultados_prefix_doubling.csv\n";
    return 0;
}


//g++ -std=c++17 algorithms/prefix_doubling.cpp -o algorithms/prefix_doubling.exe -lpsapi
//.\algorithms\prefix_doubling.exe