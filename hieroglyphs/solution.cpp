#include<bits/stdc++.h>
#include "hieroglyphs.h"

using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int, int>;

const int ALPHABET_SIZE = 200001;

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

vi get_candidate(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
    }

    vi c;
    queue<int> qa;
    queue<int> qb;

    for (int i=0; i < n; ++i) {
        if (occ_a[a[i]] <= occ_b[a[i]]) {
            qa.push(i);
        }
    }
    for (int i=0; i < m; ++i) {
        if (occ_a[b[i]] > occ_b[b[i]]) {
            qb.push(i);
        }
    }

    int i_a_curr = 0;
    int i_b_curr = 0;
    int i_a_next = 0;
    int i_b_next = 0;
    vi occ_a_curr = vi(occ_a);
    vi occ_a_next = vi(occ_a);
    vi occ_b_curr = vi(occ_b);
    vi occ_b_next = vi(occ_b);

    while(!qa.empty() && !qb.empty()) {
        while(i_a_next < qa.front()) {
            occ_a_next[a[i_a_next]]--;
            i_a_next++;
        }
        while(i_b_next < qb.front()) {
            occ_b_next[b[i_b_next]]--;
            i_b_next++;
        }

        int x = a[i_a_next];
        int y = b[i_b_next];

        int occ_x = occ_a_next[x];
        int occ_y = occ_b_next[y];

        bool a_good = (occ_a_next[y] >= occ_y && occ_b_curr[x] > occ_b_next[x]);
        bool b_good = (occ_b_next[x] >= occ_x && occ_a_curr[y] > occ_a_next[y]);

        if (a_good && b_good) return {-1};
        if (!a_good && !b_good) return {-1};

        if(a_good) {
            c.push_back(x);
            qa.pop();
            while(i_a_curr <= i_a_next) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            while(b[i_b_curr] != x) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            occ_b_curr[b[i_b_curr]]--;
            i_b_curr++;
        }
        else {
            c.push_back(y);
            qb.pop();
            while(i_b_curr <= i_b_next) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            while(a[i_a_curr] != y) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            occ_a_curr[a[i_a_curr]]--;
            i_a_curr++;
        }
    }

    while(!qa.empty()) {
        c.push_back(a[qa.front()]);
        qa.pop();
    }
    while(!qb.empty()) {
        c.push_back(b[qb.front()]);
        qb.pop();
    }

    return ((is_subsequence(a, c) && is_subsequence(b, c)) ? c : vi({-1}));
}

vi index_vector(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();
    vi v(m);
    vi prev_occ_b(ALPHABET_SIZE, -1);
    vvi a_occ(ALPHABET_SIZE);
    for (int i=0; i < n; ++i) {
        a_occ[a[i]].push_back(i);
    }
    for (int i=0; i < ALPHABET_SIZE; ++i) {
        a_occ[i].push_back(n);
    }
    vector<ii> min_stack;
    for (int i=0; i < m; ++i) {
        if (prev_occ_b[b[i]] == -1) {
            v[i] = a_occ[b[i]][0];
        }
        else {
            int min_val = lower_bound(min_stack.begin(), min_stack.end(), ii(prev_occ_b[b[i]], -1))->second;
            if (min_val < n) v[i] = *lower_bound(a_occ[b[i]].begin(), a_occ[b[i]].end(), min_val+1);
            else v[i] = n;
        }
        while(!min_stack.empty() && min_stack.back().second >= v[i]) {
            min_stack.pop_back();
        }
        min_stack.emplace_back(i, v[i]);
        prev_occ_b[b[i]] = i;
    }
    return v; 
}

vi reverse_index_vector(vi a, vi b) {
    int n = a.size();
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    vi v = index_vector(a, b);
    reverse(v.begin(), v.end());
    for (int& x : v) x = n-1-x;
    return v; 
}

bool verify(const vi& a, const vi& b, const vi& c) {
    if (c == vi({-1})) return false;
    if (c == vi()) return true; 
    int n = a.size();
    int m = b.size();
    int l = c.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
    }

    vi iva = index_vector(a, c);
    vi ivb = index_vector(b, c);
    vi riva = reverse_index_vector(a, c);
    vi rivb = reverse_index_vector(b, c);

    int cai = 0;
    int cbi = 0;
    vector<ii> a_stack;
    vector<ii> b_stack;

    for (int i=0; i < l; ++i) {
        if (occ_a[c[i]] <= occ_b[c[i]]) {
            while (a[cai] != c[i]) cai++;

            while (!a_stack.empty() && a_stack.back().second <= rivb[i]) a_stack.pop_back();
            a_stack.emplace_back(cai, rivb[i]);

            while (!b_stack.empty() && b_stack.back().second < cai) b_stack.pop_back();
            if (!b_stack.empty() && b_stack.back().first > ivb[i]) {
                return false;
            }

            cai++;
        }
        else {
            while (b[cbi] != c[i]) cbi++;

            while (!b_stack.empty() && b_stack.back().second <= riva[i]) b_stack.pop_back();
            b_stack.emplace_back(cbi, riva[i]);

            while (!a_stack.empty() && a_stack.back().second < cbi) a_stack.pop_back();
            if (!a_stack.empty() && a_stack.back().first > iva[i]) {
                return false;
            }

            cbi++;
        }
    }
    return true;
}

vector<int> ucs(vector<int> a, vector<int> b) {
    vi c = get_candidate(a, b);
    if (verify(a, b, c)) return c;
    return {-1};
}
