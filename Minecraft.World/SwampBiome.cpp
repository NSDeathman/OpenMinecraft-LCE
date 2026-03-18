#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.biome.h"
#include "SwampTreeFeature.h"

SwampBiome::SwampBiome(int id) : Biome(id)
{
	decorator->treeCount = 2;
	decorator->flowerCount = -999;
	decorator->blueOrchidCount = 2;
	decorator->deadBushCount = 1;
	decorator->mushroomCount = 8;
	decorator->reedsCount = 10;
	decorator->clayCount = 1;
	decorator->waterlilyCount = 4;
	decorator->grassCount = 3;

	enemies.push_back(new MobSpawnerData(eTYPE_SLIME, 1, 1, 1));
}

Feature *SwampBiome::getTreeFeature(Random *random)
{
	return new SwampTreeFeature();
}
