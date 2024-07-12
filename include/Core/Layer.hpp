#ifndef GRAVYENGINE_LAYER_HPP
#define GRAVYENGINE_LAYER_HPP

namespace GravyEngine
{
    enum Layer
    {
        Layer_None = 0,
        Layer_Default = 1 << 0,
        Layer_IgnoreRaycast = 1 << 1,
        Layer_IgnoreCulling = 1 << 2
    };
};

#endif