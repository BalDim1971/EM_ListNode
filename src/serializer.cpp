#include "serializer.hpp"
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

struct NodeSerialization {
    int64_t data_offset;
    int32_t rand_index;
    int32_t prev_index;
    int32_t next_index;
};

void serialize(ListNode* head, const std::string& filename) {
    // 1. Собираем все узлы в вектор
    std::vector<ListNode*> nodes;
    for (ListNode* p = head; p != nullptr; p = p->next) {
        nodes.push_back(p);
    }
    
    std::ofstream fout(filename, std::ios::binary);
    if (!fout) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    // 2. Записываем количество узлов
    int32_t count = static_cast<int32_t>(nodes.size());
    fout.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    // 3. Вычисляем смещение первой строки
    int64_t first_string_offset = sizeof(int32_t) + 
                                   static_cast<int64_t>(nodes.size()) * sizeof(NodeSerialization);
    
    // 4. Создаём вектор метаданных узлов
    std::vector<NodeSerialization> nodeInfos(nodes.size());
    
    // 5. Заполняем метаданные
    for (size_t i = 0; i < nodes.size(); ++i) {
        ListNode* node = nodes[i];
        
        nodeInfos[i].data_offset = first_string_offset;
        
        // Индекс rand
        if (node->rand == nullptr) {
            nodeInfos[i].rand_index = -1;
        } else {
            int32_t rand_idx = -1;
            for (size_t j = 0; j < nodes.size(); ++j) {
                if (nodes[j] == node->rand) {
                    rand_idx = static_cast<int32_t>(j);
                    break;
                }
            }
            nodeInfos[i].rand_index = rand_idx;
        }
        
        // Индекс prev
        if (node->prev == nullptr) {
            nodeInfos[i].prev_index = -1;
        } else {
            for (size_t j = 0; j < nodes.size(); ++j) {
                if (nodes[j] == node->prev) {
                    nodeInfos[i].prev_index = static_cast<int32_t>(j);
                    break;
                }
            }
        }
        
        // Индекс next
        if (node->next == nullptr) {
            nodeInfos[i].next_index = -1;
        } else {
            for (size_t j = 0; j < nodes.size(); ++j) {
                if (nodes[j] == node->next) {
                    nodeInfos[i].next_index = static_cast<int32_t>(j);
                    break;
                }
            }
        }
        
        // Обновляем смещение для следующей строки
        first_string_offset += static_cast<int64_t>(node->data.size());
    }
    
    // 6. Записываем метаданные всех узлов
    for (const auto& info : nodeInfos) {
        fout.write(reinterpret_cast<const char*>(&info), sizeof(info));
    }
    
    // 7. Записываем все строки данных
    for (const auto* node : nodes) {
        fout.write(node->data.data(), static_cast<std::streamsize>(node->data.size()));
    }
    
    fout.close();
}