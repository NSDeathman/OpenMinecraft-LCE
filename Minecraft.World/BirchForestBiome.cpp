#include "stdafx.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.h"
#include "BirchForestBiome.h"

BirchForestBiome::BirchForestBiome(int id) : Biome(id)
{
	decorator->treeCount = 10;
	decorator->grassCount = 2;
}

Feature* BirchForestBiome::getTreeFeature(Random* random)
{
    return new BirchFeature(false);
}
