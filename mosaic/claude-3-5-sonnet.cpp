#include "mosaic.h"
#include <vector>

std::vector<long long> mosaic(std::vector<int> X, std::vector<int> Y,
                              std::vector<int> T, std::vector<int> B,
                              std::vector<int> L, std::vector<int> R) {
    int N = X.size();
    int Q = T.size();
    
    // Precompute the entire mosaic
    std::vector<std::vector<int>> grid(N, std::vector<int>(N));
    
    // Fill first row and column
    for (int i = 0; i < N; i++) {
        grid[0][i] = X[i];
        grid[i][0] = Y[i];
    }
    
    // Fill the rest of the grid
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            if (grid[i-1][j] == 0 && grid[i][j-1] == 0) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
    
    // Compute 2D prefix sum
    std::vector<std::vector<long long>> prefix_sum(N+1, std::vector<long long>(N+1, 0));
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            prefix_sum[i][j] = prefix_sum[i-1][j] + prefix_sum[i][j-1] - prefix_sum[i-1][j-1] + grid[i-1][j-1];
        }
    }
    
    // Answer queries
    std::vector<long long> result;
    for (int q = 0; q < Q; q++) {
        int top = T[q], bottom = B[q], left = L[q], right = R[q];
        long long count = prefix_sum[bottom+1][right+1] - prefix_sum[top][right+1] - prefix_sum[bottom+1][left] + prefix_sum[top][left];
        result.push_back(count);
    }
    
    return result;
}