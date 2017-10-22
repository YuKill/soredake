#include "bitarray.h"

/*
 * This algorithm tries to find the letters that are in some sequence 'C' that
 * is a subsequence of 'S' (which corresponds to the whole string) such that
 * 'C' equals to a pattern sequence 'P'.
 *
 * The sequence, in this case, may not be contiguous. However, it must be always
 * of increasing index.
 *
 * For example:
 *    01 02 03 04 05 06 07 08 09 10 11 12 13
 * S: e  u  e  h  e  e  u  h  h  e  h  u  h
 * P: hue
 * The solution would be the letters that have index '4', '7' and '10'.
 * That is because: 
 * - for letters from '0-3': there are no leading 'h' (first letter of 'P');
 * - for letters from '5-6': there are no 'u' preceding the 'e's;
 * - for letters from '8-9': there are neither 'u's nor 'e's (one after the other); and
 * - for letters from '11-13': there are no 'e's.
 */

int main(int argc, char** argv) {
    string              pattern;
    vector<string>      s_vector;
    vector<vector<int>> pos_vector;

    getline(cin, pattern);
    for (string line; getline(cin, line);)
        s_vector.push_back(line);

#ifdef DEBUG
    cout << "Pattern: " << pattern << endl;
#endif

    int s_number = s_vector.size();
    int p_len    = pattern.length();

    unsigned buckets = get_uarray_buckets(p_len);

    for (int k = 0; k < s_number; ++k) {
        string& s     = s_vector[k];
        int     s_len = s.size();

        // This is used to check what patter letters some letter of the
        // string can take place.
        vector<unsigned*> in_solution(s_len, nullptr);

        for (int i = 0; i < s_len; ++i) {
            in_solution[i] = new unsigned[buckets];
            init_uarray(in_solution[i], p_len, true);
        }

        // The core of this algorithm. We iterate the string in order to
        // build these vectors.
        //
        // Notice that we are iterating both forwardly and backwardly at
        // the same time. Here we explain 'check' when iterating forwardly.
        // Given that, 'bcheck' is about the same, but backwardly.
        //
        // For every 'i' varying from '0' to the size of the string:
        // For every 'j' varying from '0' to the size of the pattern:
        // - If the current letter 's[i]' equals the current pattern letter
        //   'p[j]' and the letter 'p[j-1]' has been already seen:
        //   >> 'p[j]' has also already been seen;
        //   >> 's[i]' may represent the 'j'-th letter of the pattern.
        vector<bool> check(p_len + 2, false);
        vector<bool> bcheck(p_len + 2, false);

        check[0]          = true;
        bcheck[p_len + 1] = true;

#ifdef DEBUG
        cout << "==----------------------------------------==" << endl;
        cout << "String: " << s << endl;
#endif
        for (int i = 0; i < s_len; ++i) {
            int bi = s_len - i - 1;

#ifdef DEBUG
            cout << "## s[" << i << "]:" << s[i]
                << " -- s[" << bi << "]:" << s[bi] << "##" << endl;
#endif

            vector<bool> local_check  = check;
            vector<bool> local_bcheck = bcheck;

            // We use 'arr' ('barr') to keep track of the 'p[j]' that
            // 's[i]' ('s[bi]') can be when iterating forwardly (backwardly).
            unsigned arr[buckets];
            unsigned barr[buckets];

            init_uarray(arr, p_len, false);
            init_uarray(barr, p_len, false);

            for (int j = 0; j < p_len; ++j) {
                bool expr_check   = s[i] == pattern[j] && check[j];
                bool bexpr_check = s[bi] == pattern[j] && bcheck[j + 2];

                local_check[j+1] = expr_check || local_check[j+1];
                set_uarray_bit(arr, j, expr_check);

                local_bcheck[j+1] = bexpr_check || local_bcheck[j+1];
                set_uarray_bit(barr, j, bexpr_check);

#ifdef DEBUG
                if (expr_check)
                    cout << "\t    ", print_uarray(arr, p_len);
                if (bexpr_check)
                    cout << "\t (b)", print_uarray(barr, p_len);
#endif
            }

            // The 'i'('bi')-th letter of the string may only be in some sequence
            // if both the forwardly and backwardly iterations agree on some 'j'.
            land_uarray(in_solution[i], arr, in_solution[i], p_len);
            land_uarray(in_solution[bi], barr, in_solution[bi], p_len);
#ifdef DEBUG
            cout << ">>    ";
            print_uarray(in_solution[i], p_len);
            cout << ">> (b)";
            print_uarray(in_solution[bi], p_len);
#endif
            // Update checks.
            check  = local_check;
            bcheck = local_bcheck;
        }

#ifdef DEBUG
        cout << "Matched: ";
        for (int i = 0; i < s_len; ++i)
            if (has_onebit_uarray(in_solution[i], p_len)) cout << s[i];
            else cout << '*';
        cout << endl;
#endif

        vector<int> pos_vector;

        for (int i = 0; i < s_len; ++i)
            if (has_onebit_uarray(in_solution[i], p_len)) pos_vector.push_back(i);

        if (pos_vector.empty()) continue;

        // Printing intervals, not indexes.
        cout << pos_vector[0] << ";";
        for (int i = 1, e = pos_vector.size(); i < e; ++i) {
            if (pos_vector[i-1] != pos_vector[i] - 1)
                cout << pos_vector[i-1] << " " << pos_vector[i] << ";";
        }
        cout << pos_vector[pos_vector.size() - 1];
        cout << "-:-" << s << endl;
    }

    return 0;
}
