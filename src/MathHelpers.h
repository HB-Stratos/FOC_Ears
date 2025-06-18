#pragma once
#include <algorithm> // for std::clamp

namespace utils
{

    // Remap a value from [in_min, in_max] to [out_min, out_max]
    inline float mapf(float value, float in_min, float in_max, float out_min, float out_max)
    {
        return out_min + (value - in_min) * (out_max - out_min) / (in_max - in_min);
    }

    // Clamp a value between lo and hi
    template <typename T>
    inline T clampf(T v, T lo, T hi)
    {
        return std::clamp(v, lo, hi);
    }

    // Combined remap+clamp
    inline float remap_clamped(float v, float in0, float in1, float out0, float out1)
    {
        float m = mapf(v, in0, in1, out0, out1);
        return clampf(m, std::min(out0, out1), std::max(out0, out1));
    }

} // namespace utils
