#pragma once

#include <concepts>
#include <vector>
#include <cstddef>
#include <string>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdint>
#include <iterator>


template<typename T>
concept Serializable = requires(T t, const std::vector<std::byte> data) {
    { t.to_bytes() } -> std::convertible_to<std::vector<std::byte>>;
    { T::from_bytes(data) } -> std::same_as<T>;
};


struct Record {
    std::string key;
    std::string value;
    
    // Сериализация: ключ + значение в вектор байт
    std::vector<std::byte> to_bytes() const {
        std::vector<std::byte> result;
        
        uint32_t key_len = static_cast<uint32_t>(key.size());
        const std::byte* key_len_bytes = reinterpret_cast<const std::byte*>(&key_len);
        result.insert(result.end(), key_len_bytes, key_len_bytes + sizeof(key_len));
        
        const std::byte* key_bytes = reinterpret_cast<const std::byte*>(key.data());
        result.insert(result.end(), key_bytes, key_bytes + key.size());
        
        uint32_t val_len = static_cast<uint32_t>(value.size());
        const std::byte* val_len_bytes = reinterpret_cast<const std::byte*>(&val_len);
        result.insert(result.end(), val_len_bytes, val_len_bytes + sizeof(val_len));
        
        const std::byte* val_bytes = reinterpret_cast<const std::byte*>(value.data());
        result.insert(result.end(), val_bytes, val_bytes + value.size());
        
        return result;
    }
    
    // Десериализация
    static Record from_bytes(const std::vector<std::byte>& data) {
        if (data.size() < 8) {
            throw std::runtime_error("Data too short for Record");
        }
        
        Record record;
        size_t pos = 0;
        
        uint32_t key_len;
        std::memcpy(&key_len, data.data() + pos, sizeof(key_len));
        pos += sizeof(key_len);
        
        record.key.assign(
            reinterpret_cast<const char*>(data.data() + pos),
            key_len
        );
        pos += key_len;
        
        uint32_t val_len;
        std::memcpy(&val_len, data.data() + pos, sizeof(val_len));
        pos += sizeof(val_len);
        
        record.value.assign(
            reinterpret_cast<const char*>(data.data() + pos),
            val_len
        );
        
        return record;
    }
    
    bool operator==(const Record&) const = default;
};


// Сохранение в файл
template<Serializable T>
void save_to_file(const T& obj, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    auto bytes = obj.to_bytes();
    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

// Загрузка из файла
template<Serializable T>
T load_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::vector<char> char_bytes(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    
    std::vector<std::byte> bytes(char_bytes.size());
    std::memcpy(bytes.data(), char_bytes.data(), char_bytes.size());
    
    return T::from_bytes(bytes);
}