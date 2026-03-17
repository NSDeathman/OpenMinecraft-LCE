#include "stdafx.h"
#include "Color.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.h"
#include "Biome.h"
#include "net.minecraft.world.level.biome.h"

std::vector<Biome*> Biome::biomes;

Biome* Biome::ocean = nullptr;
Biome* Biome::plains = nullptr;
Biome* Biome::desert = nullptr;
Biome* Biome::extremeHills = nullptr;
Biome* Biome::forest = nullptr;
Biome* Biome::birchForest = nullptr;
Biome* Biome::oakForest = nullptr;
Biome* Biome::taiga = nullptr;
Biome* Biome::taigaSnowed = nullptr;
Biome* Biome::swampland = nullptr;
Biome* Biome::river = nullptr;
Biome* Biome::hell = nullptr;
Biome* Biome::sky = nullptr;
Biome* Biome::frozenOcean = nullptr;
Biome* Biome::frozenRiver = nullptr;
Biome* Biome::iceFlats = nullptr;
Biome* Biome::iceMountains = nullptr;
Biome* Biome::mushroomIsland = nullptr;
Biome* Biome::mushroomIslandShore = nullptr;
Biome* Biome::beaches = nullptr;
Biome* Biome::desertHills = nullptr;
Biome* Biome::forestHills = nullptr;
Biome* Biome::taigaHills = nullptr;
Biome* Biome::smallerExtremeHills = nullptr;
Biome* Biome::jungle = nullptr;
Biome* Biome::jungleHills = nullptr;

Biome* Biome::getBiome(int id)
{
	if (id >= 0 && id < static_cast<int>(biomes.size()))
		return biomes[id];
	return nullptr;
}

// Регистрация биома по ID (автоматически расширяет вектор при необходимости)
void Biome::registerBiome(int id, Biome* biome)
{
	if (id < 0) return; // защита от отрицательных ID
	if (id >= static_cast<int>(biomes.size()))
	{
		biomes.resize(id + 1, nullptr);
	}
	biomes[id] = biome;
}

int Biome::getBiomeCount()
{
	return static_cast<int>(biomes.size());
}

void Biome::staticCtor()
{
	static int s_biomeId = 0;
#define NEXT_ID (s_biomeId++)

#define COLOURS(suffix) \
        eMinecraftColour_Grass_##suffix, \
        eMinecraftColour_Foliage_##suffix, \
        eMinecraftColour_Water_##suffix, \
        eMinecraftColour_Sky_##suffix

#define BIOME(classType, name, colour, temp, downfall, depth, scale, colourSuffix) \
        Biome::name = (new classType(NEXT_ID)) \
            ->setColor(colour) \
            ->setName(L#name) \
            ->setTemperatureAndDownfall(temp, downfall) \
            ->setDepthAndScale(depth, scale) \
            ->setLeafFoliageWaterSkyColor(COLOURS(colourSuffix))

#define BIOME_NO_RAIN(classType, name, colour, depth, scale, colourSuffix) \
        Biome::name = (new classType(NEXT_ID)) \
            ->setColor(colour) \
            ->setName(L#name) \
            ->setNoRain() \
            ->setTemperatureAndDownfall(2.0f, 0.0f) \
            ->setDepthAndScale(depth, scale) \
            ->setLeafFoliageWaterSkyColor(COLOURS(colourSuffix))

#define BIOME_SNOW(classType, name, colour, temp, downfall, depth, scale, colourSuffix) \
        Biome::name = (new classType(NEXT_ID)) \
            ->setColor(colour) \
            ->setName(L#name) \
            ->setSnowCovered() \
            ->setTemperatureAndDownfall(temp, downfall) \
            ->setDepthAndScale(depth, scale) \
            ->setLeafFoliageWaterSkyColor(COLOURS(colourSuffix))

#define BIOME_LEAF(classType, name, colour, leafColour, temp, downfall, depth, scale, colourSuffix) \
        Biome::name = (new classType(NEXT_ID)) \
            ->setColor(colour) \
            ->setName(L#name) \
            ->setLeafColor(leafColour) \
            ->setTemperatureAndDownfall(temp, downfall) \
            ->setDepthAndScale(depth, scale) \
            ->setLeafFoliageWaterSkyColor(COLOURS(colourSuffix))

	BIOME(OceanBiome, ocean, RGB(0, 0, 112), 0.5f, 0.5f, -1.0f, 0.4f, Ocean);
	BIOME(PlainsBiome, plains, RGB(100, 179, 40), 0.8f, 0.4f, 0.0f, 0.1f, Plains);
	BIOME_NO_RAIN(DesertBiome, desert, RGB(250, 148, 24), 0.1f, 0.2f, Desert);
	BIOME(ExtremeHillsBiome, extremeHills, RGB(96, 96, 96), 0.2f, 0.3f, 0.3f, 1.5f, ExtremeHills);
	BIOME_LEAF(BirchForestBiome, birchForest, RGB(5, 102, 33), RGB(78, 186, 49), 0.7f, 0.8f, 0.0f, 0.1f, Forest);
	BIOME_LEAF(ForestBiome, forest, RGB(5, 102, 33), RGB(78, 186, 49), 0.7f, 0.8f, 0.0f, 0.1f, Forest);
	BIOME_LEAF(OakForestBiome, oakForest, RGB(5, 102, 33), RGB(78, 186, 49), 0.7f, 0.8f, 0.0f, 0.1f, Forest);
	BIOME_LEAF(TaigaBiome, taiga, RGB(11, 102, 89), RGB(78, 186, 49), 0.7f, 0.8f, 0.1f, 0.4f, Taiga);
	BIOME_SNOW(TaigaBiome, taigaSnowed, RGB(11, 102, 89), 0.05f, 0.8f, 0.1f, 0.4f, Taiga);
	BIOME_LEAF(SwampBiome, swampland, RGB(7, 249, 178), RGB(139, 175, 72), 0.8f, 0.9f, -0.2f, 0.1f, Swampland);
	BIOME(RiverBiome, river, RGB(0, 0, 255), 0.5f, 0.5f, -0.5f, 0.0f, River);
	BIOME_NO_RAIN(HellBiome, hell, RGB(255, 0, 0), 0.0f, 0.1f, Hell);
	BIOME_NO_RAIN(TheEndBiome, sky, RGB(128, 128, 255), 0.0f, 0.1f, Sky);
	BIOME_SNOW(OceanBiome, frozenOcean, RGB(144, 144, 160), 0.0f, 0.5f, -1.0f, 0.5f, FrozenOcean);
	BIOME_SNOW(RiverBiome, frozenRiver, RGB(160, 160, 255), 0.0f, 0.5f, -0.5f, 0.0f, FrozenRiver);
	BIOME_SNOW(IceBiome, iceFlats, RGB(255, 255, 255), 0.0f, 0.5f, 0.0f, 0.1f, IcePlains);
	BIOME_SNOW(IceBiome, iceMountains, RGB(160, 160, 160), 0.0f, 0.5f, 0.3f, 1.3f, IceMountains);
	BIOME(MushroomIslandBiome, mushroomIsland, RGB(255, 0, 255), 0.9f, 1.0f, 0.2f, 1.0f, MushroomIsland);
	BIOME(MushroomIslandBiome, mushroomIslandShore, RGB(160, 0, 255), 0.9f, 1.0f, -1.0f, 0.1f, MushroomIslandShore);
	BIOME(BeachBiome, beaches, RGB(250, 222, 85), 0.8f, 0.4f, 0.0f, 0.1f, Beach);
	BIOME_NO_RAIN(DesertBiome, desertHills, RGB(210, 95, 18), 0.3f, 0.8f, DesertHills);
	BIOME_LEAF(ForestBiome, forestHills, RGB(34, 85, 28), RGB(78, 186, 49), 0.7f, 0.8f, 0.3f, 0.7f, ForestHills);
	BIOME_SNOW(TaigaBiome, taigaHills, RGB(22, 57, 51), 0.05f, 0.8f, 0.3f, 0.8f, TaigaHills);
	BIOME(ExtremeHillsBiome, smallerExtremeHills, RGB(114, 120, 154), 0.2f, 0.3f, 0.2f, 0.8f, ExtremeHillsEdge);
	BIOME_LEAF(JungleBiome, jungle, RGB(83, 123, 9), RGB(83, 123, 9), 1.2f, 0.9f, 0.2f, 0.4f, Jungle);
	BIOME_LEAF(JungleBiome, jungleHills, RGB(44, 66, 5), RGB(83, 123, 9), 1.2f, 0.9f, 1.8f, 0.5f, JungleHills);
}

Biome::Biome(int id) : id(id)
{
	color = 0;

	topMaterial = static_cast<byte>(Tile::grass_Id);
	material = static_cast<byte>(Tile::dirt_Id);
	leafColor = 0x4EE031;
	_hasRain = true;
	depth = 0.1f;
	scale = 0.3f;
	temperature = 0.5f;
	downfall = 0.5f;
	decorator = nullptr;

	m_grassColor = eMinecraftColour_NOT_SET;
	m_foliageColor = eMinecraftColour_NOT_SET;
	m_waterColor = eMinecraftColour_NOT_SET;

	registerBiome(id, this);

	decorator = createDecorator();

	friendlies.push_back(new MobSpawnerData(eTYPE_SHEEP, 12, 4, 4));
	friendlies.push_back(new MobSpawnerData(eTYPE_PIG, 10, 4, 4));
	friendlies_chicken.push_back(new MobSpawnerData(eTYPE_CHICKEN, 10, 4, 4)); // 4J - куры в отдельной категории
	friendlies.push_back(new MobSpawnerData(eTYPE_COW, 8, 4, 4));

	enemies.push_back(new MobSpawnerData(eTYPE_SPIDER, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_ZOMBIE, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_SKELETON, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_CREEPER, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_SLIME, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_ENDERMAN, 1, 1, 4));

	waterFriendlies.push_back(new MobSpawnerData(eTYPE_SQUID, 10, 4, 4));

	ambientFriendlies.push_back(new MobSpawnerData(eTYPE_BAT, 10, 8, 8));
}

Biome::~Biome()
{
	if(decorator != nullptr) delete decorator;
}

BiomeDecorator *Biome::createDecorator()
{
	return new BiomeDecorator(this);
}

// 4J Added
Biome *Biome::setLeafFoliageWaterSkyColor(eMinecraftColour grassColor, eMinecraftColour foliageColor, eMinecraftColour waterColour, eMinecraftColour skyColour)
{
	m_grassColor = grassColor;
	m_foliageColor = foliageColor;
	m_waterColor = waterColour;
	m_skyColor = skyColour;
	return this;
}

Biome *Biome::setTemperatureAndDownfall(float temp, float downfall)
{
	temperature = temp;
	this->downfall = downfall;
	return this;
}

Biome *Biome::setDepthAndScale(float depth, float scale)
{
	this->depth = depth;
	this->scale = scale;
	return this;
}

Biome *Biome::setNoRain()
{
	_hasRain = false;
	return this;
}

Feature *Biome::getTreeFeature(Random *random)
{
	if (random->nextInt(10) == 0)
	{
		return new BasicTree(false); // 4J used to return member fancyTree, now returning newly created object so that caller can be consistently resposible for cleanup
	}
	return new TreeFeature(false); // 4J used to return member normalTree, now returning newly created object so that caller can be consistently resposible for cleanup
}

Feature *Biome::getGrassFeature(Random *random)
{
	return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::TALL_GRASS);
}

Biome *Biome::setSnowCovered()
{
	snowCovered = true;
	return this;
}

Biome *Biome::setName(const wstring &name)
{
	this->m_name = name;
	return this;
}

Biome *Biome::setLeafColor(int leafColor)
{
	this->leafColor = leafColor;
	return this;
}

Biome *Biome::setColor(int color)
{
	this->color = color;
	return this;
}

int Biome::getSkyColor(float temp)
{
	return Minecraft::GetInstance()->getColourTable()->getColor( m_skyColor );
}

vector<Biome::MobSpawnerData *> *Biome::getMobs(MobCategory *category)
{
	if (category == MobCategory::monster) return &enemies;
	if (category == MobCategory::creature) return &friendlies;
	if (category == MobCategory::waterCreature) return &waterFriendlies;
	if (category == MobCategory::creature_chicken) return &friendlies_chicken;
	if (category == MobCategory::creature_wolf) return &friendlies_wolf;
	if (category == MobCategory::creature_mushroomcow) return &friendlies_mushroomcow;
	if (category == MobCategory::ambient) return &ambientFriendlies;
	return nullptr;
}

bool Biome::hasSnow()
{
	if( !_hasRain ) return false;

	if( getTemperature() >= 0.15f ) return false;

	return true;
}

bool Biome::hasRain()
{
	if( hasSnow() ) return false;
	return _hasRain;
}

bool Biome::isHumid()
{
	return downfall > .85f;
}

float Biome::getCreatureProbability()
{
	return 0.1f;
}

int Biome::getDownfallInt()
{
	return static_cast<int>(downfall * 65536);
}

int Biome::getTemperatureInt()
{
	return static_cast<int>(temperature * 65536);
}

float Biome::getDownfall()
{
	return downfall;
}

float Biome::getTemperature()
{
	return temperature;
}

void Biome::decorate(Level *level, Random *random, int xo, int zo)
{
	decorator->decorate(level, random, xo, zo);
}

int Biome::getGrassColor()
{
	return Minecraft::GetInstance()->getColourTable()->getColor( m_grassColor );
}

int Biome::getFolageColor()
{
	return Minecraft::GetInstance()->getColourTable()->getColor( m_foliageColor );
}

int Biome::getWaterColor()
{
	return Minecraft::GetInstance()->getColourTable()->getColor( m_waterColor );
}