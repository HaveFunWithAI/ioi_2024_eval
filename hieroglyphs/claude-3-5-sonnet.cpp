#include "hieroglyphs.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

std::vector<int> ucs(std::vector<int> A, std::vector<int> B) {
    std::unordered_map<int, std::vector<int>> posA, posB;
    
    // Store positions of each element in A and B
    for (int i = 0; i < A.size(); i++) {
        posA[A[i]].push_back(i);
    }
    for (int i = 0; i < B.size(); i++) {
        posB[B[i]].push_back(i);
    }
    
    std::vector<int> result;
    int lastPosA = -1, lastPosB = -1;
    
    while (true) {
        bool found = false;
        int nextElement = -1;
        int nextPosA = A.size(), nextPosB = B.size();
        
        // Find the next common element that maintains increasing order
        for (const auto& pair : posA) {
            int element = pair.first;
            if (posB.count(element)) {
                auto itA = std::upper_bound(posA[element].begin(), posA[element].end(), lastPosA);
                auto itB = std::upper_bound(posB[element].begin(), posB[element].end(), lastPosB);
                
                if (itA != posA[element].end() && itB != posB[element].end()) {
                    int candidatePosA = *itA;
                    int candidatePosB = *itB;
                    
                    if (candidatePosA < nextPosA || (candidatePosA == nextPosA && candidatePosB < nextPosB)) {
                        nextElement = element;
                        nextPosA = candidatePosA;
                        nextPosB = candidatePosB;
                        found = true;
                    }
                }
            }
        }
        
        if (!found) break;
        
        result.push_back(nextElement);
        lastPosA = nextPosA;
        lastPosB = nextPosB;
    }
    
    // Check if the result is a universal common subsequence
    for (int i = 0; i < result.size(); i++) {
        auto itA = std::find(A.begin() + (i > 0 ? posA[result[i-1]].back() + 1 : 0), A.end(), result[i]);
        auto itB = std::find(B.begin() + (i > 0 ? posB[result[i-1]].back() + 1 : 0), B.end(), result[i]);
        
        if (itA == A.end() || itB == B.end()) {
            return {-1};  // Not a universal common subsequence
        }
    }
    
    return result.empty() ? std::vector<int>{-1} : result;
}