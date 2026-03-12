#pragma once
#include "Tile.h"

class LightSourceTile : public Tile
{
public:
    LightSourceTile(int id);

    // Нет визуала
    bool isSolidRender(bool isServerLevel) override;
    bool isCubeShaped() override;
    int  getRenderShape() override;

    // Нет коллизии с сущностями
    AABB* getAABB(Level* level, int x, int y, int z) override;

    // Нет рамки выделения при наведении
    AABB* getTileAABB(Level* level, int x, int y, int z) override;

    // Нельзя навести прицел — луч сквозь блок
    HitResult* clip(Level* level, int x, int y, int z, Vec3* a, Vec3* b) override;

    // Нельзя сломать
    float getDestroyProgress(shared_ptr<Player> player, Level* level, int x, int y, int z) override;

    // Нельзя подобрать средней кнопкой мыши
    bool mayPick() override;

    // Не выбрасывает предметы при удалении
    int getResourceCount(Random* random) override;

    // Не мешает движению
    bool isPathfindable(LevelSource* level, int x, int y, int z) override;
};
