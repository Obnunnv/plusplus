#include "memorypool.h"
#include <cstdlib>
#include <iostream>
#include <cstddef>

struct Freeblock {
	size_t size;
	Freeblock* next;
};


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
    Freeblock* prev = nullptr;
	Freeblock* current = freeList;
	
	while (current != nullptr && current->size < size) {
		prev = current;
		current = current->next;
	}
	if (current -- nullptr) {
		return ::operator new(size);
	}
	
	else {
		if (prev) {
			prev->next = current->next;
		}
		else {
			freeList = current->next;
		}
		return reinterpret_cast<void*>(current);
	}
	
	return ::operator new(size);
}

void Memorypool::release(void* ptr) {
    F
	::operator delete(ptr);
}

int main() {

    return 0;
}
