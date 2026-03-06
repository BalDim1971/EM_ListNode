#include "file_io.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

ListNode* buildListFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Cannot open input file: " + filename);
    }
    
    struct TempNode {
        ListNode* node;
        int rand_index;
    };
    
    std::vector<TempNode> nodes;
    std::string line;
    
    // 1. Читаем все строки и создаём узлы
    while (std::getline(fin, line)) {
        size_t pos = line.rfind(';');
        if (pos == std::string::npos) continue;
        
        std::string data = line.substr(0, pos);
        std::string rand_idx_str = line.substr(pos + 1);
        
        ListNode* node = new ListNode();
        node->data = data;
        
        TempNode temp;
        temp.node = node;
        temp.rand_index = std::stoi(rand_idx_str);
        
        nodes.push_back(temp);
    }
    
    fin.close();
    
    if (nodes.empty()) {
        return nullptr;
    }
    
    // 2. Связываем prev/next
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i > 0) nodes[i].node->prev = nodes[i - 1].node;
        if (i < nodes.size() - 1) nodes[i].node->next = nodes[i + 1].node;
    }
    
    // 3. Восстанавливаем rand по индексам
    for (const auto& temp : nodes) {
        int idx = temp.rand_index;
        if (idx >= 0 && idx < static_cast<int>(nodes.size())) {
            temp.node->rand = nodes[idx].node;
        }
    }
    
    return nodes[0].node;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}