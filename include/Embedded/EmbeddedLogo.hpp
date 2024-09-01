#ifndef GRAVYENGINE_EMBEDDEDLOGO_HPP
#define GRAVYENGINE_EMBEDDEDLOGO_HPP

#include <cstdint>
#include <cstdlib>

namespace GravyEngine
{
    class EmbeddedLogo
    {
    public:
        const uint8_t *GetData() const;
        size_t GetSize() const;
    };
};

#endif
