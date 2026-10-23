# ProjectA

cat > README.md << 'EOF'
# KVS (Key-Value Store)

In-memory key-value хранилище с сетевым доступом (учебный проект практикума).

## Сборка и запуск

Проект использует CMake и C++23.

```bash
# Очистка и конфигурация сборки
rm -rf build
cmake -B build -GNinja

# Компиляция
ninja -C build

# Запуск консольного клиента
./build/kvs_cli

# Запуск тестов
cd build && ctest --output-on-failure