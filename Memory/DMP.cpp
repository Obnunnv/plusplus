#include "memorypool.hpp"
#include <cstdlib>
#include <iostream>
#include <cstddef>
#include <cstring>

struct Freeblock {
	size_t size;
	Freeblock* next;
};


Memorypool::Memorypool(size_t size) {
    pool = malloc(size);
    if (!pool) {
        throw std::bad_alloc();
    }
    freeList = nullptr;
    poolSize = size;
    std::cout << poolSize << " bytes created" << std::endl;
}

Memorypool::~Memorypool() {
    free(pool);
    std::cout << poolSize << " bytes destroyed" << std::endl;
}

void* Memorypool::allocate(size_t size) {
    Freeblock* prev = nullptr;
    Freeblock* current = freeList;

    while (current != nullptr && current->size < size) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr) {
        if (prev) {
            prev->next = current->next;
        } else {
            freeList = current->next;
        }

        return reinterpret_cast<char*>(current) + sizeof(Freeblock);
    }

    void* rawBlock = malloc(size + sizeof(size_t));
    if (rawBlock) {
        *((size_t*)rawBlock) = size;
        return (char*)rawBlock + sizeof(size_t);
    }

    return nullptr;
}

size_t Memorypool::getBlocksize(void* ptr) {
    return *((size_t*)((char*)ptr - sizeof(size_t)));
}

void Memorypool::release(void* ptr) {
    Freeblock* block = reinterpret_cast<Freeblock*>(ptr);
	block->next = freeList;
    freeList = block;
}

void* Memorypool::reallocate(void* ptr, size_t newSize) {
    if (ptr == nullptr) {
        return allocate(newSize);

    }
    size_t CurrentSize = getBlocksize(ptr);

    if (newSize <= CurrentSize) {
        return ptr;
    }

    
    void* newptr = allocate(newSize);
    memcpy(newptr, ptr, CurrentSize);
    release(ptr);
    return newptr;
}

int main() {

    return 0;
}
