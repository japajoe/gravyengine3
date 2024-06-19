#ifndef GRAVYENGINE_EVENTHANDLER_HPP
#define GRAVYENGINE_EVENTHANDLER_HPP

#include <vector>
#include <functional>

namespace GravyEngine
{
    template<typename T>
    class EventHandler
    {
    public:
        std::vector<T> callbacks;

        template<typename ... Param>
        void operator () (Param ... param)
        {
            for(size_t i = 0; i < callbacks.size(); i++)
            {
                if(callbacks[i] != nullptr)
                {
                    callbacks[i](param...);
                }
            }
        }

        void operator += (T callback)
        {
            callbacks.push_back(callback);
        }

        bool operator != (void* ptr)
        {
            if(callbacks.size() > 0)
                return true;
            return false;
        }
    };
};

#endif