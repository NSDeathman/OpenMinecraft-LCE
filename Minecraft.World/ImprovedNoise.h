#pragma once
#include "Synth.h"

class ImprovedNoise : public Synth
{
    friend class PerlinNoise_SPU;
private:
    int p[512];                // permutation table (size 512 for easy indexing)
    double xo, yo, zo;         // random offsets for each instance

public:
    ImprovedNoise();
    explicit ImprovedNoise(Random* random);
    void init(Random* random);

    double noise(double x, double y, double z) const;
    static double lerp(double t, double a, double b);
    static double grad(int hash, double x, double y, double z);
    static double grad2(int hash, double x, double z); // 2‑D gradient (used in add() for ySize==1)

    // Synth interface
    double getValue(double x, double y) override;          // 2D noise (z = 0)
    double getValue(double x, double y, double z); // 3D noise

    void add(doubleArray buffer, double _x, double _y, double _z,
        int xSize, int ySize, int zSize,
        double xs, double ys, double zs, double pow) const;
};
