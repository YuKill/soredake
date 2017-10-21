#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

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

    for (int k = 0; k < s_number; ++k) {
        string& s     = s_vector[k];
        int     s_len = s.size();

        vector<bool> in_solution(s_len, true);
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

            bool pass  = false;
            bool bpass = false;

            for (int j = 0; j < p_len; ++j) {
                bool expr_check   = s[i] == pattern[j] && check[j];
                bool bexpr_check = s[bi] == pattern[j] && bcheck[j + 2];

                local_check[j+1] = expr_check || local_check[j+1];
                pass             = expr_check || pass;

                local_bcheck[j+1] = bexpr_check || local_bcheck[j+1];
                bpass             = bexpr_check || bpass;

#ifdef DEBUG
                if (pass && expr_check)
                    cout << "\t s[" << i << "] in solution" << endl;
                if (bpass && bexpr_check)
                    cout << "\t s[" << bi << "] in solution" << endl;
#endif
            }

            in_solution[i]  = in_solution[i]  && pass;
            in_solution[bi] = in_solution[bi] && bpass;

            check  = local_check;
            bcheck = local_bcheck;
        }

#ifdef DEBUG
        cout << "Matched: ";
        for (int i = 0; i < s_len; ++i)
            if (in_solution[i]) cout << s[i];
            else cout << '*';
        cout << endl;
#endif
        for (int i = 0; i < s_len; ++i)
            if (in_solution[i]) cout << i << " ";
        cout << "-:-" << s << endl;
    }

    return 0;
}
