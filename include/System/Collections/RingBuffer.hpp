#ifndef GRAVYENGINE_RINGBUFFER_HPP
#define GRAVYENGINE_RINGBUFFER_HPP

#include <vector>
#include <stdexcept>
#include <cstdlib>

namespace GravyEngine
{
    template <typename T>
    class RingBuffer 
    {
    public:
        RingBuffer() 
            : maxSize(50), startIndex(0), endIndex(0), itemCount(0) 
        {
            items.resize(maxSize);
        }

        RingBuffer(size_t maxMessages) 
            : maxSize(maxMessages), startIndex(0), endIndex(0), itemCount(0) 
        {
            items.resize(maxSize);
        }

        RingBuffer(const RingBuffer &other) 
        {
            items = other.items;
            maxSize = other.maxSize;
            startIndex = other.startIndex;
            endIndex = other.endIndex;
            itemCount = other.itemCount;
        }

        RingBuffer(RingBuffer &&other) noexcept 
        {
            items = std::move(other.items);
            maxSize = other.maxSize;
            startIndex = other.startIndex;
            endIndex = other.endIndex;
            itemCount = other.itemCount;
        }

        RingBuffer& operator=(const RingBuffer &other) 
        {
            if(this != &other) 
            {
                items = other.items;
                maxSize = other.maxSize;
                startIndex = other.startIndex;
                endIndex = other.endIndex;
                itemCount = other.itemCount;
            }
            return *this;
        }

        RingBuffer& operator=(RingBuffer &&other) noexcept 
        {
            if(this != &other) 
            {
                items = std::move(other.items);
                maxSize = other.maxSize;
                startIndex = other.startIndex;
                endIndex = other.endIndex;
                itemCount = other.itemCount;
            }
            return *this;
        }

        void Add(const T &m) 
        {
            items[endIndex] = m;
            endIndex = (endIndex + 1) % maxSize;

            if (itemCount < maxSize)
                itemCount++;
            else
                startIndex = (startIndex + 1) % maxSize;
        }

        size_t GetCount() const 
        {
            return itemCount;
        }

        size_t GetSize() const 
        {
            return maxSize;
        }

        T& GetAt(size_t index) 
        {
            if (index >= 0 && index < itemCount) 
            {
                int idx = (startIndex + index) % maxSize;
                return items[idx];
            }
            throw std::out_of_range("Index out of range");
        }

        void Clear() 
        {
            items.clear();
            items.resize(maxSize);
            startIndex = 0;
            endIndex = 0;
            itemCount = 0;
        }
    private:
        std::vector<T> items;
        size_t maxSize;
        size_t startIndex;
        size_t endIndex;
        size_t itemCount;
    };
};

#endif