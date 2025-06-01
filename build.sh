#!/bin/bash

set -e # Прерывать при ошибках

# Создаем папку сборки
mkdir -p build && cd build

# Конфигурируем CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Компилируем
cmake --build . --parallel $(nproc)

echo "✅ Сборка успешно завершена"
