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
          // --- Basic Constructor ---
          Matrix4x4() {
            row[0] = _mm_setzero_ps();
            row[1] = _mm_setzero_ps();
            row[2] = _mm_setzero_ps();
            row[3] = _mm_setzero_ps();
          } 
       
       public : 
          // --- Static Helper ---
          // Return Identity Matrix
          static Matrix4x4 Identity() {
            Matrix4x4 mat;

            mat.row[0] = _mm_set_ps(0, 0, 0, 1);
            mat.row[1] = _mm_set_ps(0, 0, 1, 0);
            mat.row[2] = _mm_set_ps(0, 1, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);

            return mat;
          }

          // Return Point (Vector3)
          static Vector3 TransformPoint(const Vector3& v, const Matrix4x4& mat) {
            __m128 x = _mm_set1_ps(v.x); // [x, x, x, x]
            __m128 y = _mm_set1_ps(v.y); // [y, y, y, y]
            __m128 z = _mm_set1_ps(v.z); // [z, z, z, z]
            __m128 w = _mm_set1_ps(1.0f); // [1, 1, 1, 1]

            // Linear Combination
            // Result = x*Row0 + y*Row1 + z*Row2 + 1*Row3
            __m128 r = _mm_mul_ps(x, mat.row[0]);
            r = _mm_add_ps(r, _mm_mul_ps(y, mat.row[1]));
            r = _mm_add_ps(r, _mm_mul_ps(z, mat.row[2]));
            r = _mm_add_ps(r, _mm_mul_ps(w, mat.row[3])); 

            return Vector3(r);
        }

        static __m128 TransformVector(const Vector3& v, const Matrix4x4& mat) {
            __m128 x = _mm_set1_ps(v.x);
            __m128 y = _mm_set1_ps(v.y);
            __m128 z = _mm_set1_ps(v.z);
            __m128 w = _mm_set1_ps(1.0f);

            __m128 r = _mm_mul_ps(x, mat.row[0]);
            r = _mm_add_ps(r, _mm_mul_ps(y, mat.row[1]));
            r = _mm_add_ps(r, _mm_mul_ps(z, mat.row[2]));
            r = _mm_add_ps(r, _mm_mul_ps(w, mat.row[3]));

            return r;
        }

        static __m128 TransformDirection(const Vector3& v, const Matrix4x4& mat) {
           __m128 x = _mm_set1_ps(v.x);
           __m128 y = _mm_set1_ps(v.y);
           __m128 z = _mm_set1_ps(v.z);
           // w = 0
    
           __m128 r = _mm_mul_ps(x, mat.row[0]);
           r = _mm_add_ps(r, _mm_mul_ps(y, mat.row[1]));
           r = _mm_add_ps(r, _mm_mul_ps(z, mat.row[2]));
           // Not add row[3] (Translation)
    
           return r;
        }

        // Transpose for Graphics API (Row-Major -> Column-Major)
        Matrix4x4 Transposed() const {
            Matrix4x4 mat;
            
            // Load rows
            __m128 row0 = row[0];
            __m128 row1 = row[1];
            __m128 row2 = row[2];
            __m128 row3 = row[3];

            // Transpose utilizing unpacking instructions
            // Step 1: Interleave low/high parts
            __m128 tmp0 = _mm_unpacklo_ps(row0, row1); // [r0.x, r1.x, r0.y, r1.y]
            __m128 tmp1 = _mm_unpacklo_ps(row2, row3); // [r2.x, r3.x, r2.y, r3.y]
            __m128 tmp2 = _mm_unpackhi_ps(row0, row1); // [r0.z, r1.z, r0.w, r1.w]
            __m128 tmp3 = _mm_unpackhi_ps(row2, row3); // [r2.z, r3.z, r2.w, r3.w]

            // Step 2: Move to correct rows
            mat.row[0] = _mm_movelh_ps(tmp0, tmp1); // [r0.x, r1.x, r2.x, r3.x] -> Col 0
            mat.row[1] = _mm_movehl_ps(tmp1, tmp0); // [r0.y, r1.y, r2.y, r3.y] -> Col 1
            mat.row[2] = _mm_movelh_ps(tmp2, tmp3); // [r0.z, r1.z, r2.z, r3.z] -> Col 2
            mat.row[3] = _mm_movehl_ps(tmp3, tmp2); // [r0.w, r1.w, r2.w, r3.w] -> Col 3

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

          // --- Transformaiton Matrices ---
          // Translation Matrices
          static Matrix4x4 Translation(const Vector3& translation) {
            Matrix4x4 mat = Identity();
            mat.row[3] = _mm_set_ps(1.0f, translation.z, translation.y, translation.x);
            return mat;
          }
          // Scaling Matricies
          static Matrix4x4 Scaling(const Vector3& scaling) {
            Matrix4x4 mat;
            mat.row[0] = _mm_set_ps(0, 0, 0, scaling.x);
            mat.row[1] = _mm_set_ps(0, 0, scaling.y, 0);
            mat.row[2] = _mm_set_ps(0, scaling.z, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around x-axis
          static Matrix4x4 RotationX(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, 0, 0, 1);
            mat.row[1] = _mm_set_ps(0, -s, c, 0);
            mat.row[2] = _mm_set_ps(0, c, s, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around y-axis
          static Matrix4x4 RotationY(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, s, 0, c);
            mat.row[1] = _mm_set_ps(0, 0, 1, 0);
            mat.row[2] = _mm_set_ps(0, c, 0, -s);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around z-axis
          static Matrix4x4 RotationZ(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, 0, -s, c);
            mat.row[1] = _mm_set_ps(0, 0, c, s);
            mat.row[2] = _mm_set_ps(0, 1, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
        
       public :
          // --- View&Projection ---
          // View Matrix (Left-Handed)
          static Matrix4x4 LookAtLH(const Vector3& eye, const Vector3& focus, const Vector3& up) {
            Vector3 f = (focus-eye).Normalized(); // Forward
            Vector3 r = up.Cross(f).Normalized(); // Right
            Vector3 u = f.Cross(r); // Up

            Matrix4x4 mat;
            // First Row (Right)
            mat.row[0] = _mm_set_ps(0, f.x, u.x, r.x);
            // Second Row (Up)
            mat.row[1] = _mm_set_ps(0, f.y, u.y, r.y);
            // Third Row (Forward)
            mat.row[2] = _mm_set_ps(0, f.z, u.z, r.z);
            // Fourth Row (Position)
            float valX = -r.Dot(eye);
            float valY = -u.Dot(eye);
            float valZ = -f.Dot(eye);
            mat.row[3] = _mm_set_ps(1, valZ, valY, valX);

            return mat;
          }
          // Projection Matrix (Left-Handed)
          // range : Z [0, 1] for DirectX
          static Matrix4x4 PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ) {
            Matrix4x4 mat;

            float rad = fovAngleY;
            float h = 1.0f / std::tan(rad / 2.0f); // Cotangent
            float w = h / aspectRatio;
            float q = farZ / (farZ - nearZ);

            mat.row[0] = _mm_set_ps(0, 0, 0, w);
            mat.row[1] = _mm_set_ps(0, 0, h, 0);
            mat.row[2] = _mm_set_ps(1, q, 0, 0);
            mat.row[3] = _mm_set_ps(0, -q * nearZ, 0, 0);

            return mat;
          }

          // Reflection Matrix 
          static Matrix4x4 Reflection(const Vector3& normal, float d){

            Vector3 n = normal.Normalized();
            float x = n.x; float y = n.y; float z = n.z;

            Matrix4x4 mat;

            mat.row[0] = _mm_set_ps(0, -2*x*z, -2*x*y, 1 - 2*x*x);
            mat.row[1] = _mm_set_ps(0, -2*y*z, 1 - 2*y*y, -2*y*x);
            mat.row[2] = _mm_set_ps(0, 1 - 2*z*z, -2*z*y, -2*z*x);
            mat.row[3] = _mm_set_ps(1, -2*d*z, -2*d*y, -2*d*x);

            return mat;
          
        }

       public :       
          // --- View&Projection ---
          // View Matrix (Right-Handed) 
          static Matrix4x4 LookAtRH(const Vector3& eye, const Vector3& focus, const Vector3& up) {
            Vector3 f = (eye-focus).Normalized(); // Forward
            Vector3 r = up.Cross(f).Normalized(); // Right
            Vector3 u = f.Cross(r); // Up

            Matrix4x4 mat;
            // Row-Major
            mat.row[0] = _mm_set_ps(0, f.x, u.x, r.x);
            mat.row[1] = _mm_set_ps(0, f.y, u.y, r.y);
            mat.row[2] = _mm_set_ps(0, f.z, u.z, r.z);
            // Translation
            float valX = -r.Dot(eye);
            float valY = -u.Dot(eye);
            float valZ = -f.Dot(eye);
            mat.row[3] = _mm_set_ps(1, valZ, valY, valX);

            return mat;
          }
          // Projection Matrix (Reft-Handed) + Y-Flip 
          static Matrix4x4 PerspectiveFovRH_Vulkan(float fovAngleY, float aspectRatio, float nearZ, float farZ) {
            Matrix4x4 mat;

            float rad = fovAngleY;
            float h = 1.0f / std::tan(rad / 2.0f); // Cotangent
            float w = h / aspectRatio;
            float q = farZ / (nearZ - farZ);

            mat.row[0] = _mm_set_ps(0, 0, 0, w);
            mat.row[1] = _mm_set_ps(0, 0, -h, 0);
            mat.row[2] = _mm_set_ps(-1, q, 0, 0);
            mat.row[3] = _mm_set_ps(0, q * nearZ, 0, 0);

            return mat;
          }

    };
}