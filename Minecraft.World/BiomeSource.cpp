#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "System.h"
#include "BiomeSource.h"
#include "ClimateLayer.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\ProgressRenderer.h"

void BiomeSource::_init()
{
	layer = nullptr;
	zoomedLayer = nullptr;

	cache = new BiomeCache(this);

	playerSpawnBiomes.push_back(Biome::plains);
	playerSpawnBiomes.push_back(Biome::forest);
	playerSpawnBiomes.push_back(Biome::forestHills);
	playerSpawnBiomes.push_back(Biome::taiga);
	playerSpawnBiomes.push_back(Biome::taigaHills);
	playerSpawnBiomes.push_back(Biome::jungle);
	playerSpawnBiomes.push_back(Biome::jungleHills);
	playerSpawnBiomes.push_back(Biome::desert);
	playerSpawnBiomes.push_back(Biome::extremeHills);
	playerSpawnBiomes.push_back(Biome::swampland);
	playerSpawnBiomes.push_back(Biome::iceFlats);
	playerSpawnBiomes.push_back(Biome::iceMountains);
	playerSpawnBiomes.push_back(Biome::mushroomIsland);
	playerSpawnBiomes.push_back(Biome::mushroomIslandShore);
	playerSpawnBiomes.push_back(Biome::beaches);
	playerSpawnBiomes.push_back(Biome::desertHills);
	playerSpawnBiomes.push_back(Biome::smallerExtremeHills);
}

void BiomeSource::_init(int64_t seed, LevelType* generator)
{
	_init();

	LayerArray layers = Layer::getDefaultLayers(seed, generator);
	layer = layers[0];
	zoomedLayer = layers[1];

	delete[] layers.data;
}

BiomeSource::BiomeSource()
{
	_init();
}

BiomeSource::BiomeSource(int64_t seed, LevelType* generator)
{
	_init(seed, generator);
}

BiomeSource::BiomeSource(Level* level)
{
	_init(level->getSeed(), level->getLevelData()->getGenerator());
}

BiomeSource::~BiomeSource()
{
	delete cache;
}

Biome* BiomeSource::getBiome(ChunkPos* cp)
{
	return getBiome(cp->x << 4, cp->z << 4);
}

Biome* BiomeSource::getBiome(int x, int z)
{
	return cache->getBiome(x, z);
}

float BiomeSource::getDownfall(int x, int z) const
{
	return cache->getDownfall(x, z);
}

floatArray BiomeSource::getDownfallBlock(int x, int z, int w, int h) const
{
	floatArray downfalls;
	getDownfallBlock(downfalls, x, z, w, h);
	return downfalls;
}

void BiomeSource::getDownfallBlock(floatArray& downfalls, int x, int z, int w, int h) const
{
	IntCache::releaseAll();

	if (downfalls.data == nullptr || downfalls.length < w * h)
	{
		if (downfalls.data != nullptr) delete[] downfalls.data;
		downfalls = floatArray(w * h);
	}

	intArray result = zoomedLayer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		Biome* biome = Biome::getBiome(result[i]);
		float d = static_cast<float>(biome->getDownfallInt()) / 65536.0f;
		if (d > 1) d = 1;
		downfalls[i] = d;
	}
}

BiomeCache::Block* BiomeSource::getBlockAt(int x, int y)
{
	return cache->getBlockAt(x, y);
}

float BiomeSource::getTemperature(int x, int y, int z) const
{
	return scaleTemp(cache->getTemperature(x, z), y);
}

float BiomeSource::scaleTemp(float temp, int y) const
{
	return temp;
}

floatArray BiomeSource::getTemperatureBlock(int x, int z, int w, int h) const
{
	floatArray temperatures;
	getTemperatureBlock(temperatures, x, z, w, h);
	return temperatures;
}

void BiomeSource::getTemperatureBlock(floatArray& temperatures, int x, int z, int w, int h) const
{
	IntCache::releaseAll();

	if (temperatures.data == nullptr || temperatures.length < w * h)
	{
		if (temperatures.data != nullptr) delete[] temperatures.data;
		temperatures = floatArray(w * h);
	}

	intArray result = zoomedLayer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		Biome* biome = Biome::getBiome(result[i]);
		float t = static_cast<float>(biome->getTemperatureInt()) / 65536.0f;
		if (t > 1) t = 1;
		temperatures[i] = t;
	}
}

BiomeArray BiomeSource::getRawBiomeBlock(int x, int z, int w, int h) const
{
	BiomeArray biomes;
	getRawBiomeBlock(biomes, x, z, w, h);
	return biomes;
}

void BiomeSource::getRawBiomeIndices(intArray& biomes, int x, int z, int w, int h) const
{
	IntCache::releaseAll();

	intArray result = layer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		biomes[i] = result[i];
	}
}

void BiomeSource::getRawBiomeBlock(BiomeArray& biomes, int x, int z, int w, int h) const
{
	IntCache::releaseAll();

	if (biomes.data == nullptr || biomes.length < w * h)
	{
		if (biomes.data != nullptr) delete[] biomes.data;
		biomes = BiomeArray(w * h);
	}

	intArray result = layer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		biomes[i] = Biome::getBiome(result[i]);
#ifndef _CONTENT_PACKAGE
		if (biomes[i] == nullptr)
		{
			app.DebugPrintf("Tried to assign null biome %d\n", result[i]);
			__debugbreak();
		}
#endif
	}
}

BiomeArray BiomeSource::getBiomeBlock(int x, int z, int w, int h) const
{
	if (w == 16 && h == 16 && (x & 0xf) == 0 && (z & 0xf) == 0)
	{
		return cache->getBiomeBlockAt(x, z);
	}
	BiomeArray biomes;
	getBiomeBlock(biomes, x, z, w, h, true);
	return biomes;
}

void BiomeSource::getBiomeBlock(BiomeArray& biomes, int x, int z, int w, int h, bool useCache) const
{
	IntCache::releaseAll();

	if (biomes.data == nullptr || biomes.length < w * h)
	{
		if (biomes.data != nullptr) delete[] biomes.data;
		biomes = BiomeArray(w * h);
	}

	if (useCache && w == 16 && h == 16 && (x & 0xf) == 0 && (z & 0xf) == 0)
	{
		BiomeArray tmp = cache->getBiomeBlockAt(x, z);
		System::arraycopy(tmp, 0, &biomes, 0, w * h);
		delete tmp.data;
		// return; // если нужно только это, но у нас ниже ещё код? Оставим как было.
	}

	intArray result = zoomedLayer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		biomes[i] = Biome::getBiome(result[i]);
	}
}

byteArray BiomeSource::getBiomeIndexBlock(int x, int z, int w, int h) const
{
	if (w == 16 && h == 16 && (x & 0xf) == 0 && (z & 0xf) == 0)
	{
		return cache->getBiomeIndexBlockAt(x, z);
	}
	byteArray biomeIndices;
	getBiomeIndexBlock(biomeIndices, x, z, w, h, true);
	return biomeIndices;
}

void BiomeSource::getBiomeIndexBlock(byteArray& biomeIndices, int x, int z, int w, int h, bool useCache) const
{
	IntCache::releaseAll();

	if (biomeIndices.data == nullptr || biomeIndices.length < w * h)
	{
		if (biomeIndices.data != nullptr) delete[] biomeIndices.data;
		biomeIndices = byteArray(w * h);
	}

	if (useCache && w == 16 && h == 16 && (x & 0xf) == 0 && (z & 0xf) == 0)
	{
		byteArray tmp = cache->getBiomeIndexBlockAt(x, z);
		System::arraycopy(tmp, 0, &biomeIndices, 0, w * h);
		// return;
	}

	intArray result = zoomedLayer->getArea(x, z, w, h);
	for (int i = 0; i < w * h; i++)
	{
		biomeIndices[i] = static_cast<byte>(result[i]);
	}
}

bool BiomeSource::containsOnly(int x, int z, int r, vector<Biome*> allowed)
{
	IntCache::releaseAll();
	int x0 = ((x - r) >> 2);
	int z0 = ((z - r) >> 2);
	int x1 = ((x + r) >> 2);
	int z1 = ((z + r) >> 2);

	int w = x1 - x0 + 1;
	int h = z1 - z0 + 1;

	intArray biomes = layer->getArea(x0, z0, w, h);
	for (int i = 0; i < w * h; i++)
	{
		Biome* b = Biome::getBiome(biomes[i]);
		if (find(allowed.begin(), allowed.end(), b) == allowed.end()) return false;
	}

	return true;
}

bool BiomeSource::containsOnly(int x, int z, int r, Biome* allowed)
{
	IntCache::releaseAll();
	int x0 = ((x - r) >> 2);
	int z0 = ((z - r) >> 2);
	int x1 = ((x + r) >> 2);
	int z1 = ((z + r) >> 2);

	int w = x1 - x0;
	int h = z1 - z0;
	int biomesCount = w * h;
	intArray biomes = layer->getArea(x0, z0, w, h);
	for (unsigned int i = 0; i < biomesCount; i++)
	{
		Biome* b = Biome::getBiome(biomes[i]);
		if (allowed != b) return false;
	}

	return true;
}

TilePos* BiomeSource::findBiome(int x, int z, int r, Biome* toFind, Random* random)
{
	IntCache::releaseAll();
	int x0 = ((x - r) >> 2);
	int z0 = ((z - r) >> 2);
	int x1 = ((x + r) >> 2);
	int z1 = ((z + r) >> 2);

	int w = x1 - x0 + 1;
	int h = z1 - z0 + 1;
	intArray biomes = layer->getArea(x0, z0, w, h);
	TilePos* res = nullptr;
	int found = 0;
	int biomesCount = w * h;
	for (unsigned int i = 0; i < biomesCount; i++)
	{
		int xx = x0 + i % w;
		int zz = z0 + i / w;
		Biome* b = Biome::getBiome(biomes[i]);
		if (b == toFind)
		{
			if (res == nullptr || random->nextInt(found + 1) == 0)
			{
				res = new TilePos(xx, 0, zz);
				found++;
			}
		}
	}

	return res;
}

TilePos* BiomeSource::findBiome(int x, int z, int r, vector<Biome*> allowed, Random* random)
{
	IntCache::releaseAll();
	int x0 = ((x - r) >> 2);
	int z0 = ((z - r) >> 2);
	int x1 = ((x + r) >> 2);
	int z1 = ((z + r) >> 2);

	int w = x1 - x0 + 1;
	int h = z1 - z0 + 1;
	MemSect(50);
	intArray biomes = layer->getArea(x0, z0, w, h);
	TilePos* res = nullptr;
	int found = 0;
	for (unsigned int i = 0; i < w * h; i++)
	{
		int xx = (x0 + i % w) << 2;
		int zz = (z0 + i / w) << 2;
		Biome* b = Biome::getBiome(biomes[i]);
		if (find(allowed.begin(), allowed.end(), b) != allowed.end())
		{
			if (res == nullptr || random->nextInt(found + 1) == 0)
			{
				delete res;
				res = new TilePos(xx, 0, zz);
				found++;
			}
		}
	}
	MemSect(0);

	return res;
}

void BiomeSource::update()
{
	cache->update();
}

const int MAX_SEED_ATTEMPTS = 100;

#ifdef __PSVITA__
int64_t BiomeSource::findSeed(LevelType* generator, bool* pServerRunning)
#else
int64_t BiomeSource::findSeed(LevelType* generator)
#endif
{
	// Статический счетчик вызовов для уникальности начального seed генератора
	static int64_t callCounter = 0;

	ProgressRenderer* mcprogress = Minecraft::GetInstance()->progressRenderer;
	mcprogress->progressStage(IDS_PROGRESS_NEW_WORLD_SEED);

	int64_t bestSeed = 0;
	int64_t lastSeed = 0; // запоминаем последний испытанный seed

	// Используем более случайное начальное значение для Random
	// Комбинируем время и счетчик вызовов
	Random* pr = new Random(System::nanoTime() ^ (++callCounter));

	static const int biomeWidth = (64 * 2) - 16;
	static const int biomeOffset = -(biomeWidth / 2);

	intArray indices = intArray(biomeWidth * biomeWidth);
	float toCompare[Biome::BIOME_COUNT];

	bool matchFound = false;
	int tryCount = 0;

	do
	{
		// Генерируем seed для мира
		int64_t seed = System::currentTimeMillis() ^ (tryCount << 32) ^ (int64_t)pr->nextLong();
		lastSeed = seed; // сохраняем на случай неудачи

		BiomeSource* biomeSource = new BiomeSource(seed, generator);

		biomeSource->getRawBiomeIndices(indices, biomeOffset, biomeOffset, biomeWidth, biomeWidth);
		getFracs(indices, toCompare);

		matchFound = getIsMatch(toCompare);

		if (matchFound)
		{
			bestSeed = seed;
		}

		delete biomeSource;
		tryCount++;

		mcprogress->progressStagePercentage(tryCount % 100);

#ifdef __PSVITA__
	} while (!matchFound && *pServerRunning && tryCount < MAX_SEED_ATTEMPTS);
#else
	} while (!matchFound && tryCount < MAX_SEED_ATTEMPTS);
#endif

	delete pr;
	delete indices.data;

	// Если подходящий seed не найден, используем последний испытанный
	if (!matchFound)
	{
		bestSeed = lastSeed;
	}

	// Отладочная генерация изображения (оставлена как есть)
	#if 1
	int size = 64 * 16;
	BiomeSource* biomeSource = new BiomeSource(bestSeed, generator);
	BiomeArray biomes = biomeSource->getBiomeBlock(-27 * 16, -27 * 16, size, size);

	unsigned int* pixels = new unsigned int[size * size];
	for (int i = 0; i < size * size; i++)
	{
		int id = biomes[i]->id;
		if (id == 1) id = 14;
		else if (id == 14) id = 1;
		else if (id == 9) id = 15;
		else if (id == 15) id = 9;
		pixels[i] = 0xff000000;
		if (id & 1) pixels[i] |= 0x00ff0000;
		if (id & 2) pixels[i] |= 0x0000ff00;
		if (id & 4) pixels[i] |= 0x000000ff;
		if (id & 8) pixels[i] |= 0x00808080;
	}
	D3DXIMAGE_INFO srcInfo;
	srcInfo.Width = size;
	srcInfo.Height = size;

	char buf[256];
	sprintf(buf, "GAME:\\BiomeTest%d.bmp", bestSeed);
	RenderManager.SaveTextureData(buf, &srcInfo, (int*)pixels);

	delete[] pixels;
	delete biomes.data;
	delete biomeSource;
	#endif

	return bestSeed;
}

void BiomeSource::getFracs(intArray indices, float* fracs)
{
	for (int i = 0; i < Biome::getBiomeCount(); i++)
	{
		fracs[i] = 0.0f;
	}

	for (int i = 0; i < indices.length; i++)
	{
		fracs[indices[i]] += 1.0f;
	}

	for (int i = 0; i < Biome::getBiomeCount(); i++)
	{
		fracs[i] /= static_cast<float>(indices.length);
	}
}

bool BiomeSource::getIsMatch(float* frac)
{
	// Океан может занимать до 50%
	if (frac[0] > 0.5f) return false;

	int typeCount = 0;
	for (int i = 0; i < Biome::getBiomeCount(); i++) {
		if (i == 15 || i == 17 || i == 18 || i == 19 || i == 22) continue;
		if (frac[i] > 0.001f) typeCount++;
	}
	return typeCount >= 12;
}
