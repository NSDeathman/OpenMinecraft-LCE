#pragma once
#include "Layer.h"
#include "PerlinNoise.h"
#include "Biome.h"
#include <vector>

class ClimateLayer : public Layer
{
private:
    PerlinNoise* temperatureNoise;
    PerlinNoise* humidityNoise;
    double scale; // масштаб шума (чем меньше, тем крупнее биомы)

    struct BiomeCenter {
        float temp;
        float hum;
        Biome* biome;
    };
    std::vector<BiomeCenter> centers;

    void initBiomeCenters();           // заполнить центры биомов
    Biome* getBiomeForClimate(float temp, float hum) const;

public:
    ClimateLayer(int64_t seed, int octaves, double scale, shared_ptr<Layer> parent);
    virtual ~ClimateLayer();

    virtual intArray getArea(int xo, int yo, int w, int h) override;
};
