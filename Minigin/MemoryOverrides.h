#pragma once
// overload global new and delete.

namespace CustomMemoryAllocator
{
	class MemoryAllocator;
}

void* operator new (size_t nbBytes);

void* operator new[] (size_t nbBytes);

void* operator new (size_t nbBytes, CustomMemoryAllocator::MemoryAllocator& storagePool);

void* operator new[] (size_t nbBytes, CustomMemoryAllocator::MemoryAllocator& storagePool);

void operator delete (void* pointerToBuffer);

void operator delete[] (void* pointerToBuffer);

void operator delete (void* pointerToBuffer, CustomMemoryAllocator::MemoryAllocator &storagePool);

