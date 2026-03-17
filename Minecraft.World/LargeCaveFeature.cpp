// LargeCaveFeature.cpp
#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.biome.h"
#include "LargeCaveFeature.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

const int BLOCK_ARRAY_SIZE = 16 * 16 * Level::genDepth;

void LargeCaveFeature::addRoom(int64_t seed, int xOffs, int zOffs, byteArray blocks, double xRoom, double yRoom, double zRoom)
{
    addTunnel(seed, xOffs, zOffs, blocks, xRoom, yRoom, zRoom, 1 + random->nextFloat() * 6, 0, 0, -1, -1, 0.5);
}

// ----------------------------------------------------------------------
// Основной метод туннеля
void LargeCaveFeature::addTunnel(int64_t seed, int xOffs, int zOffs, byteArray blocks,
                                  double xCave, double yCave, double zCave,
                                  float thickness, float yRot, float xRot,
                                  int step, int dist, double yScale)
{
    double xMid = xOffs * 16 + 8;
    double zMid = zOffs * 16 + 8;

    Random random(seed);
    float yRota = 0.0f, xRota = 0.0f;
    bool singleStep = false;
    int splitPoint = 0;
    bool steep = false;

    // Инициализация параметров туннеля
    initTunnelParams(random, step, dist, singleStep, splitPoint, steep, yRota, xRota);

    for (; step < dist; step++)
    {
        double rad = 1.5 + (Mth::sin(step * PI / dist) * thickness) * 1.0;
        double yRad = rad * yScale;

        // Обновление позиции и углов
        updatePositionAndAngles(xCave, yCave, zCave, xRot, yRot, xRota, yRota, steep, random);

        // Ветвление в splitPoint
        if (!singleStep && step == splitPoint && thickness > 1.0f && dist > 0)
        {
            if (handleBranching(random, seed, xOffs, zOffs, blocks, xCave, yCave, zCave,
                                yRot, xRot, step, dist, thickness))
                return;
        }

        // Случайный пропуск шага (вероятность 1/4)
        if (!singleStep && shouldSkipStep(random))
            continue;

        // Проверка на выход за пределы допустимой области
        if (checkBoundaryExit(xCave, zCave, xMid, zMid, step, dist, thickness))
            return;

        // Вычисление границ для вырезания
        int x0, x1, y0, y1, z0, z1;
        calculateBounds(xCave, yCave, zCave, rad, yRad, xOffs, zOffs,
                        x0, x1, y0, y1, z0, z1);

        // Проверка наличия воды
        if (detectWater(blocks, x0, x1, y0, y1, z0, z1))
            continue;

        // Вырезание блоков
        carveBlocks(level, blocks, xCave, yCave, zCave, rad, yRad, xOffs, zOffs,
                    x0, x1, y0, y1, z0, z1);

        if (singleStep)
            break;
    }
}

// ----------------------------------------------------------------------
// Инициализация параметров туннеля
void LargeCaveFeature::initTunnelParams(Random& random, int& step, int& dist,
                                         bool& singleStep, int& splitPoint,
                                         bool& steep, float& yRota, float& xRota)
{
    if (dist <= 0)
    {
        int max = radius * 16 - 16;
        dist = max - random.nextInt(max / 4);
    }

    if (step == -1)
    {
        step = dist / 2;
        singleStep = true;
    }
    else
    {
        singleStep = false;
    }

    splitPoint = random.nextInt(dist / 2) + dist / 4;
    // Вероятность крутого туннеля: 1/6
    steep = random.chance(1, 6);
    yRota = 0.0f;
    xRota = 0.0f;
}

// ----------------------------------------------------------------------
// Обновление позиции и углов
void LargeCaveFeature::updatePositionAndAngles(double& xCave, double& yCave, double& zCave,
                                                float& xRot, float& yRot,
                                                float& xRota, float& yRota,
                                                bool steep, Random& random)
{
    float xc = Mth::cos(xRot);
    float xs = Mth::sin(xRot);
    xCave += Mth::cos(yRot) * xc;
    yCave += xs;
    zCave += Mth::sin(yRot) * xc;

    if (steep)
        xRot *= 0.92f;
    else
        xRot *= 0.7f;

    xRot += xRota * 0.1f;
    yRot += yRota * 0.1f;

    xRota *= 0.90f;
    yRota *= 0.75f;
    xRota += (random.nextFloat() - random.nextFloat()) * random.nextFloat() * 2.0f;
    yRota += (random.nextFloat() - random.nextFloat()) * random.nextFloat() * 4.0f;
}

// ----------------------------------------------------------------------
// Проверка на выход за пределы допустимой области (условие return)
bool LargeCaveFeature::checkBoundaryExit(double xCave, double zCave, double xMid, double zMid,
                                          int step, int dist, float thickness) const
{
    double xd = xCave - xMid;
    double zd = zCave - zMid;
    double remaining = dist - step;
    double rr = (thickness + 2.0f) + 24.0; // или 32
    return (xd * xd + zd * zd - remaining * remaining) > rr * rr;
}

// ----------------------------------------------------------------------
// Обработка ветвления в splitPoint
bool LargeCaveFeature::handleBranching(Random& random, int64_t originalSeed,
                                        int xOffs, int zOffs, byteArray blocks,
                                        double xCave, double yCave, double zCave,
                                        float yRot, float xRot, int step, int dist,
                                        float thickness)
{
    // Создаём два ответвления влево и вправо
    addTunnel(random.nextLong(), xOffs, zOffs, blocks, xCave, yCave, zCave,
              random.nextFloat() * 0.5f + 0.5f, yRot - PI / 2, xRot / 3,
              step, dist, 1.0);
    addTunnel(random.nextLong(), xOffs, zOffs, blocks, xCave, yCave, zCave,
              random.nextFloat() * 0.5f + 0.5f, yRot + PI / 2, xRot / 3,
              step, dist, 1.0);
    return true; // завершаем текущий туннель
}

// ----------------------------------------------------------------------
// Случайный пропуск шага
bool LargeCaveFeature::shouldSkipStep(Random& random) const
{
    // Вероятность пропуска: 1/4
    return random.chance(1, 4);
}

// ----------------------------------------------------------------------
// Проверка наличия воды в области
bool LargeCaveFeature::detectWater(byteArray blocks, int x0, int x1, int y0, int y1, int z0, int z1) const
{
    bool detectedWater = false;
    for (int xx = x0; !detectedWater && xx < x1; xx++)
    {
        for (int zz = z0; !detectedWater && zz < z1; zz++)
        {
            for (int yy = y1 + 1; !detectedWater && yy >= y0 - 1; yy--)
            {
                int p = (xx * 16 + zz) * Level::genDepth + yy;
                if (yy < 0 || yy >= Level::genDepth || p < 0 || p >= BLOCK_ARRAY_SIZE) continue;
                if (blocks[p] == Tile::water_Id || blocks[p] == Tile::calmWater_Id)
                {
                    detectedWater = true;
                }
                // Оптимизация: если мы не на границе, пропускаем внутренние блоки
                if (yy != y0 - 1 && xx != x0 && xx != x1 - 1 && zz != z0 && zz != z1 - 1)
                {
                    yy = y0;
                }
            }
        }
    }
    return detectedWater;
}

// ----------------------------------------------------------------------
// Вычисление границ области для вырезания
void LargeCaveFeature::calculateBounds(double xCave, double yCave, double zCave,
                                        double rad, double yRad, int xOffs, int zOffs,
                                        int& x0, int& x1, int& y0, int& y1,
                                        int& z0, int& z1) const
{
    x0 = Mth::floor(xCave - rad) - xOffs * 16 - 1;
    x1 = Mth::floor(xCave + rad) - xOffs * 16 + 1;
    y0 = Mth::floor(yCave - yRad) - 1;
    y1 = Mth::floor(yCave + yRad) + 1;
    z0 = Mth::floor(zCave - rad) - zOffs * 16 - 1;
    z1 = Mth::floor(zCave + rad) - zOffs * 16 + 1;

    // Обрезаем по границам чанка и мира
    if (x0 < 0) x0 = 0;
    if (x1 > 16) x1 = 16;
    if (y0 < 1) y0 = 1;
    if (y1 > Level::genDepth - 8) y1 = Level::genDepth - 8;
    if (z0 < 0) z0 = 0;
    if (z1 > 16) z1 = 16;
}

// ----------------------------------------------------------------------
// Вырезание блоков в области
void LargeCaveFeature::carveBlocks(Level* level, byteArray blocks,
                                    double xCave, double yCave, double zCave,
                                    double rad, double yRad, int xOffs, int zOffs,
                                    int x0, int x1, int y0, int y1, int z0, int z1)
{
    for (int xx = x0; xx < x1; xx++)
    {
        double xd = ((xx + xOffs * 16 + 0.5) - xCave) / rad;
        for (int zz = z0; zz < z1; zz++)
        {
            double zd = ((zz + zOffs * 16 + 0.5) - zCave) / rad;
            if (xd * xd + zd * zd < 1.0)
            {
                int p = (xx * 16 + zz) * Level::genDepth + y1;

                // Проверяем начальный индекс
                if (p < 0 || p >= BLOCK_ARRAY_SIZE) continue;

                bool hasGrass = false;
                for (int yy = y1 - 1; yy >= y0; yy--)
                {
                    if (p < 0 || p >= BLOCK_ARRAY_SIZE)
                    {
                        p--;
                        continue;
                    }

                    double yd = (yy + 0.5 - yCave) / yRad;
                    if (yd > -0.7 && xd * xd + yd * yd + zd * zd < 1.0)
                    {
                        int block = blocks[p];
                        if (block == Tile::grass_Id)
                            hasGrass = true;
                        if (block == Tile::stone_Id || block == Tile::dirt_Id || block == Tile::grass_Id)
                        {
                            if (yy < 10)
                            {
                                blocks[p] = static_cast<byte>(Tile::lava_Id);
                            }
                            else
                            {
                                blocks[p] = static_cast<byte>(0);
                                int below = p - Level::genDepth;
                                if (hasGrass && below >= 0 && below < BLOCK_ARRAY_SIZE && blocks[below] == Tile::dirt_Id)
                                {
                                    blocks[p - Level::genDepth] = (byte) level->getBiome(xx + xOffs * 16, zz + zOffs * 16)->topMaterial;
                                }
                            }
                        }
                    }
                    p --;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------
// addFeature – генерация пещер в чанке
void LargeCaveFeature::addFeature(Level* level, int x, int z, int xOffs, int zOffs, byteArray blocks)
{
    // Оригинальное условие: if (random->nextInt(15) != 0) caves = 0;
    // Заменяем на chance(14,15): с вероятностью 14/15 выходим (не генерируем пещеры)
    if (random->chance(14, 15))
        return;

    // Количество пещер в чанке: сложное выражение
    int caves = random->nextInt(random->nextInt(random->nextInt(40) + 1) + 1);
    for (int cave = 0; cave < caves; cave++)
    {
        double xCave = x * 16 + random->nextInt(16);
        double yCave = random->nextInt(random->nextInt(Level::genDepth - 8) + 8);
        double zCave = z * 16 + random->nextInt(16);

        int tunnels = 1;
        // Шанс комнаты 1/3 (было 1/4)
        if (random->chance(1, 3))
        {
            addRoom(random->nextLong(), xOffs, zOffs, blocks, xCave, yCave, zCave);
            // Количество дополнительных туннелей от комнаты: 1-5
            tunnels += random->nextInt(5) + 1; // 1-5 дополнительных, итого 2-6 туннелей
        }

        for (int i = 0; i < tunnels; i++)
        {
            float yRot = random->nextFloat() * PI * 2;
            float xRot = ((random->nextFloat() - 0.5f) * 2) / 8;

            float thickness = 1.25f + random->nextFloat();
            // Редкие огромные залы: шанс 1/10
            if (random->chance(1, 10))
                thickness *= 1.25f;

            addTunnel(random->nextLong(), xOffs, zOffs, blocks,
                xCave, yCave, zCave,
                thickness, yRot, xRot, 0, 0, 1.0);
        }
    }
}