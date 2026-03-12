#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "LightSourceTile.h"

LightSourceTile::LightSourceTile(int id)
    : Tile(id, Material::air, /*isSolidRender=*/false)
{
    setLightEmission(1.0f);
    setDestroyTime(0.0f);
}

AABB* LightSourceTile::getAABB(Level* level, int x, int y, int z)
{
    return nullptr; // нет физической коллизии
}

AABB* LightSourceTile::getTileAABB(Level* level, int x, int y, int z)
{
    return nullptr; // нет коллизии для выделения
}

bool LightSourceTile::isSolidRender(bool isServerLevel)
{
    return false; // не закрывает соседние грани
}

bool LightSourceTile::isCubeShaped()
{
    return false;
}

int LightSourceTile::getRenderShape()
{
    return Tile::SHAPE_INVISIBLE; // рендер пропускается
}

bool LightSourceTile::isPathfindable(LevelSource* level, int x, int y, int z)
{
    return true; // мобы проходят сквозь блок
}
