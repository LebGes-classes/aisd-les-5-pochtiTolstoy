#!/bin/bash

cd "$(dirname "$0")" # Переход в корень проекта

# Автоматическая сборка если нет бинарника
if [ ! -f build/queue_benchmark ]; then
    ./build.sh
fi

# Запуск программы
exec ./build/queue_benchmark
