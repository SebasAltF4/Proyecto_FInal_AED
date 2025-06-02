#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Tipos de sufijo
enum SuffixType { S_TYPE, L_TYPE };

// Comparar sufijos LMS (Leftmost S-Type)
bool isLMS(int i, const vector<SuffixType>& t) {
    return i > 0 && t[i] == S_TYPE && t[i - 1] == L_TYPE;
}

void induced_sort(const string& s, vector<int>& sa, const vector<SuffixType>& t,
                  const vector<int>& bucket_sizes) {
    int n = s.size();
    vector<int> bucket_ends(256, 0);
    vector<int> bucket_heads(256, 0);
    int sum = 0;

    for (int i = 0; i < 256; ++i) {
        bucket_heads[i] = sum;
        sum += bucket_sizes[i];
        bucket_ends[i] = sum;
    }

    fill(sa.begin(), sa.end(), -1);

    // Colocar sufijos LMS al final de cada bucket
    for (int i = n - 1; i >= 0; --i) {
        if (isLMS(i, t)) {
            sa[--bucket_ends[(int)s[i]]] = i;
        }
    }

    // Inducir L-Type
    for (int i = 0; i < n; ++i) {
        int j = sa[i] - 1;
        if (j >= 0 && t[j] == L_TYPE) {
            sa[bucket_heads[(int)s[j]]++] = j;
        }
    }

    // Inducir S-Type
    for (int i = n - 1; i >= 0; --i) {
        int j = sa[i] - 1;
        if (j >= 0 && t[j] == S_TYPE) {
            sa[--bucket_ends[(int)s[j]]] = j;
        }
    }
}

vector<int> build_suffix_array_sa_is(const string& s_input) {
    string s = s_input + '\0'; // Añadir terminador nulo
    int n = s.size();
    vector<int> sa(n, -1);
    vector<SuffixType> t(n);

    // Paso 1: determinar tipo de cada posición
    t[n - 1] = S_TYPE;
    for (int i = n - 2; i >= 0; --i) {
        if (s[i] < s[i + 1]) t[i] = S_TYPE;
        else if (s[i] > s[i + 1]) t[i] = L_TYPE;
        else t[i] = t[i + 1];
    }

    // Paso 2: contar frecuencia de caracteres
    vector<int> bucket_sizes(256, 0);
    for (char c : s) bucket_sizes[(int)c]++;

    // Paso 3: ordenar LMS y luego inducir sufijos
    induced_sort(s, sa, t, bucket_sizes);

    // Retornar SA sin el terminador '\0'
    return vector<int>(sa.begin() + 1, sa.end());
}

int main() {
    string input;
    cout << "Ingrese el texto: ";
    cin >> input;

    vector<int> suffix_array = build_suffix_array_sa_is(input);

    ofstream file("suffix_array.json");
    file << "[\n";
    for (size_t i = 0; i < suffix_array.size(); ++i) {
        int idx = suffix_array[i];
        file << "  {\n";
        file << "    \"rank\": " << i << ",\n";
        file << "    \"index\": " << idx << ",\n";
        file << "    \"original_char\": \"" << input[idx] << "\",\n";
        file << "    \"length\": " << input.size() - idx << ",\n";
        file << "    \"suffix\": \"" << input.substr(idx) << "\"\n";
        file << "  }";
        if (i != suffix_array.size() - 1) file << ",";
        file << "\n";
    }
    file << "]\n";
    file.close();

    cout << "Suffix Array generado con SA-IS en 'suffix_array.json'" << endl;
    return 0;
}
