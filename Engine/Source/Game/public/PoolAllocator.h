#pragma once
#include <cstddef>
#include <vector>
#include <cassert>
#include <new>       

template<typename Base, size_t Capacity,
   size_t ObjectSize = sizeof(Base),
   size_t Alignment = alignof(Base)>

class PoolAllocator
{
private:
   static constexpr size_t BufferAlignment = alignof(Base);
   using BufferSlot = std::aligned_storage_t<ObjectSize, Alignment>;

public:
   PoolAllocator()
   {
      m_FreeList.reserve(Capacity);
      for (size_t i = 0; i < Capacity; ++i)
         m_FreeList.push_back(i);
   }

   template<typename T, typename... Args>
   T* Allocate(Args&&... args)
   {
      static_assert(std::is_base_of<Base, T>::value, "T must derive from Base");
      static_assert(sizeof(T) <= sizeof(BufferSlot), "T is too large for pool");
      static_assert(alignof(T) <= BufferAlignment, "T has stricter alignment than pool");

      if (m_FreeList.empty()) {
         assert(false && "Pool out of memory!");
         return nullptr;
      }

      size_t index = m_FreeList.back();

      if (index >= Capacity) {
         assert(false && "Invalid index in free list!");
         return nullptr;
      }

      m_FreeList.pop_back();

      void* slot = &m_Buffer[index];
      return new(slot) T(std::forward<Args>(args)...);
   }

private:
   std::aligned_storage_t<ObjectSize, Alignment> m_Buffer[Capacity];
   std::vector<size_t> m_FreeList;
};