#pragma once

#include <clay/clay.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

class MemoryArena {
  private:
    size_t m_capacity;
    std::uint8_t* m_buffer;
    size_t m_offset{ 0 };
    static std::size_t align_up(std::size_t addr, std::size_t alignment) {
        std::size_t mask = alignment - 1;
        return (addr + mask) & ~mask;
    }

  public:
    explicit MemoryArena(size_t capacity)
    : m_capacity(capacity), m_buffer(new std::uint8_t[capacity]) {}
    ~MemoryArena() {
        delete[] this->m_buffer;
    };
    // disable copying and moving
    MemoryArena(const MemoryArena&) = delete;
    MemoryArena& operator=(const MemoryArena&) = delete;
    MemoryArena(MemoryArena&&) = delete;
    MemoryArena& operator=(MemoryArena&&) = delete;

    void* alloc(size_t alloc_size, size_t alignment = alignof(std::max_align_t));
    char* alloc_string(const std::string& input_string);
    Clay_String alloc_clay_string(const std::string& input_string);
    void clear();
};
