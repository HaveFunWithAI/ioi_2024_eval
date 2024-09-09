#include "nile.h"
#include <vector>
#include <algorithm>
#include <climits>

std::vector<long long> calculate_costs(std::vector<int> W, std::vector<int> A, 
                                       std::vector<int> B, std::vector<int> E) {
    int N = W.size();
    int Q = E.size();
    std::vector<long long> result(Q);

    // Sort artifacts by weight
    std::vector<std::pair<int, int>> artifacts(N);
    for (int i = 0; i < N; i++) {
        artifacts[i] = {W[i], i};
    }
    std::sort(artifacts.begin(), artifacts.end());

    // Pre-compute cumulative sums for B costs
    std::vector<long long> cumB(N + 1, 0);
    for (int i = 0; i < N; i++) {
        cumB[i + 1] = cumB[i] + B[artifacts[i].second];
    }

    for (int q = 0; q < Q; q++) {
        int D = E[q];
        std::vector<long long> dp(N + 1, LLONG_MAX);
        dp[0] = 0;

        for (int i = 0; i < N; i++) {
            // Option 1: Send artifact alone
            dp[i + 1] = std::min(dp[i + 1], dp[i] + A[artifacts[i].second]);

            // Option 2: Try to pair with a previous artifact
            int j = i - 1;
            while (j >= 0 && artifacts[i].first - artifacts[j].first <= D) {
                dp[i + 1] = std::min(dp[i + 1], dp[j] + (cumB[i + 1] - cumB[j]));
                j--;
            }
        }

        result[q] = dp[N];
    }

    return result;
}