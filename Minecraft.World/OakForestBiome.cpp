#include "stdafx.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.h"
#include "OakForestBiome.h"

OakForestBiome::OakForestBiome(int id) : Biome(id)
{
    decorator->treeCount = 10;
    decorator->grassCount = 2;
}

Feature *OakForestBiome::getTreeFeature(Random *random)
{
    if (random->nextInt(10) == 0)
    {
        return new BasicTree(false); // 4J used to return member fancyTree, now returning newly created object so that caller can be consistently resposible for cleanup
    }
    return new TreeFeature(false); // 4J used to return member normalTree, now returning newly created object so that caller can be consistently resposible for cleanup
}