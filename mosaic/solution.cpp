#include "bits/stdc++.h"
#include "mosaic.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    auto cell_index = [](int i, int j){
        return ((i < j) - (i > j)) * (max(i, j) - min(i, j));
    };
    auto next_layer = [](vector<long long> l){
        if(l.size() < 3) return vector<long long>();
        int m = (int)l.size() / 2;
        l[m] = !(l[m-1] | l[m+1]);
        l.erase(l.begin() + m + 1);
        l.erase(l.begin() + m - 1);
        m--;
        for(int i = 1; i <= m; i++){
            l[m-i] = !(l[m-i] | l[m-i+1]);
            l[m+i] = !(l[m+i] | l[m+i-1]);
        }
        return l;
    };
    auto accumulate_layer = [](vector<long long> l){
        l.insert(l.begin(), 0);
        for(int i = 1; i < (int)l.size(); i++)
            l[i] += l[i-1];
        return l;
    };

    vector<long long> layer[4];
    layer[0] = {Y.rbegin(), Y.rend()};
    layer[0].insert(layer[0].end(), X.begin()+1, X.end());
    layer[1] = next_layer(layer[0]);
    layer[2] = next_layer(layer[1]);
    layer[0] = accumulate_layer(layer[0]);
    layer[1] = accumulate_layer(layer[1]);
    layer[2] = accumulate_layer(layer[2]);
    layer[3] = accumulate_layer(layer[2]);

    auto layer_sum = [&](int i, int l, int r){
        if(r < l) return 0LL;
        int m = (int)layer[i].size() / 2;
        return layer[i][m + r] - layer[i][m + l - 1];
    };
    auto special_layers = [&](int t, int b, int l, int r){
        long long tot = 0;
        for(int k = 0; k < 3; k++){
            if(t <= k && k <= b) tot += layer_sum(k, cell_index(k, max(l, k)), cell_index(k, r));
            if(l <= k && k <= r) tot += layer_sum(k, cell_index(b, k), cell_index(max(t, k), k));
            if(t <= k && k <= b && l <= k && k <= r) tot -= layer_sum(k, 0, 0);
        }
        return tot;
    };
    auto nested_sum = [&](int t, int b, int l, int r){
        if(b < t || r < l) return 0LL;
        return layer_sum(3, cell_index(t, r) - (b - t) + 1, cell_index(t, r) + 1)
             - layer_sum(3, cell_index(t, l) - (b - t),     cell_index(t, l));
    };

    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++){
        auto [t, b, l, r] = tie(T[q], B[q], L[q], R[q]);
        C[q] = special_layers(t, b, l, r) + nested_sum(max(3, t), b, max(3, l), r);
    }
    return C;
}
