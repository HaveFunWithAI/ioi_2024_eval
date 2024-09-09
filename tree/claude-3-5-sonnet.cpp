#include "tree.h"
#include <vector>
#include <algorithm>
#include <cmath>

const int MAX_N = 200000;
std::vector<int> children[MAX_N];
std::vector<int> weights;
int subtree_size[MAX_N];
long long subtree_weight[MAX_N];

void dfs(int node) {
    subtree_size[node] = 1;
    subtree_weight[node] = weights[node];
    for (int child : children[node]) {
        dfs(child);
        subtree_size[node] += subtree_size[child];
        subtree_weight[node] += subtree_weight[child];
    }
}

void init(std::vector<int> P, std::vector<int> W) {
    int N = W.size();
    weights = W;
    for (int i = 1; i < N; i++) {
        children[P[i]].push_back(i);
    }
    dfs(0);
}

long long query(int L, int R) {
    long long result = 0;
    std::function<void(int, int, int)> solve = [&](int node, int lower, int upper) {
        int subtree_sum = subtree_size[node];
        long long weight_sum = subtree_weight[node];
        
        int min_coef = std::max(L - (subtree_sum - 1), -upper);
        int max_coef = std::min(R, upper - (subtree_sum - 1));
        
        if (min_coef > max_coef) return;
        
        int optimal_coef;
        if (weights[node] == 0) {
            optimal_coef = 0;
        } else if (L <= 0 && 0 <= R) {
            optimal_coef = 0;
        } else {
            optimal_coef = std::min(std::abs(min_coef), std::abs(max_coef));
            if (min_coef <= 0 && 0 <= max_coef) {
                optimal_coef = 0;
            }
        }
        
        result += (long long)std::abs(optimal_coef) * weights[node];
        
        int new_lower = std::max(lower - optimal_coef, L);
        int new_upper = std::min(upper - optimal_coef, R);
        
        for (int child : children[node]) {
            solve(child, new_lower, new_upper);
        }
    };
    
    solve(0, L, R);
    return result;
}