#ifndef MINIAUDIOEX_AUDIOBUFFER_HPP
#define MINIAUDIOEX_AUDIOBUFFER_HPP

#include <cstdlib>
#include <stdexcept>

namespace GravyEngine
{
    template<typename T>
    struct AudioBuffer
    {
    public:
        AudioBuffer(void *pointer, size_t length)
        {
            this->pointer = reinterpret_cast<T*>(pointer);
            this->length = length;
        }

        inline T& operator[](int index) 
        {
            if (index < 0 || index >= length)
                throw std::out_of_range("Index out of range");
            return pointer[index];
        }

        size_t GetLength() const
        {
            return length;
        }

        bool IsEmpty() const
        {
            return length == 0;
        }

        template<typename U>
        inline U *GetPointer() const
        {
            return reinterpret_cast<U*>(pointer);
        }
    private:
        T *pointer;
        size_t length;
    };
};

#endif