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
    size_t size; //Unsigned integer size member of struct Freeblock
    Freeblock* next; //Pointer to another freeblock creating a linked list of freeblock structures 
};

Memorypool::Memorypool(size_t size) { //Constructor for Memorypool with parameter size
    pool = malloc(size); //Allocates size memory to pool
    if (!pool) { 
        throw std::bad_alloc(); //If the allocation fails std::bad_alloc error is thrown
    }
    freeList = nullptr; //Sets no more freeblocks in the linked list after this by setting freeList to nullptr
    poolSize = size; //Sets poolSize to size for the next line
    std::cout << poolSize << " bytes created" << std::endl; //Prints amount of memory created
}

Memorypool::~Memorypool() { //Destructor for Memorypool
    free(pool); //Fress the memory from pool
    std::cout << poolSize << " bytes freed" << std::endl; //Prints the amount of memory freed
    std::this_thread::sleep_for(std::chrono::seconds(1)); //Sleeps program for 1 seconds
    std::cout << "\033[2J"; //Clears screen
}

void* Memorypool::allocate(size_t size) { //Allocate function from class memorypool with parameter size
    std::lock_guard<std::mutex> lock(mtx); //Uses mutex to lock resources of this thread

    Freeblock* prev = nullptr; //Move the current node on the linked list up and sets the previous one to nullptr
    Freeblock* current = freeList; //Sets current node on linkedlist to current allocated memory

    while (current != nullptr && current->size < size) { //While the current block is not null and the size of the current block isint smaller then the size supposed to be allocated
        prev = current; //Sets the previous block to current
        current = current->next; //Moves the current block to the next
    }

    if (current != nullptr) { //If the current block isint null
        if (prev) { //If previous block exists
            prev->next = current->next; //Moves the node on the linked list up?
        } else {
            freeList = current->next; //Moves the current block up
        }

        return reinterpret_cast<char*>(current) + sizeof(Freeblock); //Pointer magic (arithmetic) to return usable memory
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


        ptr = pool.reallocate(ptr, msize);  // Reallocate with new size 
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
