#include <cstddef>
#include <cstdlib>
#include <thread>
#include <mutex>

#pragma once

class Memorypool {
private:
    void* pool;
    size_t poolSize;
    struct Freeblock* freeList = nullptr;
	std::mutex mtx;

public:
    Memorypool(size_t size);
    ~Memorypool();

    void* allocate(size_t size);
    void release(void* ptr);
    void* reallocate(void* ptr, size_t newSize);
    size_t getBlocksize(void* ptr);
	void MergeAdjacentBlocks();
};
