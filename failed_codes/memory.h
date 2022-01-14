#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_MEMORY_H
#define PBRT_CORE_MEMORY_H

namespace pbrt {

	// Memory Declarations
#define ARENA_ALLOC(arena, Type) new ((arena).Alloc(sizeof(Type))) Type
	void* AllocAligned(size_t size);
	template <typename T>
	T* AllocAligned(size_t count) {
		return (T*)AllocAligned(count * sizeof(T));
	}
}