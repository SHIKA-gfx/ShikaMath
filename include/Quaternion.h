#pragma once

#include <cmath>
#include "Common.h"
#include "Vector3.h"
#include "Matrix4x4.h"

namespace Shika {

    struct Quaternion {
        float x, y, z, w;

        public : 
        // 1. Basic Constructor
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        // 2. With Value
        Quaternion(float _x, float _y, float _z, float _w)
            :x(_x), y(_y), z(_z), w(_w) {} 

        // 3. With Axis-Angle
        static Quaternion RotationAxis(const Vector3& axis, float angleRadian) {
            Vector3 n = axis.Normalized();
            float sinHalf = std::sin(angleRadian * 0.5f);
            float cosHalf = std::cos(angleRadian * 0.5f);

            return Quaternion(n.x * sinHalf, n.y * sinHalf, n.z * sinHalf, cosHalf);
        }

        public : 
        // Quaternion Multiplication
        Quaternion operator* (const Quaternion& other) const {
            return Quaternion(
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w,
                w * other.w - x * other.x - y * other.y - z * other.z
            );
        }

        Matrix4x4 ToMatrix() const {
            Matrix4x4 mat;

            float x2 = x + x;  float y2 = y + y;  float z2 = z + z;
            float xx = x * x2; float xy = x * y2; float xz = x * z2;
            float yy = y * y2; float yz = y * z2; float zz = z * z2;
            float wx = w * x2; float wy = w * y2; float wz = w * z2;

            // Row 0
            mat.row[0] = _mm_set_ps(0.0f, xz - wy, xy + wz, 1.0f - (yy + zz));
            // Row 1
            mat.row[1] = _mm_set_ps(0.0f, yz + wx, 1.0f - (xx + zz), xy - wz);
            // Row 2
            mat.row[2] = _mm_set_ps(0.0f, 1.0f - (xx + yy), yz - wx, xz + wy);
            // Row 3 (Identity)
            mat.row[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

            return mat;
        }

    };
}