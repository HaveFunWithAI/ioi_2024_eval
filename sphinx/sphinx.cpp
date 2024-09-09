#include "sphinx.h"
#include <vector>
#include <algorithm>

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) {
    std::vector<int> colors(N, -1);
    std::vector<std::vector<int>> adj(N);
    
    // Build adjacency list
    for (int i = 0; i < X.size(); i++) {
        adj[X[i]].push_back(Y[i]);
        adj[Y[i]].push_back(X[i]);
    }

    // Helper function to count components
    auto count_components = [&](const std::vector<int>& coloring) {
        return perform_experiment(coloring);
    };

    // Find the first color
    int first_color = -1;
    for (int c = 0; c < N; c++) {
        std::vector<int> test(N, c);
        if (count_components(test) == 1) {
            first_color = c;
            break;
        }
    }

    colors[0] = first_color;

    // DFS to color the graph
    std::function<void(int)> dfs = [&](int v) {
        for (int u : adj[v]) {
            if (colors[u] == -1) {
                std::vector<int> test = colors;
                test[u] = colors[v];
                if (count_components(test) == count_components(colors)) {
                    colors[u] = colors[v];
                } else {
                    for (int c = 0; c < N; c++) {
                        if (c != colors[v]) {
                            test[u] = c;
                            if (count_components(test) == count_components(colors) + 1) {
                                colors[u] = c;
                                break;
                            }
                        }
                    }
                }
                dfs(u);
            }
        }
    };

    dfs(0);

    return colors;
}