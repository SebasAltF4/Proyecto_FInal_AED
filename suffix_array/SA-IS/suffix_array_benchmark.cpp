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
#else
#include <sys/resource.h>
#endif

using namespace std;

// Tipos de sufijo
enum SuffixType { S_TYPE, L_TYPE };

// Verifica si una posición es LMS
bool isLMS(int i, const vector<SuffixType>& t) {
    return i > 0 && t[i] == S_TYPE && t[i - 1] == L_TYPE;
}

// Obtener los tamaños de los buckets por caracter
vector<int> get_bucket_sizes(const string& s, int alphabet_size) {
    vector<int> bucket_sizes(alphabet_size, 0);
    for (char c : s)
        bucket_sizes[(int)c]++;
    return bucket_sizes;
}

// Inicializa las posiciones finales de cada bucket
vector<int> get_bucket_ends(const vector<int>& bucket_sizes) {
    vector<int> bucket_ends(bucket_sizes.size());
    int sum = 0;
    for (size_t i = 0; i < bucket_sizes.size(); ++i) {
        sum += bucket_sizes[i];
        bucket_ends[i] = sum;
    }
    return bucket_ends;
}

// Inicializa las posiciones de inicio de cada bucket
vector<int> get_bucket_heads(const vector<int>& bucket_sizes) {
    vector<int> bucket_heads(bucket_sizes.size());
    int sum = 0;
    for (size_t i = 0; i < bucket_sizes.size(); ++i) {
        bucket_heads[i] = sum;
        sum += bucket_sizes[i];
    }
    return bucket_heads;
}

// Induced Sort (fase principal del SA-IS)
void induced_sort(const string& s, vector<int>& sa, const vector<SuffixType>& t, const vector<int>& bucket_sizes) {
    int n = s.size();
    fill(sa.begin(), sa.end(), -1);

    auto bucket_ends = get_bucket_ends(bucket_sizes);

    // Insertar LMS
    for (int i = n - 1; i >= 0; --i) {
        if (isLMS(i, t)) {
            sa[--bucket_ends[(int)s[i]]] = i;
        }
    }

    auto bucket_heads = get_bucket_heads(bucket_sizes);
    // Inducir L-type
    for (int i = 0; i < n; ++i) {
        int j = sa[i];
        if (j > 0 && t[j - 1] == L_TYPE) {
            sa[bucket_heads[(int)s[j - 1]]++] = j - 1;
        }
    }

    bucket_ends = get_bucket_ends(bucket_sizes);
    // Inducir S-type
    for (int i = n - 1; i >= 0; --i) {
        int j = sa[i];
        if (j > 0 && t[j - 1] == S_TYPE) {
            sa[--bucket_ends[(int)s[j - 1]]] = j - 1;
        }
    }
}

// Construcción del suffix array con SA-IS
vector<int> build_suffix_array_sa_is(const string& s_input) {
    string s = s_input + '\0';
    int n = s.size();
    vector<int> sa(n, -1);
    vector<SuffixType> t(n);

    t[n - 1] = S_TYPE;
    for (int i = n - 2; i >= 0; --i) {
        if (s[i] < s[i + 1]) t[i] = S_TYPE;
        else if (s[i] > s[i + 1]) t[i] = L_TYPE;
        else t[i] = t[i + 1];
    }

    auto bucket_sizes = get_bucket_sizes(s, 256);
    induced_sort(s, sa, t, bucket_sizes);

    return vector<int>(sa.begin() + 1, sa.end()); // Quitar '\0'
}

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
#endif
}

int main() {
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 250000, 500000, 1000000};

    ofstream csv("resultados_SA-IS.csv");
    csv << "Tamaño,Tiempo(s),Memoria Pico (KB)" << endl;

    for (int size : sizes) {
        cout << "\nGenerando string de tamaño " << size << "..." << endl;
        string input = generate_random_string(size);

        auto start = chrono::high_resolution_clock::now();
        vector<int> sa = build_suffix_array_sa_is(input);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        size_t peak_memory = getPeakMemoryUsageKB();

        cout << "Tamaño: " << size << ", Tiempo: " << duration.count()
             << " s, Memoria pico: " << peak_memory << " KB" << endl;

        csv << size << "," << duration.count() << "," << peak_memory << endl;
    }

    csv.close();
    cout << "\nResultados guardados en 'resultados_SA-IS.csv'" << endl;
    return 0;
}
