#include "stdafx.h"
#include "PerlinNoise.h"
#include "Random.h"
#include "Mth.h"

PerlinNoise::PerlinNoise(int levels)
{
    Random random;          // использует новый высокоэнтропийный seed
    init(&random, levels);
}

PerlinNoise::PerlinNoise(Random* random, int levels)
{
    init(random, levels);
}

void PerlinNoise::init(Random* random, int levels)
{
    MemSect(2);
    this->levels = levels;
    noiseLevels = new ImprovedNoise * [levels];
    for (int i = 0; i < levels; ++i)
    {
        noiseLevels[i] = new ImprovedNoise(random);
    }
    MemSect(0);
}

PerlinNoise::~PerlinNoise()
{
    for (int i = 0; i < levels; ++i)
        delete noiseLevels[i];
    delete[] noiseLevels;
}

double PerlinNoise::getValue(double x, double y)
{
    double value = 0.0;
    double amp = 1.0;          // амплитуда (здесь 1/pow в оригинале)
    for (int i = 0; i < levels; ++i)
    {
        value += noiseLevels[i]->getValue(x * amp, y * amp) / amp;
        amp *= 0.5;             // уменьшаем частоту (pow /= 2)
    }
    return value;
}

double PerlinNoise::getValue(double x, double y, double z)
{
    double value = 0.0;
    double amp = 1.0;
    for (int i = 0; i < levels; ++i)
    {
        value += noiseLevels[i]->getValue(x * amp, y * amp, z * amp) / amp;
        amp *= 0.5;
    }
    return value;
}

doubleArray PerlinNoise::getRegion(doubleArray buffer,
    int x, int y, int z,
    int xSize, int ySize, int zSize,
    double xScale, double yScale, double zScale)
{
    // Инициализация буфера нулями, если необходимо
    if (buffer.data == nullptr)
        buffer = doubleArray(xSize * ySize * zSize);
    else
        for (unsigned int i = 0; i < buffer.length; ++i)
            buffer[i] = 0.0;

    double freq = 1.0;   // частота (pow)
    for (int i = 0; i < levels; ++i)
    {
        // Вызов add для текущего уровня с правильным масштабированием
        noiseLevels[i]->add(buffer,
            x * freq * xScale,
            y * freq * yScale,
            z * freq * zScale,
            xSize, ySize, zSize,
            xScale * freq,
            yScale * freq,
            zScale * freq,
            freq);
        freq *= 0.5;
    }

    return buffer;
}

// Перегрузка для 2D региона (y фиксирован)
doubleArray PerlinNoise::getRegion(doubleArray sr, int x, int z,
    int xSize, int zSize,
    double xScale, double zScale, double pow)
{
    // Используем y = 10 (как в оригинале), ySize = 1
    return getRegion(sr, x, 10, z, xSize, 1, zSize, xScale, 1.0, zScale);
}
