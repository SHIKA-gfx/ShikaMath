#include <iostream>
#include <cstdio>
#include <cmath> 
#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 
#include "../include/Quaternion.h"
#include "../include/Mesh.h"
#include "../include/Rasterizer.h"

using namespace Shika;

int main() {
/*
    const int width = 400;
    const int height = 300;
    Canvas canvas(width, height);

    Mesh cube = Mesh::CreateCube();

    // --- MVP Matrix ---
    Quaternion qX = Quaternion::RotationAxis(Vector3(1, 0, 0), ToRadian(-30));
    Quaternion qY = Quaternion::RotationAxis(Vector3(0, 1, 0), ToRadian(-30));
    Quaternion qFinal = qY * qX;
    Matrix4x4 matRotation = qFinal.ToMatrix();
    
    Matrix4x4 matWorld = matRotation * Matrix4x4::Translation(Vector3(0, 0, 16.0f));
    Matrix4x4 matView = Matrix4x4::LookAtLH({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    Matrix4x4 matProj = Matrix4x4::PerspectiveFovLH(ToRadian(30), (float)width/height, 0.1f, 100.0f);

    Matrix4x4 matMVP = matWorld * matView * matProj;

    // Directional Light
    Vector3 lightDir = Vector3(-1.0f, 1.0f, -1.0f).Normalized();

    // Render Loop
    for (const auto& tri : cube.indices) {

        // 1. Triangles Vertex
        Vector3 p0 = cube.vertices[tri[0]];
        Vector3 p1 = cube.vertices[tri[1]];
        Vector3 p2 = cube.vertices[tri[2]];

        // 2. Calculate FaceNormal
        Vector3 normal = Rasterizer::CalculateFaceNormal(p0, p1, p2);

        // 3. Normal -> World Space
        __m128 normalRotated = Matrix4x4::TransformDirection(normal, matWorld);
        Vector3 worldNormal = Vector3(normalRotated).Normalized();

        // Light Calculates with Lambert's Raw
        float intensity = std::max(0.0f, worldNormal.Dot(lightDir));
        intensity = std::clamp(intensity + 0.1f, 0.0f, 1.0f);

        // With Orange Color (R: 1.0, G: 0.5, B: 0.0)
        Color finalColor = {
            1.0f * intensity, 
            0.5f * intensity, 
            0.0f * intensity
        };

        Vector3 sp0 = Rasterizer::TransformVertex(p0, matMVP, width, height);
        Vector3 sp1 = Rasterizer::TransformVertex(p1, matMVP, width, height);
        Vector3 sp2 = Rasterizer::TransformVertex(p2, matMVP, width, height);

        Rasterizer::DrawFilledTriangle(canvas, sp0, sp1, sp2, finalColor);
    }


    canvas.SaveToPPM("Orange_shaded.cube.ppm");
*/

    printf("\n=== Left - Rigth Handed View Test ===\n");
    Matrix4x4 matLV;
    matLV = Matrix4x4::LookAtLH(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Matrix4x4 matRV;
    matRV = Matrix4x4::LookAtRH(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
    printf("[ View(LH) Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matLV.m[i][j]);
        printf("\n");
    }
    printf("[ View(RH) Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matRV.m[i][j]);
        printf("\n");
    }

    printf("\n=== Left - Rigth Handed Perspective Test ===\n");
    Matrix4x4 matLP;
    matLP = Matrix4x4::PerspectiveFovLH(ToRadian(90.0f), 1.0f, 1.0f, 10.0f);
    Matrix4x4 matRP;
    matRP = Matrix4x4::PerspectiveFovRH_Vulkan(ToRadian(90.0f), 1.0f, 1.0f, 10.0f);
    printf("[ Perspective(LH) Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matLP.m[i][j]);
        printf("\n");
    }
    printf("[ Perspective(RH) Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matRP.m[i][j]);
        printf("\n");
    }

    printf("\n=== Transpose Test ===\n");
    Matrix4x4 matRow;
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j+=2)
            matRow.m[i][j] = 1.11;
        
    Matrix4x4 matCol;
    matCol = matRow.Transposed();
    printf("[ Row-Major Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matRow.m[i][j]);
        printf("\n");
    }
    printf("[ Col-Major Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matCol.m[i][j]);
        printf("\n");
    }

    return 0;
}