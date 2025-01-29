#include "memorypool.hpp"
#include <cstdlib> //For Malloc and Free
#include <iostream> //For std::cout and std::cerr
#include <cstddef> //For size_t
#include <string> //For std::string and std::transform()
#include <thread> //For std::this_thread::sleep_for()
#include <mutex> //For std::lock_guard<std::mutex> lock(mtx)
#include <chrono> //For std::chrono::steady_clock::now(), std::chrono::seconds(), and std::chrono::milliseconds()
#include <cstring> //For std::memmove
#include <algorithm> //For std::transform
#include <cctype> //For ::toupper()

//./dmp Function Amount (bytes) Time (seconds)

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
    std::cout << poolSize << " bytes freed" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\033[2J";
}

void* Memorypool::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(mtx);

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
    std::lock_guard<std::mutex> lock(mtx);

    Freeblock* block = reinterpret_cast<Freeblock*>(ptr);
    block->next = freeList;
    freeList = block;
}

void* Memorypool::reallocate(void* ptr, size_t newSize) {
    std::lock_guard<std::mutex> lock(mtx);

    if (ptr == nullptr) {
        return allocate(newSize);
    }

    size_t currentSize = getBlocksize(ptr);

    if (newSize <= currentSize) {
        return ptr;
    }

    void* newPtr = allocate(newSize);
    std::memmove(newPtr, ptr, currentSize);

    release(ptr);

    return newPtr;
}

void Memorypool::MergeAdjacentBlocks() {
    Freeblock* current = freeList;

    while (current != nullptr && current->next != nullptr) {
        char* currentEnd = reinterpret_cast<char*>(current) + sizeof(Freeblock) + current->size;
        char* nextStart = reinterpret_cast<char*>(current->next);

        if (currentEnd == nextStart) {
            current->size += sizeof(Freeblock) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Missing Command Line arguments" << "\n";
        std::cerr << "./dmp function size (bytes) time (seconds)" << "\n";
        return 1;
    }

    Memorypool pool(1024 * 1024);

    std::string function = argv[1];
    std::transform(function.begin(), function.end(), function.begin(), ::toupper);
    int msize = std::stoi(argv[2]);
    int time = std::stoi(argv[3]);

    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds(time);

    void* ptr = nullptr;

    if (function == "ALLOCATE") {
        ptr = pool.allocate(msize);
        std::cout << "Allocated " << msize << " bytes" << "\n";
    }
    else if (function == "REALLOCATE") {
        ptr = pool.allocate(msize);
        std::cout << "Allocated " << msize << " bytes" << "\n";


        while (std::chrono::steady_clock::now() < end) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }


        ptr = pool.reallocate(ptr, msize);  // Reallocate with new size (doubling the size as an example)
        std::cout << "Reallocated to " << msize << " bytes" << "\n";
    }


    while (std::chrono::steady_clock::now() < end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    if (ptr != nullptr) {
        pool.release(ptr);
        std::cerr << "Time up Memory freed" << std::endl;
    }


    return 0;
}
