#include <iostream>
#include <cstdio>
#include <cmath> 
#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 

using namespace Shika;

struct Point2D { int x, y; };

inline float EdgeFunction(const Vector3& v0, const Vector3& v1, const Vector3& p) {
    return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

void DrawFilledTriangle(Canvas& canvas, const Vector3& v0, const Vector3& v1, const Vector3& v2, Color color) {
    // Calculate Bounding Box
    int minX = (int)std::floor(std::min({v0.x, v1.x, v2.x}));
    int minY = (int)std::floor(std::min({v0.y, v1.y, v2.y}));
    int maxX = (int)std::ceil(std::max({v0.x, v1.x, v2.x}));
    int maxY = (int)std::ceil(std::max({v0.y, v1.y, v2.y}));

    // Clipping
    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, canvas.GetWidth() - 1);
    maxY = std::min(maxY, canvas.GetHeight() - 1);

    // The Area of the Triangle
    float area = EdgeFunction(v0, v1, v2);

    if (area >= 0) return;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // Miidle point of Pixel (+0.5f)
            Vector3 p((float)x + 0.5f, (float)y + 0.5f, 0.0f);

            // Edge Function Test about 3 sides
            float w0 = EdgeFunction(v1, v2, p); // v1 -> v2 
            float w1 = EdgeFunction(v2, v0, p); // v2 -> v0 
            float w2 = EdgeFunction(v0, v1, p); // v0 -> v1 

            // Put Pixef if pass Edge Test
            if (w0 <= 0 && w1 <= 0 && w2 <= 0) {
                // alpha + beta + gamma = 1
                float alpha = w0 / area;
                float beta  = w1 / area;
                float gamma = w2 / area;

                // Depth Interpolation
                float z = alpha * v0.z + beta * v1.z + gamma * v2.z;

                if (z < canvas.GetDepth(x, y)) {
                    canvas.SetDepth(x, y, z);      // Depth Update
                    canvas.PutPixel(x, y, color);  // Color Update
                }
            }
        }
    }
}

void DrawLine(Canvas& canvas, Point2D p1, Point2D p2, Color color) {
    int x0 = p1.x; int y0 = p1.y;
    int x1 = p2.x; int y1 = p2.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        canvas.PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

int main() {

    const int width = 400;
    const int height = 300;
    Canvas canvas(width, height);

    // --- Triangle ---
    // Red Triangle : z=0.0
    Vector3 tri1[3] = {
        Vector3(0.0f,  0.5f, 0.0f),
        Vector3(0.5f, -0.5f, 0.0f),
        Vector3(-0.5f, -0.5f, 0.0f)
    };

    // Blue Triangle : z=0.5
    Vector3 tri2[3] = {
        Vector3(0.2f,  0.5f, 0.5f),
        Vector3(0.7f, -0.5f, 0.5f),
        Vector3(-0.3f, -0.5f, 0.5f)
    };

    // --- MVP Matrix ---
    Matrix4x4 matWorld = Matrix4x4::RotationY(ToRadian(30.0f)) * Matrix4x4::Translation({0, 0, 4.0f});
    Matrix4x4 matView = Matrix4x4::LookAtLH({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    Matrix4x4 matProj = Matrix4x4::PerspectiveFovLH(ToRadian(60.0f), (float)width/height, 0.1f, 100.0f);

    Matrix4x4 matMVP = matWorld * matView * matProj;

    auto ProcessAndDraw = [&](Vector3* verts, Color c) {
        Point2D screenPts[3];
        float depth[3]; // Save Depth(z) or w value on Screen

        for(int i=0; i<3; ++i) {
            __m128 raw = Matrix4x4::TransformVector(verts[i], matMVP);
            alignas(16) float res[4]; _mm_store_ps(res, raw);
            
            float x = res[0], y = res[1], z = res[2], w = res[3];
            
            if (w != 0.0f) { x /= w; y /= w; z /= w; } 

            screenPts[i].x = (int)((x + 1.0f) * 0.5f * width);
            screenPts[i].y = (int)((1.0f - y) * 0.5f * height);
            depth[i] = z; // Save Depth
        }

        Vector3 p0((float)screenPts[0].x, (float)screenPts[0].y, depth[0]);
        Vector3 p1((float)screenPts[1].x, (float)screenPts[1].y, depth[1]);
        Vector3 p2((float)screenPts[2].x, (float)screenPts[2].y, depth[2]);

        DrawFilledTriangle(canvas, p0, p1, p2, c);
    };

    canvas.ClearDepth();

    ProcessAndDraw(tri1, Color::Red());  // Draw Red First
    ProcessAndDraw(tri2, Color::Blue());

    canvas.SaveToPPM("zbuffer_test.ppm");

    return 0;
}