#pragma once

#include "LargeFeature.h"

class LargeCaveFeature : public LargeFeature
{
protected:
    void addRoom(int64_t seed, int xOffs, int zOffs, byteArray blocks, double xRoom, double yRoom, double zRoom);
    void addTunnel(int64_t seed, int xOffs, int zOffs, byteArray blocks, double xCave, double yCave, double zCave, float thickness, float yRot, float xRot, int step, int dist, double yScale);
    virtual void addFeature(Level* level, int x, int z, int xOffs, int zOffs, byteArray blocks) override;

private:
    void initTunnelParams(Random& random, int& step, int& dist, bool& singleStep,
        int& splitPoint, bool& steep, float& yRota, float& xRota);

    void updatePositionAndAngles(double& xCave, double& yCave, double& zCave,
        float& xRot, float& yRot,
        float& xRota, float& yRota,
        bool steep, Random& random);

    bool checkBoundaryExit(double xCave, double zCave, double xMid, double zMid,
        int step, int dist, float thickness) const;

    bool isOutsideChunk(double xCave, double zCave, double xMid, double zMid, double rad) const;

    bool handleBranching(Random& random, int64_t originalSeed, int xOffs, int zOffs,
        byteArray blocks, double xCave, double yCave, double zCave,
        float yRot, float xRot, int step, int dist, float thickness);

    bool shouldSkipStep(Random& random) const;

    bool detectWater(byteArray blocks, int x0, int x1, int y0, int y1, int z0, int z1) const;

    void carveBlocks(Level* level, byteArray blocks, double xCave, double yCave, double zCave,
        double rad, double yRad, int xOffs, int zOffs,
        int x0, int x1, int y0, int y1, int z0, int z1);

    void calculateBounds(double xCave, double yCave, double zCave,
        double rad, double yRad, int xOffs, int zOffs,
        int& x0, int& x1, int& y0, int& y1, int& z0, int& z1) const;
};
