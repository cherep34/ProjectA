#pragma once
#include <cstddef>
#include <utility>

class OwningBuffer {
public:
    explicit OwningBuffer(std::size_t s);
    ~OwningBuffer();
    
    OwningBuffer(const OwningBuffer& other);
    OwningBuffer(OwningBuffer&& other) noexcept;

    OwningBuffer& operator=(const OwningBuffer& other);
    OwningBuffer& operator=(OwningBuffer&& other) noexcept;

    char* get_data() { return data; }
    std::size_t get_size() const { return size; }

private:
    char* data;
    std::size_t size;
    void swap(OwningBuffer& b) noexcept;
};