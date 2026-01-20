#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

// Simple Quaternion struct
struct Quaternion {
    float x, y, z, w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {} 
    
};

// ---------------------------------------------------------

// 1. Call by Reference(const)
Quaternion Mul_ByRef (const Quaternion& a, const Quaternion& b) {
            return Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
            );
        }

// 2. Call by Value 
Quaternion Mul_ByVal (Quaternion a, Quaternion b) {
            return Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
            );
        }

// ---------------------------------------------------------

int main() {
    const int ITERATIONS = 100'000'000; 
    
    Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
    
    volatile float prevent_opt = 0.0f; 

    std::cout << "=== Quaternion Passing Benchmark (Iterations: " << ITERATIONS << ") ===\n\n";

    // 1. Reference Test
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        Quaternion result;
        for (int i = 0; i < ITERATIONS; ++i) {
            result = Mul_ByRef(q1, q2); 
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        
        float* f = (float*)&result;
        prevent_opt = f[0]; 

        std::cout << "[1] Const Reference : " << elapsed.count() << " ms" << std::endl;
    }

    // 2. Value Test
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        Quaternion result;
        for (int i = 0; i < ITERATIONS; ++i) {
            result = Mul_ByVal(q1, q2);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        
        float* f = (float*)&result;
        prevent_opt = f[0];

        std::cout << "[2] Call by Value   : " << elapsed.count() << " ms" << std::endl;
    }


    return 0;
}