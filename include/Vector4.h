#pragma once

#include "Common.h"

namespace Shika {

    struct alignas(16) Vector4{
        public:
           union {
           // Anonymous struct
           struct {float x, y, z, w;};
 
           // SIMD register
           __m128 v;

           // for indexing
           float e[4];
           };
           

        public : 
           // 1. Basic Constructor
           Vector4() 
           : v(_mm_setzero_ps()) {}
           // 2. Value Injection Constructor
           Vector4(float _x, float _y, float _z, float _w) 
           : v(_mm_set_ps(_w, _z, _y, _x)) {};
           // 3. SIMD Constructor
           Vector4(__m128 _v)
           : v(_v) {};
           // 4. Value Injection Constructor wiht Vector3
           Vector4(const Vector3& v3, float _w = 1.0f) 
           : v(_mm_set_ps(_w, v3.z, v3.y, v3.x)) {}

        public : 
           // Operator Overloading
           // Add operator
           Vector4 operator+(Vector4 other) const {
               return Vector4( _mm_add_ps(v, other.v) );
           }
           // Sub operator
           Vector4 operator-(Vector4 other) const {
              return Vector4( _mm_sub_ps(v, other.v) );
           }
           // Scalar Multiplication operator (*Scalar)
           Vector4 operator*(float scalar) const {
              __m128 s = _mm_set1_ps(scalar);
              return Vector4( _mm_mul_ps(v, s) );
           }
           // Component-wise Multiplication operator (*Vector4)
           Vector4 operator*(Vector4 other) const {
           return Vector4( _mm_mul_ps(v, other.v) );
           }
 
           // Compound Assignment operator (*=Scalar)
           Vector4& operator*=(float scalar) {
              __m128 s = _mm_set1_ps(scalar);
              v = _mm_mul_ps(v, s);
              return *this;
           }

        public : 
           // Vector Geometry Functions
           // Dot Product
           float Dot(Vector4 other) const {
              __m128 mul = _mm_dp_ps(v, other.v, 0xFF);
              return _mm_cvtss_f32(mul);
           }

           // Length squared
           float LengthSq() const {
              return Dot(*this);
           }

           // Length / Magnitude
           float Length() const {
              return std::sqrt(LengthSq());
           }

           // Normalize
           void Normalize() {
              float len = Length();

              if (len < EPSILON) return;

              *this *= (1.0f / len);
           }
           // Return Normalized Vector4
           Vector4 Normalized() const {
              Vector4 temp = *this;
              temp.Normalize();
              return temp;
           }

           // For Fast Normalize (Precision: ~0.03% error, No Zero Check)
           void NormalizeFast() {
              __m128 dp = _mm_dp_ps(v, v, 0xFF);

              __m128 rsqrt = _mm_rsqrt_ps(dp);

              v = _mm_mul_ps(v, rsqrt);
           }

           Vector4 NormalizedFast() const {
              Vector4 temp = *this;
              temp.NormalizeFast();
              return temp;
           }

    };
}