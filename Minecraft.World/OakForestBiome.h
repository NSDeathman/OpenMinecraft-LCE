#pragma once
#include "Biome.h"

class OakForestBiome : public Biome
{
public:
	OakForestBiome(int id);
	virtual Feature *getTreeFeature(Random *random);
};