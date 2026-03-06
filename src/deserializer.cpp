#include "deserializer.hpp"
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

ListNode* deserialize(const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }
    
    // 1. Читаем количество узлов
    int32_t count;
    fin.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    if (count <= 0) {
        return nullptr;
    }
    
    // 2. Читаем метаданные всех узлов
    std::vector<NodeSerialization> nodeInfos(count);
    fin.read(reinterpret_cast<char*>(nodeInfos.data()), 
             static_cast<std::streamsize>(count * sizeof(NodeSerialization)));
    
    // 3. Создаём вектор узлов
    std::vector<ListNode*> nodes(count);
    for (int32_t i = 0; i < count; ++i) {
        nodes[i] = new ListNode();
    }
    
    // 4. Читаем все строки данных
    for (int32_t i = 0; i < count; ++i) {
        int64_t offset = nodeInfos[i].data_offset;
        int64_t length = 0;
        
        if (i < count - 1) {
            length = nodeInfos[i + 1].data_offset - offset;
        } else {
            fin.seekg(0, std::ios::end);
            int64_t file_end = fin.tellg();
            length = file_end - offset;
        }
        
        fin.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
        nodes[i]->data.resize(static_cast<size_t>(length));
        fin.read(&nodes[i]->data[0], static_cast<std::streamsize>(length));
    }
    
    // 5. Восстанавливаем указатели по индексам
    for (int32_t i = 0; i < count; ++i) {
        if (nodeInfos[i].prev_index != -1) {
            nodes[i]->prev = nodes[nodeInfos[i].prev_index];
        }
        if (nodeInfos[i].next_index != -1) {
            nodes[i]->next = nodes[nodeInfos[i].next_index];
        }
        if (nodeInfos[i].rand_index != -1) {
            nodes[i]->rand = nodes[nodeInfos[i].rand_index];
        }
    }
    
    fin.close();
    return count > 0 ? nodes[0] : nullptr;
}