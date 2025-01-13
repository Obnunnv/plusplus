#include <cstddef>

#pragma once

class Memorypool {
private:
    void* pool;
    size_t poolSize;
public:
    Memorypool(size_t size);
    ~Memorypool();

    void* allocate(size_t size);
    void release(void* ptr);
};
