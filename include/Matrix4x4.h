#pragma once
#include "Vector3.h"

namespace Shika {

    struct alignas(16) Matrix4x4 {
       public : 
          union {
            // Two dimensional array m[row][col]
            float m[4][4];
            // for send data
            float e[16];
            // SIMD register
            __m128 row[4];
          };

       public : 
          // 1. Basic Constructor
          Matrix4x4() {
            row[0] = _mm_setzero_ps();
            row[1] = _mm_setzero_ps();
            row[2] = _mm_setzero_ps();
            row[3] = _mm_setzero_ps();
          } 
       
       public : 
          // Static Helper

          // Return Identity Matrix
          static Matrix4x4 Identity() {
            Matrix4x4 mat;

            mat.row[0] = _mm_set_ps(0, 0, 0, 1);
            mat.row[1] = _mm_set_ps(0, 0, 1, 0);
            mat.row[2] = _mm_set_ps(0, 1, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);

            return mat;
          }


    };
}