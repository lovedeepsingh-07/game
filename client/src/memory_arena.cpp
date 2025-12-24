#include "memory_arena.hpp"

void* MemoryArena::alloc(size_t alloc_size, size_t alignment) {
    size_t curr_mem_addr = reinterpret_cast<size_t>(this->m_buffer) + this->m_offset;
    size_t aligned_mem_addr = align_up(curr_mem_addr, alignment);
    size_t new_offset =
        (aligned_mem_addr + alloc_size) - reinterpret_cast<size_t>(this->m_buffer);

    if (new_offset > this->m_capacity) {
        return nullptr;
    }

    void* ptr = this->m_buffer
        + (aligned_mem_addr - reinterpret_cast<size_t>(this->m_buffer));
    this->m_offset = new_offset;
    return ptr;
}
char* MemoryArena::alloc_string(const std::string& input_string) {
    size_t len = strlen(input_string.data()) + 1;
    char* allocated_string = (char*)this->alloc(len);
    memcpy(allocated_string, input_string.data(), len);
    return allocated_string;
}
Clay_String MemoryArena::alloc_clay_string(const std::string& input_string) {
    auto cs = Clay_String{ .length = (std::int32_t)input_string.size(),
                           .chars = this->alloc_string(input_string) };
    return cs;
}
void MemoryArena::clear() {
    this->m_offset = 0;
}
