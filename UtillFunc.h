#pragma once

namespace jh::utill
{

float XM_PI = 3.141592654f;
inline static float Degree2Rad(const float degree)
{
    return degree * XM_PI / 180.0f;
}

inline static float Rad2Degree(const float rad)
{
    return rad * 180.0f / XM_PI;
}

}