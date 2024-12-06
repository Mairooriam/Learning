#include <benchmark/benchmark.h>
#include "ResourceManager.hpp"

// Global instance of ResourceManager
ResourceManager& resourceManager = ResourceManager::getInstance();

// Benchmark function for createSprite
static void BM_CreateSprite(benchmark::State& state) {
    sf::Texture test_texture = resourceManager.getErrorTexture();
    for (auto _ : state) {
        sf::Sprite sprite = resourceManager.createSprite(1, test_texture);
        benchmark::DoNotOptimize(sprite);
    }
}
BENCHMARK(BM_CreateSprite);

BENCHMARK_MAIN();