#pragma once
#include "Tile.h"

class LightSourceTile : public Tile
{
public:
    LightSourceTile(int id);

    // Нет коллизии
    AABB* getAABB(Level* level, int x, int y, int z) override;
    AABB* getTileAABB(Level* level, int x, int y, int z) override;

    // Нет визуала
    bool isSolidRender(bool isServerLevel) override;
    bool isCubeShaped() override;
    int  getRenderShape() override;

    // Не мешает движению
    bool isPathfindable(LevelSource* level, int x, int y, int z) override;
};
