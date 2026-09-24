#include "owning_buffer.h"
#include <algorithm>

OwningBuffer::OwningBuffer(std::size_t s) 
    : data(new char[s]), size(s) {}

OwningBuffer::~OwningBuffer() {
    delete[] data;
}

OwningBuffer::OwningBuffer(const OwningBuffer& other) 
    : data(new char[other.size]), size(other.size) {
    std::copy(other.data, other.data + other.size, data);
}

OwningBuffer::OwningBuffer(OwningBuffer&& other) noexcept 
    : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

OwningBuffer& OwningBuffer::operator=(const OwningBuffer& other) {
    OwningBuffer tmp(other);
    swap(tmp);
    return *this;
}

OwningBuffer& OwningBuffer::operator=(OwningBuffer&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}

void OwningBuffer::swap(OwningBuffer& b) noexcept {
    std::swap(data, b.data);
    std::swap(size, b.size);
}