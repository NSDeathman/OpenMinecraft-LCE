#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "LightSourceTile.h"

LightSourceTile::LightSourceTile(int id)
    : Tile(id, Material::decoration, /*isSolidRender=*/false)
{
    setLightEmission(1.0f);
    setDestroyTime(0.0f);
}

bool LightSourceTile::isSolidRender(bool isServerLevel)
{
    return false;
}

bool LightSourceTile::isCubeShaped()
{
    return false;
}

int LightSourceTile::getRenderShape()
{
    return Tile::SHAPE_INVISIBLE;
}

AABB* LightSourceTile::getAABB(Level* level, int x, int y, int z)
{
    return nullptr; // нет физической коллизии
}

AABB* LightSourceTile::getTileAABB(Level* level, int x, int y, int z)
{
    return nullptr; // нет рамки выделени€
}

HitResult* LightSourceTile::clip(Level* level, int x, int y, int z, Vec3* a, Vec3* b)
{
    return nullptr; // луч проходит сквозь блок, прицел не реагирует
}

float LightSourceTile::getDestroyProgress(shared_ptr<Player> player, Level* level, int x, int y, int z)
{
    return 0.0f; // прогресс разрушени€ всегда 0 Ч сломать невозможно
}

bool LightSourceTile::mayPick()
{
    return false; // нельз€ подобрать средней кнопкой
}

int LightSourceTile::getResourceCount(Random* random)
{
    return 0; // не выбрасывает ничего
}

bool LightSourceTile::isPathfindable(LevelSource* level, int x, int y, int z)
{
    return true; // мобы проход€т насквозь
}
