#include <cstddef> //For size_t
#include <cstdlib>
#include <thread>
#include <mutex> //For std::mutex mtx

#pragma once //Pre proccesor Directive ensuring this file in only included once in the main file

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
