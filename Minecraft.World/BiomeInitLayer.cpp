#include "stdafx.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "net.minecraft.world.level.h"
#include "BiomeInitLayer.h"

BiomeInitLayer::BiomeInitLayer(int64_t seed, shared_ptr<Layer> parent, LevelType* levelType) : Layer(seed)
{
    this->parent = parent;

    // Собираем все биомы Верхнего мира, кроме ада и энда
    std::vector<Biome*> overworldBiomes;
    overworldBiomes.push_back(Biome::plains);
    overworldBiomes.push_back(Biome::desert);
    overworldBiomes.push_back(Biome::extremeHills);
    overworldBiomes.push_back(Biome::forest);
    overworldBiomes.push_back(Biome::birchForest);
    overworldBiomes.push_back(Biome::rainForest);
    overworldBiomes.push_back(Biome::taiga);
    overworldBiomes.push_back(Biome::taigaSnowed);
    overworldBiomes.push_back(Biome::swampland);
    overworldBiomes.push_back(Biome::beaches);
    overworldBiomes.push_back(Biome::jungle);

    // Создаем массив нужного размера и копируем указатели
    startBiomes = BiomeArray(overworldBiomes.size());
    for (size_t i = 0; i < overworldBiomes.size(); ++i)
    {
        startBiomes[i] = overworldBiomes[i];
    }
}

BiomeInitLayer::~BiomeInitLayer()
{
	delete [] startBiomes.data;
}

intArray BiomeInitLayer::getArea(int xo, int yo, int w, int h)
{
    intArray b = parent->getArea(xo, yo, w, h);
    intArray result = IntCache::allocate(w * h);

    // Получаем ID грибного острова (с проверкой)
    int mushroomId = Biome::mushroomIsland ? Biome::mushroomIsland->id : -1;

    // Проверка, что startBiomes валидный
    if (startBiomes.data == nullptr || startBiomes.length == 0)
    {
        // Если нет биомов, заполняем нулями (океан)
        for (int i = 0; i < w * h; ++i)
            result[i] = 0;
        return result;
    }

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            initRandom(x + xo, y + yo);
            int old = b[x + y * w];

            // Если ячейка помечена как океан (0), оставляем океан
            if (old == 0)
            {
                result[x + y * w] = 0;
            }
            // Если это грибной остров, сохраняем его
            else if (mushroomId != -1 && old == mushroomId)
            {
                result[x + y * w] = old;
            }
            // Во всех остальных случаях выбираем случайный биом из списка
            else
            {
                int idx = nextRandom(startBiomes.length);
                result[x + y * w] = startBiomes[idx]->id;
            }
        }
    }

    return result;
}
