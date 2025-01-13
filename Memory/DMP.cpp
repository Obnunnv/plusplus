#include "memorypool.h"
#include <cstdlib>
#include <iostream>
#include <cstddef>

Memorypool::Memorypool(size_t size) {
    pool = ::operator new(size);
    if (!pool) {
        throw std::bad_alloc();
    }

    poolSize = size;
    std::cout << poolSize << " bytes created" << std::endl;
}

Memorypool::~Memorypool() {
    ::operator delete(pool);
    std::cout << poolSize << " bytes destroyed" << std::endl;
}

void* Memorypool::allocate(size_t size) {
    return ::operator new(size);
}

void Memorypool::release(void* ptr) {
    ::operator delete(ptr);
}

int main() {
    Memorypool pool(1024);

    void* ptr1 = pool.allocate(100);
    void* ptr2 = pool.allocate(200);

    pool.release(ptr1);
    pool.release(ptr2);

    return 0;
}
