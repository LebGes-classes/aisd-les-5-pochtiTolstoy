#!/bin/bash

cd "$(dirname "$0")"

# Сборка тестов если необходимо
if [ ! -f build/queue_tests ]; then
    ./build.sh
fi

# Запуск тестов с детализированным выводом
cd build && ctest --output-on-failure
