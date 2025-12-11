#pragma once

#include <cmath> 
#include <cstdint>
#include <immintrin.h>

#ifdef _MSC_VER
    #define ALIGN16 __declspec(align(16))
#else
    #define ALIGN16 __attribute__((aligned(16)))
#endif

namespace SHIKA {

   //Define Constant Values & Helpers
   constexpr float PI = 3.1415926535f;
   constexpr float TwoPI = PI * 2.0f;
   constexpr float PiDiv2 = PI / 2.0f;

   constexpr float EPSILON = 1e-5f;

   inline float ToRadian(float deg) {
        return deg * (PI / 180.0f);
    }

    inline float ToDegree(float rad) {
        return rad * (180.0f / PI);
    }

}

inline float DtR()