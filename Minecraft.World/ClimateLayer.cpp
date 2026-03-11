#include "stdafx.h"
#include "ClimateLayer.h"
#include "Random.h"
#include "Mth.h"
#include "IntCache.h"

ClimateLayer::ClimateLayer(int64_t seed, int octaves, double scale)
    : Layer(seed), scale(scale)
{
    Random random(seed);
    temperatureNoise = new PerlinNoise(&random, octaves);
    humidityNoise = new PerlinNoise(&random, octaves);
    initBiomeCenters();
}

ClimateLayer::~ClimateLayer()
{
    delete temperatureNoise;
    delete humidityNoise;
}

void ClimateLayer::initBiomeCenters()
{
    // Задаём центры основных биомов в пространстве (температура, влажность)
    // Значения подобраны на основе setTemperatureAndDownfall() из Biome.cpp
    centers.push_back({ 0.00f, 0.25f, Biome::iceFlats });          // холодные равнины
    centers.push_back({ 0.10f, 0.80f, Biome::taiga });             // тайга
    centers.push_back({ 0.30f, 0.30f, Biome::extremeHills });      // экстремальные холмы
    centers.push_back({ 0.70f, 0.40f, Biome::plains });            // равнины
    centers.push_back({ 0.70f, 0.70f, Biome::forest });            // лес
    centers.push_back({ 0.80f, 0.95f, Biome::swampland });         // болото
    centers.push_back({ 0.95f, 0.05f, Biome::desert });            // пустыня
    centers.push_back({ 0.95f, 0.85f, Biome::jungle });            // джунгли

    // Примечание: океанические биомы не включены — они будут определяться по высоте отдельно.
    // Холмистые варианты (desertHills, forestHills и т.д.) можно добавить позже,
    // используя дополнительный шум высоты или второй проход.
}

Biome* ClimateLayer::getBiomeForClimate(float temp, float hum) const
{
    float bestDist = 1e9f;
    Biome* bestBiome = Biome::plains; // запасной вариант

    for (const auto& c : centers) {
        float dx = temp - c.temp;
        float dy = hum - c.hum;
        float dist = dx * dx + dy * dy;   // квадрат евклидова расстояния
        if (dist < bestDist) {
            bestDist = dist;
            bestBiome = c.biome;
        }
    }
    return bestBiome;
}

intArray ClimateLayer::getArea(int xo, int yo, int w, int h)
{
    IntCache::releaseAll();
    intArray result = IntCache::allocate(w * h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int worldX = xo + x;
            int worldY = yo + y;

            // Получаем значения шума в диапазоне примерно [-1, 1]
            double tempNoise = temperatureNoise->getValue(worldX * scale, worldY * scale);
            double humNoise = humidityNoise->getValue(worldX * scale, worldY * scale);

            // Нормализуем в [0, 1] (логистическая функция или просто clamp)
            float normTemp = Mth::clamp((float)((tempNoise + 1.0) * 0.5), 0.0f, 1.0f);
            float normHum = Mth::clamp((float)((humNoise + 1.0) * 0.5), 0.0f, 1.0f);

            Biome* biome = getBiomeForClimate(normTemp, normHum);
            result[x + y * w] = biome->id;
        }
    }

    return result;
}
