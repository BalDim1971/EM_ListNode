cmake_minimum_required(VERSION 3.10)
project(ListSerializer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/serializer.cpp
    src/deserializer.cpp
    src/file_io.cpp
    src/main.cpp
)

# Заголовочные файлы
set(HEADERS
    include/ListNode.hpp
    include/serializer.hpp
    include/deserializer.hpp
    include/file_io.hpp
)

# Исполняемый файл
add_executable(list_serializer ${SOURCES} ${HEADERS})

# Пути к заголовочным файлам
target_include_directories(list_serializer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Оптимизация
target_compile_options(list_serializer PRIVATE
    $<$<CONFIG:Release>:-O2>
)