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

       private : 
          // Helper for Multiplication One Row
          inline __m128 MulRow(__m128 rowVec, const Matrix4x4 other) const{
            // Broadcast x, y, z, w
            __m128 x = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(0, 0, 0, 0));
            __m128 y = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(1, 1, 1, 1));
            __m128 z = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(2, 2, 2, 2));
            __m128 w = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 r = _mm_mul_ps(x, other.row[0]);
            r = _mm_add_ps(r, _mm_mul_ps(y, other.row[1]));
            r = _mm_add_ps(r, _mm_mul_ps(z, other.row[2]));
            r = _mm_add_ps(r, _mm_mul_ps(w, other.row[3]));

            return r;
          }

       public : 
          // Matrix Multiplication Operator
          Matrix4x4 operator*(const Matrix4x4& other) const{
            Matrix4x4 result;
            result.row[0] = MulRow(row[0], other);
            result.row[1] = MulRow(row[1], other);
            result.row[2] = MulRow(row[2], other);
            result.row[3] = MulRow(row[3], other);
            return result;
          }


    };
}