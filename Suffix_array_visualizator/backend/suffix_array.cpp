#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum SuffixType { S_TYPE, L_TYPE };

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

    for (int i = n - 1; i >= 0; --i) {
        if (isLMS(i, t)) {
            sa[--bucket_ends[(int)s[i]]] = i;
        }
    }

    for (int i = 0; i < n; ++i) {
        int j = sa[i] - 1;
        if (j >= 0 && t[j] == L_TYPE) {
            sa[bucket_heads[(int)s[j]]++] = j;
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        int j = sa[i] - 1;
        if (j >= 0 && t[j] == S_TYPE) {
            sa[--bucket_ends[(int)s[j]]] = j;
        }
    }
}

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

    vector<int> bucket_sizes(256, 0);
    for (char c : s) bucket_sizes[(int)c]++;

    induced_sort(s, sa, t, bucket_sizes);

    return vector<int>(sa.begin() + 1, sa.end());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Debes ingresar la palabra como argumento." << endl;
        return 1;
    }

    string input = argv[1];
    vector<int> suffix_array = build_suffix_array_sa_is(input);

    cout << "[\n";
    for (size_t i = 0; i < suffix_array.size(); ++i) {
        int idx = suffix_array[i];
        cout << "  {\n";
        cout << "    \"rank\": " << i << ",\n";
        cout << "    \"index\": " << idx << ",\n";
        cout << "    \"original_char\": \"" << input[idx] << "\",\n";
        cout << "    \"length\": " << input.size() - idx << ",\n";
        cout << "    \"suffix\": \"" << input.substr(idx) << "\"\n";
        cout << "  }";
        if (i != suffix_array.size() - 1) cout << ",";
        cout << "\n";
    }
    cout << "]\n";

    return 0;
}
