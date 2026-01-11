<div align="center">

# ShikaMath

![Language](https://img.shields.io/badge/Language-C++17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
  ![Architecture](https://img.shields.io/badge/Architecture-SIMD%20(SSE)-FF69B4?style=flat-square)
  ![Build](https://img.shields.io/badge/Build-CMake-064F8C?style=flat-square&logo=cmake&logoColor=white)
  ![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=flat-square)

> **A High-Performance,<br>SIMD-Accelerated C++ Math Library for 3D Graphics.** <br>（3Dグラフィックスのための、高性能かつSIMDアクセラレーション対応C++数学ライブラリ）

<br>

<table>
  <tr>
    <td align="center" width="33%">
      <img src="assets/ShikaMath_Triangle.png" width="100%">
      <br>
      <b>1. ワイヤーフレーム</b><br>
      (Wireframe Assembly)<br>
      <sub>MVP行列変換 & ブレゼンハム法</sub>
    </td>
    <td align="center" width="33%">
      <img src="assets/solid_triangle.png" width="100%">
      <br>
      <b>2. ラスタライズ</b><br>
      (Rasterization)<br>
      <sub>重心座標系 (Barycentric Coords)</sub>
    </td>
    <td align="center" width="33%">
      <img src="assets/zbuffer_test.png" width="100%">
      <br>
      <b>3. Zバッファ</b><br>
      (Depth Testing)<br>
      <sub>深度テスト & 隠面消去</sub>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center" width="50%">
      <img src="assets/firstcube.png" width="100%">
      <br>
      <b>4. ライティング処理</b><br>
      (Lighting Pipeline)<br>
      <sub>フラットシェーディング (N·L) & 背面カリング</sub>
    </td>
    <td align="center" width="50%">
      <img src="assets/lastcube.png" width="100%">
      <br>
      <b>5. クォータニオン制御</b><br>
      (Quaternion Dynamics)<br>
      <sub>ジンバルロック回避 & カラー属性補間</sub>
    </td>
  </tr>
</table>
<br>
<i>掲載画像はすべて、ShikaMathの線形代数エンジンを用いたCPUソフトウェアラスタライズによる出力結果です。</i>

</div>

---

## 📖 Overview

**ShikaMath** は、リアルタイム3Dレンダリングパイプラインのためにゼロから設計された、自作の軽量数学ライブラリです。

本ライブラリは、DirectX 12やVulkanといった最新のグラフィックスAPIを導入する前に、生のベクトル演算から複雑なカメラ変換に至るまで、コンピュータグラフィックスの数学的基礎を深く理解するために開発されました。モダンC++で記述されており、低レベルなイントリンシック命令（Intrinsic）による最適化を通じてパフォーマンスを最大化することに重点を置いています。

## ✨ Key Features

ShikaMathは単なる標準数学ヘッダのラッパーではありません。グラフィックス処理のパフォーマンスに特化して設計されています。

## 🚀 SIMD Optimized Core
* 並列浮動小数点演算のために **SSE イントリンシック (__m128)** を活用。
* スカラー実装と比較して、ベクトルの加算、内積、および行列乗算において大幅なパフォーマンス向上を実現。

## 💾 Hardware-Friendly Memory Layout
* `Vector3` および `Matrix4x4` 構造体に対して **16バイトアライメント** (alignas(16)) を強制。
* SIMDレジスタとの互換性を確保し、将来的なGPU頂点バッファへの最適なデータ転送レイアウトを提供。

## 📐 Comprehensive Linear Algebra
* **Vector3:** 外積（Cross Product）、内積（Dot Product）、正規化（Normalization）、および演算子オーバーロードを含む完全な実装。
* **Matrix4x4:** SIMDブロードキャストに最適化された行優先（Row-major）格納。
    * 行の線形結合を用いた高性能な4x4行列乗算を実装。
    * GLSL/HLSLシェーダーとの互換性確保（列優先/Column-Majorへの変換）のための **転置** (Transpose) 操作をサポート。
* **Quaternion:** 3D回転制御のためのロバストなクォータニオン実装。
    * オイラー角特有の **ジンバルロック** (Gimbal Lock) 問題を回避。
    * レンダリングパイプライン向けに `Matrix4x4` へのシームレスな変換を提供。

## 🎥 3D Transformation Pipeline
* 基本的な幾何変換を標準サポート: **平行移動** (Translation)、**拡大縮小** (Scaling)、および **回転** (Rotation)（オイラー角およびクォータニオンに対応）。
* **マルチ座標系サポート (Multi-Coordinate System Support):**
   * 異なるグラフィックスAPI（DirectX と Vulkan）間の仕様差異を吸収するよう設計。
* **左手系システム (DirectX Standard):**
   * **ビュー行列:** `LookAtLH`
   * **透視投影行列:** `PerspectiveFovLH` (Z深度 0.0 ～ 1.0)
* **右手系システム (Vulkan Standard):**
   * **ビュー行列:** `LookAtRH`
   * **透視投影行列:** `PerspectiveFovRH_Vulkan` (Y軸反転補正およびZ深度 0.0 ～ 1.0 に対応)

## 🖼️ Visualization Tools
* 数学的計算結果を即座に視覚検証できる、軽量な `Canvas` クラスと **PPM画像エクスポーター** を同梱。
* ワイヤーフレームレンダリング検証のための基本的なソフトウェアラスタライズ（ブレゼンハムの直線アルゴリズム）を実装。

---

## 🛠️ Tech Stack

* **言語** : Modern C++ (C++17 Standard)
* **最適化** : SIMD Intrinsics (SSE/AVX Ready), 16バイトアライメント
* **数学エンジン** : 独自実装 Vector3 & Matrix4x4 & Quaternion
* **レンダリング** : CPU ソフトウェアレンダリング (ラスタライズ)
* **出力形式** : PPM (Portable Pixel Map)
* **ビルドシステム** : CMake 3.10+ 
* **依存関係** : **None** なし (標準テンプレートライブラリ STL のみ)

## 📦 How to Build
本プロジェクトはビルドシステムとして **CMake** を使用しています。

### Prerequisites
* C++17 compliant compiler (GCC, Clang, or MSVC)
* CMake 3.10 or higher

### Build Steps

```bash
# 1. リポジトリをクローン
git clone https://github.com/your-username/ShikaMath.git
cd ShikaMath

# 2. ビルド用ディレクトリを作成
mkdir build && cd build

# 3. 設定とビルド
cmake ..
make

```
---

## 🗺️ Roadmap

- [x] コア ベクトル/行列演算 (SIMD)

- [x] 基本カメラシステム (ビュー行列/投影行列)

- [x] ソフトウェア ワイヤーフレームレンダリング (PPM出力)

- [x] 重心座標を用いた三角形ラスタライズ (ソリッド塗りつぶし)

- [x] ロバストな回転のためのクォータニオン (四元数) 実装

- [x] Vulkan レンダリングパイプラインとの統合 (Next Goal)

## 📄 License
本プロジェクトは **MIT License** の下で公開されています。
詳細は [LICENSE](LICENSE) ファイルをご確認ください。