#include <FastNoiseLite.h>
#include <numbers>
#include <ranges>

#include "MarchingCubes.h"
#include "ScalarGrid.h"
#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(int seed)
    : m_seed(seed)
{}

Mesh* TerrainGenerator::generateTerrainMesh() const
{
    FastNoiseLite noise;
    noise.SetSeed(200);
    noise.SetFractalOctaves(3);
    noise.SetFrequency(1.0 / 20.0);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);

    constexpr auto rangeX = std::ranges::iota_view{0, 200};
    constexpr auto rangeY = std::ranges::iota_view{0, 200};
    constexpr auto rangeZ = std::ranges::iota_view{0, 100};
    auto rangeZReversed = rangeZ | std::views::reverse;

    ScalarGrid noiseGrid(glm::ivec3{rangeX.size(), rangeY.size(), rangeZ.size()});

    for (int x : rangeX)
    {
        for (int y : rangeY)
        {
            auto noise2D = 0.5f + 0.5f * noise.GetNoise((float) x, (float) y);
            auto surfaceNoise = 5.f * noise2D;
            auto cosX = 0.5f * (cos(x * std::numbers::pi * 2.f / 200.f) + 1.f);
            auto cosY = 0.5f * (cos(y * std::numbers::pi * 2.f / 200.f) + 1.f);
            auto surfaceHeight = 30.f * std::max(cosX, cosY) + 30.f + surfaceNoise;
            auto caveCeil = 40.f - surfaceNoise;

            for (auto z : rangeZ)
            {
                auto caveNoise = 0.5f + 0.5f * noise.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                if (z < (int) std::floor(surfaceHeight) && z < caveCeil)
                {
                    noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                } else if (z == (int) std::floor(surfaceHeight) && z < caveCeil && caveNoise > 0.65f)
                {
                    noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                } else
                {
                    if (noiseGrid.scalar(glm::ivec3{x, y, z - 1}) > 0.67f || noiseGrid.scalar(glm::ivec3{x, y, z - 2}) > 0.67f)
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.66f);
                    else
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.65f + 0.1 * (z - surfaceHeight));
                }
            }
        }
    }

    return MarchingCubes::march(noiseGrid, 0.65);
}
