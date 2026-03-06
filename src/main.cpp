#include <iostream>
#include <stdexcept>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "file_io.hpp"

int main() {
    try {
        // Шаг 1: Читаем inlet.in и строим список
        ListNode* head = buildListFromFile("inlet.in");
        std::cout << "List built from inlet.in" << std::endl;
        
        // Шаг 2: Сериализуем в бинарный файл
        serialize(head, "outlet.out");
        std::cout << "Serialization complete: outlet.out created" << std::endl;
        
        // Шаг 3: Десериализуем для проверки
        ListNode* restored = deserialize("outlet.out");
        std::cout << "Deserialization verified" << std::endl;
        
        // Освобождаем память
        freeList(head);
        freeList(restored);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}