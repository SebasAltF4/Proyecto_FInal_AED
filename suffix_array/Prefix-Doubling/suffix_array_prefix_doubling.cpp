#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#endif

using namespace std;

// Genera un string aleatorio
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

// Medir el uso máximo de memoria (en KB)
size_t getPeakMemoryUsageKB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.PeakWorkingSetSize / 1024;
#else
    return 0; // Opcional: añadir implementación para Unix
#endif
}

// Algoritmo Prefix Doubling
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
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 250000, 500000};

    ofstream csv("resultados_prefix_doubling.csv");
    csv << "Tamaño,Tiempo(s),Memoria Pico (KB)" << endl;

    for (int size : sizes) {
        cout << "\nGenerando string de tamaño " << size << "..." << endl;
        string input = generate_random_string(size);

        auto start = chrono::high_resolution_clock::now();
        vector<int> sa = build_suffix_array_prefix_doubling(input);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        size_t peak_memory = getPeakMemoryUsageKB();

        cout << "Tamaño: " << size << ", Tiempo: " << duration.count()
             << " s, Memoria pico: " << peak_memory << " KB" << endl;

        csv << size << "," << duration.count() << "," << peak_memory << endl;
    }

    csv.close();
    cout << "\nResultados guardados en 'resultados_prefix_doubling.csv'" << endl;
    return 0;
}
