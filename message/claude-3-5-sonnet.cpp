#include "message.h"
#include <vector>
#include <algorithm>

const int PACKET_SIZE = 31;
const int BITS_PER_PACKET = 16;

void send_message(std::vector<bool> M, std::vector<bool> C) {
    std::vector<int> safe_indices;
    for (int i = 0; i < PACKET_SIZE; i++) {
        if (C[i] == 0) safe_indices.push_back(i);
    }

    int bits_sent = 0;
    while (bits_sent < M.size()) {
        std::vector<bool> packet(PACKET_SIZE, false);
        for (int i = 0; i < BITS_PER_PACKET && bits_sent < M.size(); i++) {
            packet[safe_indices[i]] = M[bits_sent++];
        }
        send_packet(packet);
    }
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> R) {
    std::vector<bool> M;
    std::vector<int> safe_indices;

    // Find safe indices (those that are consistent across all packets)
    for (int i = 0; i < PACKET_SIZE; i++) {
        bool consistent = true;
        bool value = R[0][i];
        for (int j = 1; j < R.size(); j++) {
            if (R[j][i] != value) {
                consistent = false;
                break;
            }
        }
        if (consistent) safe_indices.push_back(i);
    }

    // Extract the message
    for (const auto& packet : R) {
        for (int i = 0; i < BITS_PER_PACKET && M.size() < 1024; i++) {
            M.push_back(packet[safe_indices[i]]);
        }
    }

    return M;
}