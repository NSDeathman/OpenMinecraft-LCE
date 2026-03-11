#include "stdafx.h"
#include "ImprovedNoise.h"
#include "Random.h"          // наш улучшенный Random
#include <cmath>              // для floor, если нужно, но используем static_cast

ImprovedNoise::ImprovedNoise()
{
    Random random;            // использует новый высокоэнтропийный seed
    init(&random);
}

ImprovedNoise::ImprovedNoise(Random* random)
{
    init(random);
}

void ImprovedNoise::init(Random* random)
{
    // Случайные смещения для каждой координаты (убирают симметрию)
    xo = random->nextDouble() * 256.0;
    yo = random->nextDouble() * 256.0;
    zo = random->nextDouble() * 256.0;

    // Заполняем permutation table числами 0..255 в случайном порядке
    for (int i = 0; i < 256; ++i)
        p[i] = i;

    for (int i = 0; i < 256; ++i)
    {
        int j = random->nextInt(256 - i) + i;   // случайная перестановка
        int tmp = p[i];
        p[i] = p[j];
        p[j] = tmp;

        p[i + 256] = p[i];                      // дублируем для избежания проверок границ
    }
}

// ----------------------------------------------------------------------
// Основной метод вычисления 3D шума Перлина
double ImprovedNoise::noise(double x, double y, double z) const
{
    // Добавляем уникальное смещение для этого экземпляра
    x += xo;
    y += yo;
    z += zo;

    // Целая часть координат (определяем куб, содержащий точку)
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;

    // Относительные координаты внутри куба [0,1)
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // Плавная кривая (fade function): 6t^5 - 15t^4 + 10t^3
    double u = x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
    double v = y * y * y * (y * (y * 6.0 - 15.0) + 10.0);
    double w = z * z * z * (z * (z * 6.0 - 15.0) + 10.0);

    // Хеши индексов восьми углов куба
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Интерполяция
    double y1 = lerp(u,
        grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z));
    double y2 = lerp(u,
        grad(p[AB], x, y - 1, z),
        grad(p[BB], x - 1, y - 1, z));
    double y3 = lerp(u,
        grad(p[AA + 1], x, y, z - 1),
        grad(p[BA + 1], x - 1, y, z - 1));
    double y4 = lerp(u,
        grad(p[AB + 1], x, y - 1, z - 1),
        grad(p[BB + 1], x - 1, y - 1, z - 1));

    double z1 = lerp(v, y1, y2);
    double z2 = lerp(v, y3, y4);
    return lerp(w, z1, z2);
}

double ImprovedNoise::lerp(double t, double a, double b)
{
    return a + t * (b - a);
}

// 3D градиент (используется в noise)
double ImprovedNoise::grad(int hash, double x, double y, double z)
{
    int h = hash & 15;          // младшие 4 бита
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// 2D градиент (используется в add() для случая ySize==1)
double ImprovedNoise::grad2(int hash, double x, double z)
{
    int h = hash & 15;
    double u = (h & 8) ? x : z;   // альтернативное распределение для 2D
    double v = (h < 4) ? 0 : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Synth интерфейс
double ImprovedNoise::getValue(double x, double y)
{
    return noise(x, y, 0.0);
}

double ImprovedNoise::getValue(double x, double y, double z)
{
    return noise(x, y, z);
}

// ----------------------------------------------------------------------
// Метод add – быстрое заполнение буфера значениями шума для прямоугольной области
// (оптимизирован, чтобы избежать повторных вычислений градиентов)
void ImprovedNoise::add(doubleArray buffer, double _x, double _y, double _z,
    int xSize, int ySize, int zSize,
    double xs, double ys, double zs, double pow) const
{
    double invPow = 1.0 / pow;
    int idx = 0;  // индекс в буфере

    if (ySize == 1)
    {
        // Оптимизация для 2D среза (y фиксирован)
        for (int xx = 0; xx < xSize; ++xx)
        {
            double x = _x + xx * xs + xo;
            int X = static_cast<int>(std::floor(x)) & 255;
            double dx = x - std::floor(x);
            double u = dx * dx * dx * (dx * (dx * 6.0 - 15.0) + 10.0);

            for (int zz = 0; zz < zSize; ++zz)
            {
                double z = _z + zz * zs + zo;
                int Z = static_cast<int>(std::floor(z)) & 255;
                double dz = z - std::floor(z);
                double w = dz * dz * dz * (dz * (dz * 6.0 - 15.0) + 10.0);

                int A = p[X];
                int AA = p[A] + Z;
                int B = p[X + 1];
                int BA = p[B] + Z;

                double v0 = lerp(u,
                    grad2(p[AA], dx, dz),
                    grad2(p[BA], dx - 1.0, dz));
                double v2 = lerp(u,
                    grad2(p[AA + 1], dx, dz - 1.0),
                    grad2(p[BA + 1], dx - 1.0, dz - 1.0));

                buffer[idx++] += lerp(w, v0, v2) * invPow;
            }
        }
    }
    else
    {
        // Полная 3D версия с кэшированием по Y
        int yPrev = -1;
        int A, AA, AB, B, BA, BB;
        double vv0, vv1, vv2, vv3;

        for (int xx = 0; xx < xSize; ++xx)
        {
            double x = _x + xx * xs + xo;
            int X = static_cast<int>(std::floor(x)) & 255;
            double dx = x - std::floor(x);
            double u = dx * dx * dx * (dx * (dx * 6.0 - 15.0) + 10.0);

            for (int zz = 0; zz < zSize; ++zz)
            {
                double z = _z + zz * zs + zo;
                int Z = static_cast<int>(std::floor(z)) & 255;
                double dz = z - std::floor(z);
                double w = dz * dz * dz * (dz * (dz * 6.0 - 15.0) + 10.0);

                for (int yy = 0; yy < ySize; ++yy)
                {
                    double y = _y + yy * ys + yo;
                    int Y = static_cast<int>(std::floor(y)) & 255;
                    double dy = y - std::floor(y);
                    double v = dy * dy * dy * (dy * (dy * 6.0 - 15.0) + 10.0);

                    // Кэшируем значения, зависящие только от Y
                    if (yy == 0 || Y != yPrev)
                    {
                        yPrev = Y;
                        A = p[X] + Y;
                        AA = p[A] + Z;
                        AB = p[A + 1] + Z;
                        B = p[X + 1] + Y;
                        BA = p[B] + Z;
                        BB = p[B + 1] + Z;

                        vv0 = lerp(u, grad(p[AA], dx, dy, dz), grad(p[BA], dx - 1.0, dy, dz));
                        vv1 = lerp(u, grad(p[AB], dx, dy - 1.0, dz), grad(p[BB], dx - 1.0, dy - 1.0, dz));
                        vv2 = lerp(u, grad(p[AA + 1], dx, dy, dz - 1.0), grad(p[BA + 1], dx - 1.0, dy, dz - 1.0));
                        vv3 = lerp(u, grad(p[AB + 1], dx, dy - 1.0, dz - 1.0), grad(p[BB + 1], dx - 1.0, dy - 1.0, dz - 1.0));
                    }

                    double v0 = lerp(v, vv0, vv1);
                    double v1 = lerp(v, vv2, vv3);
                    buffer[idx++] += lerp(w, v0, v1) * invPow;
                }
            }
        }
    }
}
