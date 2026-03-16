#include "stdafx.h"
#include "Chunk.h"
#include "TileRenderer.h"
#include "TileEntityRenderDispatcher.h"
#include "..\Minecraft.World\net.minecraft.world.level.h"
#include "..\Minecraft.World\net.minecraft.world.level.chunk.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.entity.h"
#include "LevelRenderer.h"

#ifdef __PS3__
#include "PS3\SPU_Tasks\ChunkUpdate\ChunkRebuildData.h"
#include "PS3\SPU_Tasks\ChunkUpdate\TileRenderer_SPU.h"
#include "PS3\SPU_Tasks\CompressedTile\CompressedTileStorage_SPU.h"

#include "C4JThread_SPU.h"
#include "C4JSpursJob.h"
//#define DISABLE_SPU_CODE

#endif

#ifdef _WINDOWS64
#include <emmintrin.h>  // SSE2 intrinsics
#endif

int Chunk::updates = 0;

#ifdef _LARGE_WORLDS
DWORD Chunk::tlsIdx = TlsAlloc();

void Chunk::CreateNewThreadStorage()
{
	unsigned char *tileIds = new unsigned char[16 * 16 * Level::maxBuildHeight];
	TlsSetValue(tlsIdx, tileIds);
}

void Chunk::ReleaseThreadStorage()
{
	unsigned char *tileIds = static_cast<unsigned char *>(TlsGetValue(tlsIdx));
	delete tileIds;
}

unsigned char *Chunk::GetTileIdsStorage()
{
	unsigned char *tileIds = static_cast<unsigned char *>(TlsGetValue(tlsIdx));
	return tileIds;
}
#else
// 4J Stu - Don't want this when multi-threaded
Tesselator *Chunk::t = Tesselator::getInstance();
#endif
LevelRenderer *Chunk::levelRenderer;

// TODO - 4J see how input entity vector is set up and decide what way is best to pass this to the function
Chunk::Chunk(Level *level, LevelRenderer::rteMap &globalRenderableTileEntities, CRITICAL_SECTION& globalRenderableTileEntities_cs, int x, int y, int z, ClipChunk *clipChunk)
	: globalRenderableTileEntities( &globalRenderableTileEntities ), globalRenderableTileEntities_cs(&globalRenderableTileEntities_cs)
{
	clipChunk->visible = false;
	bb = nullptr;
	id = 0;

	this->level = level;
	//this->globalRenderableTileEntities = globalRenderableTileEntities;

	assigned = false;
	this->clipChunk = clipChunk;
	setPos(x, y, z);
}

void Chunk::setPos(int x, int y, int z)
{
	if(assigned && (x == this->x && y == this->y && z == this->z)) return;

	reset();

	this->x = x;
	this->y = y;
	this->z = z;
	xm = x + XZSIZE / 2;
	ym = y + SIZE / 2;
	zm = z + XZSIZE / 2;
	clipChunk->xm = xm;
	clipChunk->ym = ym;
	clipChunk->zm = zm;

	clipChunk->globalIdx = LevelRenderer::getGlobalIndexForChunk(x, y, z, level);

#if 1
	// 4J - we're not using offsetted renderlists anymore, so just set the full position of this chunk into x/y/zRenderOffs where
	// it will be used directly in the renderlist of this chunk
	xRenderOffs = x;
	yRenderOffs = y;
	zRenderOffs = z;
	xRender = 0;
	yRender = 0;
	zRender = 0;
#else
	xRenderOffs = x & 1023;
	yRenderOffs = y;
	zRenderOffs = z & 1023;
	xRender = x - xRenderOffs;
	yRender = y - yRenderOffs;
	zRender = z - zRenderOffs;
#endif

	float g = 6.0f;
	// 4J - changed to just set the value rather than make a new one, if we've already created storage
	if( !bb )
	{
		bb = shared_ptr<AABB>(AABB::newPermanent(-g, -g, -g, XZSIZE+g, SIZE+g, XZSIZE+g));
	}
	else
	{
		// 4J MGH - bounds are relative to the position now, so the AABB will be setup already, either above, or from the tesselator bounds.
// 		bb->set(-g, -g, -g, SIZE+g, SIZE+g, SIZE+g);
	}
	clipChunk->aabb[0] = bb->x0 + x;
	clipChunk->aabb[1] = bb->y0 + y;
	clipChunk->aabb[2] = bb->z0 + z;
	clipChunk->aabb[3] = bb->x1 + x;
	clipChunk->aabb[4] = bb->y1 + y;
	clipChunk->aabb[5] = bb->z1 + z;

	assigned = true;

	EnterCriticalSection(&levelRenderer->m_csDirtyChunks);
	unsigned char refCount = levelRenderer->incGlobalChunkRefCount(x, y, z, level);
//	printf("\t\t [inc] refcount %d at %d, %d, %d\n",refCount,x,y,z);

//	int idx = levelRenderer->getGlobalIndexForChunk(x, y, z, level);

	// If we're the first thing to be referencing this, mark it up as dirty to get rebuilt
	if( refCount == 1 )
	{
//		printf("Setting %d %d %d dirty [%d]\n",x,y,z, idx);
		// Chunks being made dirty in this way can be very numerous (eg the full visible area of the world at start up, or a whole edge of the world when moving).
		// On account of this, don't want to stick them into our lock free queue that we would normally use for letting the render update thread know about this chunk.
		// Instead, just set the flag to say this is dirty, and then pass a special value of 1 through to the lock free stack which lets that thread know that at least
		// one chunk other than the ones in the stack itself have been made dirty.
		levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_DIRTY );
#ifdef _XBOX
		PIXSetMarker(0,"Non-stack event pushed");
#else
		PIXSetMarkerDeprecated(0,"Non-stack event pushed");
#endif
	}

	LeaveCriticalSection(&levelRenderer->m_csDirtyChunks);


}

void Chunk::translateToPos()
{
	glTranslatef(static_cast<float>(xRenderOffs), static_cast<float>(yRenderOffs), static_cast<float>(zRenderOffs));
}


Chunk::Chunk()
{
	bb = nullptr;
}

void Chunk::makeCopyForRebuild(Chunk *source)
{
	this->level = source->level;
	this->x = source->x;
	this->y = source->y;
	this->z = source->z;
	this->xRender = source->xRender;
	this->yRender = source->yRender;
	this->zRender = source->zRender;
	this->xRenderOffs = source->xRenderOffs;
	this->yRenderOffs = source->yRenderOffs;
	this->zRenderOffs = source->zRenderOffs;
	this->xm = source->xm;
	this->ym = source->ym;
	this->zm = source->zm;
	this->bb = source->bb;
	this->clipChunk = nullptr;
	this->id = source->id;
	this->globalRenderableTileEntities = source->globalRenderableTileEntities;
	this->globalRenderableTileEntities_cs = source->globalRenderableTileEntities_cs;
}

void Chunk::rebuild()
{
	PIXBeginNamedEvent(0, "Rebuilding chunk %d, %d, %d", x, y, z);
#if defined __PS3__ && !defined DISABLE_SPU_CODE
	rebuild_SPU();
	return;
#endif

	PIXBeginNamedEvent(0, "Rebuild section A");

#ifdef _LARGE_WORLDS
	Tesselator* t = Tesselator::getInstance();
#else
	Chunk::t = Tesselator::getInstance();
#endif

	updates++;

	const int x0 = x;
	const int y0 = y;
	const int z0 = z;
	const int x1 = x + XZSIZE;
	const int y1 = y + SIZE;
	const int z1 = z + XZSIZE;

	LevelChunk::touchedSky = false;

	vector<shared_ptr<TileEntity>> renderableTileEntities;
	renderableTileEntities.reserve(16);

	const int r = 1;

	int lists = levelRenderer->getGlobalIndexForChunk(this->x, this->y, this->z, level) * 2;
	lists += levelRenderer->chunkLists;

	PIXEndNamedEvent();
	PIXBeginNamedEvent(0, "Rebuild section B");

#ifdef _LARGE_WORLDS
	unsigned char* tileIds = GetTileIdsStorage();
#else
	static unsigned char tileIds[16 * 16 * Level::maxBuildHeight];
#endif
	byteArray tileArray = byteArray(tileIds, 16 * 16 * Level::maxBuildHeight);
	level->getChunkAt(x, z)->getBlockData(tileArray);

	Region       region(level, x0 - r, y0 - r, z0 - r, x1 + r, y1 + r, z1 + r, r);
	TileRenderer tileRenderer(&region, this->x, this->y, this->z, tileIds);

#ifdef __PSVITA__
	{
		const int xc = x >> 4;
		const int zc = z >> 4;
		region.setCachedTiles(tileIds, xc, zc);
	}
#endif

	// ── Скалярная isSolid: используется только в non-Windows64 путях ──────────
	auto isSolid = [](unsigned char id)
	{
		return id == Tile::stone_Id
			|| id == Tile::dirt_Id
			|| id == Tile::unbreakable_Id
			|| id == 255;
	};

	bool empty = true;

#ifdef _WINDOWS64
	// ═══════════════════════════════════════════════════════════════════════════
	// SSE2 Section B
	//
	// Ключевые факты из констант:
	//   SIZE                         = 16   → y-диапазон чанка ровно 16 байт
	//   COMPRESSED_CHUNK_SECTION_HEIGHT = 128 → кратно SIZE=16
	//
	// Вывод: render-чанки ВСЕГДА целиком лежат в одной секции — граница 128
	// никогда не попадает внутрь чанка. Скалярный fallback мёртв.
	// SSE2 загружает весь y-диапазон за одну инструкцию (_mm_loadu_si128).
	// ═══════════════════════════════════════════════════════════════════════════
	{
		// Выбираем секцию один раз — больше никаких ветвлений на каждый тайл
		// inUpperSection = true  → y0 >= 128, base = COMPRESSED_CHUNK_SECTION_TILES
		// inUpperSection = false → y0 <  128, base = 0
		const bool inUpperSection = (y0 >= Level::COMPRESSED_CHUNK_SECTION_HEIGHT);
		const int  sectionBase = inUpperSection
			? Level::COMPRESSED_CHUNK_SECTION_TILES
			: 0;
		// y-координата внутри секции для y0 (0..111 или 0..127)
		const int  yAdj = inUpperSection
			? y0 - Level::COMPRESSED_CHUNK_SECTION_HEIGHT
			: y0;

		// ── SSE2 константы ───────────────────────────────────────────────────
		const __m128i zero = _mm_setzero_si128();
		const __m128i v_stone = _mm_set1_epi8(static_cast<char>(Tile::stone_Id));
		const __m128i v_dirt = _mm_set1_epi8(static_cast<char>(Tile::dirt_Id));
		const __m128i v_unbr = _mm_set1_epi8(static_cast<char>(Tile::unbreakable_Id));
		const __m128i v_ff = _mm_set1_epi8(static_cast<char>(0xFF));

		// Возвращает маску 0xFF для каждого байта где тайл "сплошной"
		// Инлайнится в 4 × cmpeq + 3 × por — 7 инструкций
		auto isSolid128 = [&](__m128i v) -> __m128i
		{
			return _mm_or_si128(
				_mm_or_si128(_mm_cmpeq_epi8(v, v_stone), _mm_cmpeq_epi8(v, v_dirt)),
				_mm_or_si128(_mm_cmpeq_epi8(v, v_unbr), _mm_cmpeq_epi8(v, v_ff))
			);
		};

		// ── Шаг 1. Проверка пустоты — 256 столбцов × 1 SSE2-загрузка ─────────
		//
		// SIZE=16 → весь y-диапазон чанка = ровно 128 бит = один XMM-регистр.
		// Итого 256 загрузок вместо 4096 скалярных сравнений.
		// movemask(cmpeq(v, zero)) == 0xFFFF означает "все 16 байт равны нулю".
		for (int zz = 0; zz < 16 && empty; zz++)
		{
			for (int xx = 0; xx < 16 && empty; xx++)
			{
				const unsigned char* col =
					tileIds + sectionBase + (xx << 11) + (zz << 7) + yAdj;

				const __m128i v = _mm_loadu_si128(
					reinterpret_cast<const __m128i*>(col));

				if (_mm_movemask_epi8(_mm_cmpeq_epi8(v, zero)) != 0xFFFF)
					empty = false;
			}
		}

		// ── Шаг 2. Маркировка скрытых тайлов — 196 внутренних столбцов ───────
		//
		// Для каждого внутреннего (xx, zz) столбца:
		//   • 7 загрузок: cur + 4 горизонтальных + 2 вертикальных соседа
		//   • isSolid128 × 7 → AND всех масок → surrounded
		//   • Результат: OR(cur, surrounded) записывает 0xFF только скрытым
		//
		// edgeMask исключает крайние y-позиции где нет корректного соседа:
		//   byte[0]  маскируется если y0==0 (дно мира)
		//            или y0==128 (сосед y=-1 находится в другой секции)
		//   byte[15] маскируется если y1==maxBuildHeight (потолок мира)
		//            или y1==128 (сосед y=128 находится в другой секции)
		if (!empty)
		{
			// Строим edgeMask — вычисляется один раз для всего чанка
			alignas(16) unsigned char edgeMaskBytes[16];
			memset(edgeMaskBytes, 0xFF, sizeof(edgeMaskBytes));

			// Нижний сосед отсутствует: дно мира или начало верхней секции
			if (y0 == Level::minBuildHeight ||
				y0 == Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
				edgeMaskBytes[0] = 0x00;

			// Верхний сосед отсутствует: потолок мира или конец нижней секции
			if (y1 == Level::maxBuildHeight ||
				y1 == Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
				edgeMaskBytes[15] = 0x00;

			const __m128i edgeMask = _mm_load_si128(
				reinterpret_cast<const __m128i*>(edgeMaskBytes));

			for (int zz = 1; zz < 15; zz++)
			{
				for (int xx = 1; xx < 15; xx++)
				{
					unsigned char* pCur =
						tileIds + sectionBase + (xx << 11) + (zz << 7) + yAdj;

					// 7 загрузок: cur, ±x, ±z, ±y
					// ym1/yp1 — соседние байты в непрерывном столбце (±1 байт)
					// Корректны т.к. чанк целиком в одной секции:
					//   - pCur-1 при yAdj>0: валидно (внутри секции)
					//   - pCur+1 при yAdj+15<sectionHeight: валидно
					//   - edgeMask обнулит результат на граничных позициях
					const __m128i cur = _mm_loadu_si128(reinterpret_cast<__m128i*>(pCur));
					const __m128i xm1 = _mm_loadu_si128(reinterpret_cast<__m128i*>(
						tileIds + sectionBase + ((xx - 1) << 11) + (zz << 7) + yAdj));
					const __m128i xp1 = _mm_loadu_si128(reinterpret_cast<__m128i*>(
						tileIds + sectionBase + ((xx + 1) << 11) + (zz << 7) + yAdj));
					const __m128i zm1 = _mm_loadu_si128(reinterpret_cast<__m128i*>(
						tileIds + sectionBase + (xx << 11) + ((zz - 1) << 7) + yAdj));
					const __m128i zp1 = _mm_loadu_si128(reinterpret_cast<__m128i*>(
						tileIds + sectionBase + (xx << 11) + ((zz + 1) << 7) + yAdj));
					const __m128i ym1 = _mm_loadu_si128(
						reinterpret_cast<__m128i*>(pCur - 1));
					const __m128i yp1 = _mm_loadu_si128(
						reinterpret_cast<__m128i*>(pCur + 1));

					// Все 7 направлений должны быть сплошными
					__m128i surrounded = _mm_and_si128(
						_mm_and_si128(isSolid128(cur), isSolid128(xm1)),
						_mm_and_si128(isSolid128(xp1),
							_mm_and_si128(isSolid128(zm1),
								_mm_and_si128(isSolid128(zp1),
									_mm_and_si128(isSolid128(ym1), isSolid128(yp1))))));

					// Защита от граничных y-позиций
					surrounded = _mm_and_si128(surrounded, edgeMask);

					// OR(cur, 0xFF) = 0xFF помечен, OR(cur, 0x00) = cur без изменений
					_mm_storeu_si128(reinterpret_cast<__m128i*>(pCur),
						_mm_or_si128(cur, surrounded));
				}
			}
		}
	}

#else // !_WINDOWS64 — скалярный путь без изменений
	for (int yy = y0; yy < y1; yy++)
	{
		for (int zz = 0; zz < 16; zz++)
		{
			for (int xx = 0; xx < 16; xx++)
			{
				int indexY = yy;
				int offset = 0;
				if (indexY >= Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
				{
					indexY -= Level::COMPRESSED_CHUNK_SECTION_HEIGHT;
					offset = Level::COMPRESSED_CHUNK_SECTION_TILES;
				}

				unsigned char tileId = tileIds[offset + ((xx << 11) | (zz << 7) | indexY)];
				if (tileId > 0) empty = false;

				if (yy == (Level::maxBuildHeight - 1)) continue;
				if ((xx == 0) || (xx == 15))           continue;
				if ((zz == 0) || (zz == 15))           continue;

				if (!isSolid(tileId))                                                            continue;
				if (!isSolid(tileIds[offset + (((xx - 1) << 11) | (zz << 7) | indexY)])) continue;
				if (!isSolid(tileIds[offset + (((xx + 1) << 11) | (zz << 7) | indexY)])) continue;
				if (!isSolid(tileIds[offset + ((xx << 11) | ((zz - 1) << 7) | indexY)])) continue;
				if (!isSolid(tileIds[offset + ((xx << 11) | ((zz + 1) << 7) | indexY)])) continue;

				if (yy > 0)
				{
					int idxYm1 = yy - 1;
					int offYm1 = 0;
					if (idxYm1 >= Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
					{
						idxYm1 -= Level::COMPRESSED_CHUNK_SECTION_HEIGHT;
						offYm1 = Level::COMPRESSED_CHUNK_SECTION_TILES;
					}
					if (!isSolid(tileIds[offYm1 + ((xx << 11) | (zz << 7) | idxYm1)])) continue;
				}

				int idxYp1 = yy + 1;
				int offYp1 = 0;
				if (idxYp1 >= Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
				{
					idxYp1 -= Level::COMPRESSED_CHUNK_SECTION_HEIGHT;
					offYp1 = Level::COMPRESSED_CHUNK_SECTION_TILES;
				}
				if (!isSolid(tileIds[offYp1 + ((xx << 11) | (zz << 7) | idxYp1)])) continue;

				tileIds[offset + ((xx << 11) | (zz << 7) | indexY)] = 0xff;
			}
		}
	}
#endif // _WINDOWS64

	PIXEndNamedEvent();

	if (empty)
	{
		for (int currentLayer = 0; currentLayer < 2; currentLayer++)
		{
			levelRenderer->setGlobalChunkFlag(this->x, this->y, this->z, level,
				LevelRenderer::CHUNK_FLAG_EMPTY0, currentLayer);
			RenderManager.CBuffClear(lists + currentLayer);
		}
		return;
	}

	PIXBeginNamedEvent(0, "Rebuild section C");
	Tesselator::Bounds bounds;
	{
		const float g = 6.0f;
		bounds.boundingBox[0] = -g;    bounds.boundingBox[3] = XZSIZE + g;
		bounds.boundingBox[1] = -g;    bounds.boundingBox[4] = SIZE + g;
		bounds.boundingBox[2] = -g;    bounds.boundingBox[5] = XZSIZE + g;
	}

	for (int currentLayer = 0; currentLayer < 2; currentLayer++)
	{
		bool renderNextLayer = false;
		bool rendered = false;
		bool started = false;
		const bool collectEntities = (currentLayer == 0);

		auto ensureStarted = [&]()
		{
			if (started) return;
			started = true;
			MemSect(31);
			glNewList(lists + currentLayer, GL_COMPILE);
			MemSect(0);
			glPushMatrix();
			glDepthMask(true);
			t->useCompactVertices(true);
			translateToPos();
			t->begin();
			t->offset(static_cast<float>(-this->x),
				static_cast<float>(-this->y),
				static_cast<float>(-this->z));
		};

		auto processTile = [&](unsigned char tileId, int tx, int ty, int tz)
		{
			Tile* tile = Tile::tiles[tileId];
			if (!tile) [[unlikely]] return;

			if (collectEntities && tile->isEntityTile())
			{
				shared_ptr<TileEntity> et = region.getTileEntity(tx, ty, tz);
				if (TileEntityRenderDispatcher::instance->hasRenderer(et))
					renderableTileEntities.push_back(et);
			}

			const int renderLayer = tile->getRenderLayer();
			if (renderLayer != currentLayer)
				renderNextLayer = true;
			else
				rendered |= tileRenderer.tesselateInWorld(tile, tx, ty, tz);
		};

		for (int z = z0; z < z1; z++)
		{
			for (int x = x0; x < x1; x++)
			{
				const int xzIndex = ((x - x0) << 11) | ((z - z0) << 7);

				const int yLow1 = min(y1, Level::COMPRESSED_CHUNK_SECTION_HEIGHT);
				for (int y = y0; y < yLow1; y++)
				{
					const unsigned char tileId = tileIds[xzIndex | y];
					if (tileId == 0xff || tileId == 0) continue;
					ensureStarted();
					processTile(tileId, x, y, z);
				}

				if (y1 > Level::COMPRESSED_CHUNK_SECTION_HEIGHT)
				{
					const unsigned char* tilesHigh =
						tileIds + Level::COMPRESSED_CHUNK_SECTION_TILES;
					for (int y = Level::COMPRESSED_CHUNK_SECTION_HEIGHT; y < y1; y++)
					{
						const int indexY = y - Level::COMPRESSED_CHUNK_SECTION_HEIGHT;
						const unsigned char tileId = tilesHigh[xzIndex | indexY];
						if (tileId == 0xff || tileId == 0) continue;
						ensureStarted();
						processTile(tileId, x, y, z);
					}
				}
			}
		}

#ifdef __PSVITA__
		if (currentLayer == 0)
			levelRenderer->clearGlobalChunkFlag(this->x, this->y, this->z, level,
				LevelRenderer::CHUNK_FLAG_CUT_OUT);
#endif

		if (started)
		{
#ifdef __PSVITA__
			if (t->getCutOutFound())
				levelRenderer->setGlobalChunkFlag(this->x, this->y, this->z, level,
					LevelRenderer::CHUNK_FLAG_CUT_OUT);
#endif
			t->end();
			bounds.addBounds(t->bounds);
			glPopMatrix();
			glEndList();
			t->useCompactVertices(false);
			t->offset(0, 0, 0);
		}
		else
		{
			rendered = false;
		}

		if (rendered)
			levelRenderer->clearGlobalChunkFlag(this->x, this->y, this->z, level,
				LevelRenderer::CHUNK_FLAG_EMPTY0, currentLayer);
		else
		{
			levelRenderer->setGlobalChunkFlag(this->x, this->y, this->z, level,
				LevelRenderer::CHUNK_FLAG_EMPTY0, currentLayer);
			RenderManager.CBuffClear(lists + currentLayer);
	}

		if ((currentLayer == 0) && (!renderNextLayer))
		{
			levelRenderer->setGlobalChunkFlag(this->x, this->y, this->z, level,
				LevelRenderer::CHUNK_FLAG_EMPTY1);
			RenderManager.CBuffClear(lists + 1);
			break;
}
	}

	if (bb)
	{
		bb->set(bounds.boundingBox[0], bounds.boundingBox[1], bounds.boundingBox[2],
			bounds.boundingBox[3], bounds.boundingBox[4], bounds.boundingBox[5]);
	}

	PIXEndNamedEvent();
	PIXBeginNamedEvent(0, "Rebuild section D");

	const int key = levelRenderer->getGlobalIndexForChunk(this->x, this->y, this->z, level);

	unordered_set<shared_ptr<TileEntity>> newEntitiesSet(
		renderableTileEntities.begin(), renderableTileEntities.end());

	vector<shared_ptr<TileEntity>> toAdd;
	vector<shared_ptr<TileEntity>> toRemove;

	{
		EnterCriticalSection(globalRenderableTileEntities_cs);
		auto it = globalRenderableTileEntities->find(key);
		if (it != globalRenderableTileEntities->end())
			for (const auto& existing : it->second)
				if (newEntitiesSet.count(existing) == 0)
					toRemove.push_back(existing);
		LeaveCriticalSection(globalRenderableTileEntities_cs);
	}

	{
		EnterCriticalSection(globalRenderableTileEntities_cs);
		auto it = globalRenderableTileEntities->find(key);
		const bool hasEx = (it != globalRenderableTileEntities->end());
		for (const auto& fresh : renderableTileEntities)
			if (!hasEx || find(it->second.begin(), it->second.end(), fresh) == it->second.end())
				toAdd.push_back(fresh);
		LeaveCriticalSection(globalRenderableTileEntities_cs);
	}

	EnterCriticalSection(globalRenderableTileEntities_cs);
	{
		for (auto& e : toRemove)
			e->setRenderRemoveStage(TileEntity::e_RenderRemoveStageFlaggedAtChunk);

		if (!renderableTileEntities.empty())
		{
			auto it = globalRenderableTileEntities->find(key);
			if (it != globalRenderableTileEntities->end())
				for (auto& existing : it->second)
					if (newEntitiesSet.count(existing) > 0)
						existing->setRenderRemoveStage(TileEntity::e_RenderRemoveStageKeep);
		}

		auto& globalList = (*globalRenderableTileEntities)[key];
		for (auto& e : toAdd)
			globalList.push_back(e);
	}
	LeaveCriticalSection(globalRenderableTileEntities_cs);

	PIXEndNamedEvent();

	if (LevelChunk::touchedSky)
		levelRenderer->clearGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_NOTSKYLIT);
	else
		levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_NOTSKYLIT);

	levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_COMPILED);
	PIXEndNamedEvent();
}

#ifdef __PS3__
ChunkRebuildData g_rebuildDataIn __attribute__((__aligned__(16)));
ChunkRebuildData g_rebuildDataOut __attribute__((__aligned__(16)));
TileCompressData_SPU g_tileCompressDataIn __attribute__((__aligned__(16)));
unsigned char* g_tileCompressDataOut = (unsigned char*)&g_rebuildDataIn.m_tileIds;


void RunSPURebuild()
{

	static C4JSpursJobQueue::Port p("C4JSpursJob_ChunkUpdate");
	C4JSpursJob_CompressedTile tileJob(&g_tileCompressDataIn,g_tileCompressDataOut);
	C4JSpursJob_ChunkUpdate chunkJob(&g_rebuildDataIn, &g_rebuildDataOut);

	if(g_rebuildDataIn.m_currentLayer == 0)		// only need to create the tiles on the first layer
	{
		p.submitJob(&tileJob);
 		p.submitSync();
	}

	p.submitJob(&chunkJob);
	p.waitForCompletion();

	assert(g_rebuildDataIn.m_x0 == g_rebuildDataOut.m_x0);
}

void Chunk::rebuild_SPU()
{

//	if (!dirty) return;
	Chunk::t = Tesselator::getInstance(); // 4J - added - static initialiser being set at the wrong time
	updates++;

	int x0 = x;
	int y0 = y;
	int z0 = z;
	int x1 = x + SIZE;
	int y1 = y + SIZE;
	int z1 = z + SIZE;

	LevelChunk::touchedSky = false;

//	unordered_set<shared_ptr<TileEntity> > oldTileEntities(renderableTileEntities.begin(),renderableTileEntities.end());		// 4J removed this & next line
//	renderableTileEntities.clear();

	vector<shared_ptr<TileEntity> > renderableTileEntities;	// 4J - added

//        List<TileEntity> newTileEntities = new ArrayList<TileEntity>();
//        newTileEntities.clear();
//        renderableTileEntities.clear();

	int r = 1;

	Region region(level, x0 - r, y0 - r, z0 - r, x1 + r, y1 + r, z1 + r, r);
	TileRenderer tileRenderer(&region);

	int lists = levelRenderer->getGlobalIndexForChunk(this->x,this->y,this->z,level) * 2;
	lists += levelRenderer->chunkLists;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 4J - optimisation begins.

	// Get the data for the level chunk that this render chunk is it (level chunk is 16 x 16 x 128,
	// render chunk is 16 x 16 x 16. We wouldn't have to actually get all of it if the data was ordered differently, but currently
	// it is ordered by x then z then y so just getting a small range of y out of it would involve getting the whole thing into
	// the cache anyway.
	ChunkRebuildData* pOutData = nullptr;
	g_rebuildDataIn.buildForChunk(&region, level, x0, y0, z0);

 	Tesselator::Bounds bounds;
	{
		// this was the old default clip bounds for the chunk, set in Chunk::setPos.
		float g = 6.0f;
		bounds.boundingBox[0] = -g;
		bounds.boundingBox[1] = -g;
		bounds.boundingBox[2] = -g;
		bounds.boundingBox[3] = SIZE+g;
		bounds.boundingBox[4] = SIZE+g;
		bounds.boundingBox[5] = SIZE+g;
	}

	for (int currentLayer = 0; currentLayer < 2; currentLayer++)
	{
		bool rendered = false;

		{
			glNewList(lists + currentLayer, GL_COMPILE);
			MemSect(0);
			glPushMatrix();
			glDepthMask(true);	// 4J added
			t->useCompactVertices(true);	 // 4J added
			translateToPos();
			float ss = 1.000001f;
			// 4J - have removed this scale as I don't think we should need it, and have now optimised the vertex
			// shader so it doesn't do anything other than translate with this matrix anyway
	#if 0
			glTranslatef(-zs / 2.0f, -ys / 2.0f, -zs / 2.0f);
			glScalef(ss, ss, ss);
			glTranslatef(zs / 2.0f, ys / 2.0f, zs / 2.0f);
	#endif
			t->begin();
			t->offset((float)(-this->x), (float)(-this->y), (float)(-this->z));
		}

		g_rebuildDataIn.copyFromTesselator();
		intArray_SPU tesselatorArray((unsigned int*)g_rebuildDataIn.m_tesselator.m_PPUArray);
		g_rebuildDataIn.m_tesselator._array = &tesselatorArray;
		g_rebuildDataIn.m_currentLayer = currentLayer;
		g_tileCompressDataIn.setForChunk(&region, x0, y0, z0);
		RunSPURebuild();
 		g_rebuildDataOut.storeInTesselator();
		pOutData = &g_rebuildDataOut;

		if(pOutData->m_flags & ChunkRebuildData::e_flag_Rendered)
			rendered = true;

				// 4J - changed loop order here to leave y as the innermost loop for better cache performance
		for (int z = z0; z < z1; z++)
		{
			for (int x = x0; x < x1; x++)
			{
				for (int y = y0; y < y1; y++)
				{
					// 4J - get tile from those copied into our local array in earlier optimisation
					unsigned char tileId = pOutData->getTile(x,y,z);
                    if (tileId > 0 && tileId != 0xff)
					{
                        if (currentLayer == 0 && Tile::tiles[tileId] && Tile::tiles[tileId]->isEntityTile())
						{
							shared_ptr<TileEntity> et = region.getTileEntity(x, y, z);
							if (TileEntityRenderDispatcher::instance->hasRenderer(et))
							{
								renderableTileEntities.push_back(et);
							}
						}
						int flags = pOutData->getFlags(x,y,z);
						if(flags & ChunkRebuildData::e_flag_SPURenderCodeMissing)
						{

							Tile *tile = Tile::tiles[tileId];
							if (!tile) continue;
							int renderLayer = tile->getRenderLayer();

							if (renderLayer != currentLayer)
							{
	//							renderNextLayer = true;
							}
							else if (renderLayer == currentLayer)
							{
								//if(currentLayer == 0)
								//	numRenderedLayer0++;
 								rendered |= tileRenderer.tesselateInWorld(tile, x, y, z);
							}
						}
					}
				}
			}
		}


		{
			t->end();
			bounds.addBounds(t->bounds);
			glPopMatrix();
			glEndList();
			t->useCompactVertices(false);	 // 4J added
			t->offset(0, 0, 0);
		}
		if (rendered)
		{
			levelRenderer->clearGlobalChunkFlag(this->x, this->y, this->z, level, LevelRenderer::CHUNK_FLAG_EMPTY0, currentLayer);
		}
		else
		{
			// 4J - added - clear any renderer data associated with this unused list
			levelRenderer->setGlobalChunkFlag(this->x, this->y, this->z, level, LevelRenderer::CHUNK_FLAG_EMPTY0, currentLayer);
			RenderManager.CBuffClear(lists + currentLayer);
		}

	}

	if( bb )
	{
		bb->set(bounds.boundingBox[0], bounds.boundingBox[1], bounds.boundingBox[2],
			bounds.boundingBox[3], bounds.boundingBox[4], bounds.boundingBox[5]);
	}


	if(pOutData->m_flags & ChunkRebuildData::e_flag_TouchedSky)
		LevelChunk::touchedSky = true;


	// 4J - have rewritten the way that tile entities are stored globally to make it work more easily with split screen. Chunks are now
	// stored globally in the levelrenderer, in a hashmap with a special key made up from the dimension and chunk position (using same index
	// as is used for global flags)
#if 1
	int key = levelRenderer->getGlobalIndexForChunk(this->x,this->y,this->z,level);
	EnterCriticalSection(globalRenderableTileEntities_cs);
	if( renderableTileEntities.size() )
	{
		auto it = globalRenderableTileEntities->find(key);
		if( it != globalRenderableTileEntities->end() )
		{
			// We've got some renderable tile entities that we want associated with this chunk, and an existing list of things that used to be.
			// We need to flag any that we don't need any more to be removed, keep those that we do, and add any new ones

			// First pass - flag everything already existing to be removed
			for( auto it2 = it->second.begin(); it2 != it->second.end(); it2++ )
			{
				(*it2)->setRenderRemoveStage(TileEntity::e_RenderRemoveStageFlaggedAtChunk);
			}

			// Now go through the current list. If these are already in the list, then unflag the remove flag. If they aren't, then add
			for( size_t i = 0; i < renderableTileEntities.size(); i++ )
			{
				auto it2 = find( it->second.begin(), it->second.end(), renderableTileEntities[i] );
				if( it2 == it->second.end() )
				{
					(*globalRenderableTileEntities)[key].push_back(renderableTileEntities[i]);
				}
				else
				{
					(*it2)->setRenderRemoveStage(TileEntity::e_RenderRemoveStageKeep);
				}
			}
		}
		else
		{
			// Easy case - nothing already existing for this chunk. Add them all in.
			for( size_t i = 0; i < renderableTileEntities.size(); i++ )
			{
				(*globalRenderableTileEntities)[key].push_back(renderableTileEntities[i]);
			}
		}
	}
	else
	{
		// Another easy case - we don't want any renderable tile entities associated with this chunk. Flag all to be removed.
		auto it = globalRenderableTileEntities->find(key);
		if( it != globalRenderableTileEntities->end() )
		{
			for( auto it2 = it->second.begin(); it2 != it->second.end(); it2++ )
			{
				(*it2)->setRenderRemoveStage(TileEntity::e_RenderRemoveStageFlaggedAtChunk);
			}
		}
	}
	LeaveCriticalSection(globalRenderableTileEntities_cs);
#else
	// Find the removed ones:

	// 4J - original code for this section:
	/*
		Set<TileEntity> newTileEntities = new HashSet<TileEntity>();
		newTileEntities.addAll(renderableTileEntities);
		newTileEntities.removeAll(oldTileEntities);
		globalRenderableTileEntities.addAll(newTileEntities);

		oldTileEntities.removeAll(renderableTileEntities);
		globalRenderableTileEntities.removeAll(oldTileEntities);
		*/


    unordered_set<shared_ptr<TileEntity> > newTileEntities(renderableTileEntities.begin(),renderableTileEntities.end());

	auto endIt = oldTileEntities.end();
	for( unordered_set<shared_ptr<TileEntity> >::iterator it = oldTileEntities.begin(); it != endIt; it++ )
	{
		newTileEntities.erase(*it);
	}

	// 4J - newTileEntities is now renderableTileEntities with any old ones from oldTileEntitesRemoved (so just new things added)

	EnterCriticalSection(globalRenderableTileEntities_cs);
	endIt = newTileEntities.end();
	for( unordered_set<shared_ptr<TileEntity> >::iterator it = newTileEntities.begin(); it != endIt; it++ )
	{
		globalRenderableTileEntities.push_back(*it);
	}

	// 4J - All these new things added to globalRenderableTileEntities

	auto endItRTE = renderableTileEntities.end();
	for( vector<shared_ptr<TileEntity> >::iterator it = renderableTileEntities.begin(); it != endItRTE; it++ )
	{
		oldTileEntities.erase(*it);
	}
	// 4J - oldTileEntities is now the removed items
	vector<shared_ptr<TileEntity> >::iterator it = globalRenderableTileEntities->begin();
	while(  it != globalRenderableTileEntities->end() )
	{
		if( oldTileEntities.find(*it) != oldTileEntities.end() )
		{
			it = globalRenderableTileEntities->erase(it);
		}
		else
		{
			++it;
		}
	}

	LeaveCriticalSection(globalRenderableTileEntities_cs);
#endif

	// 4J - These removed items are now also removed from globalRenderableTileEntities

	if( LevelChunk::touchedSky )
	{
		levelRenderer->clearGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_NOTSKYLIT);
	}
	else
	{
		levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_NOTSKYLIT);
	}
	levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_COMPILED);
	return;

}
#endif // _PS3_


float Chunk::distanceToSqr(shared_ptr<Entity> player) const
{
	float xd = static_cast<float>(player->x - xm);
	float yd = static_cast<float>(player->y - ym);
	float zd = static_cast<float>(player->z - zm);
	return xd * xd + yd * yd + zd * zd;
}

float Chunk::squishedDistanceToSqr(shared_ptr<Entity> player)
{
	float xd = static_cast<float>(player->x - xm);
	float yd = static_cast<float>(player->y - ym) * 2;
	float zd = static_cast<float>(player->z - zm);
	return xd * xd + yd * yd + zd * zd;
}

void Chunk::reset()
{
	if( assigned )
	{
		EnterCriticalSection(&levelRenderer->m_csDirtyChunks);
		unsigned char refCount = levelRenderer->decGlobalChunkRefCount(x, y, z, level);
		assigned = false;
//		printf("\t\t [dec] refcount %d at %d, %d, %d\n",refCount,x,y,z);
		if( refCount == 0 )
		{
			int lists = levelRenderer->getGlobalIndexForChunk(x, y, z, level) * 2;
			if(lists >= 0)
			{
				lists += levelRenderer->chunkLists;
				for (int i = 0; i < 2; i++)
				{
					// 4J - added - clear any renderer data associated with this unused list
					RenderManager.CBuffClear(lists + i);
				}
				levelRenderer->setGlobalChunkFlags(x, y, z, level, 0);
			}
		}
		LeaveCriticalSection(&levelRenderer->m_csDirtyChunks);
	}

	clipChunk->visible = false;
}

void Chunk::_delete()
{
	reset();
	level = nullptr;
}

int Chunk::getList(int layer)
{
	if (!clipChunk->visible) return -1;

	int lists = levelRenderer->getGlobalIndexForChunk(x, y, z,level) * 2;
	lists += levelRenderer->chunkLists;

	bool empty =  levelRenderer->getGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_EMPTY0, layer);
	if (!empty) return lists + layer;
	return -1;
}

void Chunk::cull(Culler *culler)
{
	clipChunk->visible = culler->isVisible(bb.get());
}

void Chunk::renderBB()
{
//	glCallList(lists + 2);	// 4J - removed - TODO put back in
}

bool Chunk::isEmpty()
{
	if (!levelRenderer->getGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_COMPILED)) return false;
	return levelRenderer->getGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_EMPTYBOTH);
}

void Chunk::setDirty()
{
	// 4J - not used, but if this starts being used again then we'll need to investigate how best to handle it.
	__debugbreak();
	levelRenderer->setGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_DIRTY);
}

void Chunk::clearDirty()
{
	levelRenderer->clearGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_DIRTY);
#ifdef _CRITICAL_CHUNKS
	levelRenderer->clearGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_CRITICAL);
#endif
}

Chunk::~Chunk() = default;

bool Chunk::emptyFlagSet(int layer)
{
	return levelRenderer->getGlobalChunkFlag(x, y, z, level, LevelRenderer::CHUNK_FLAG_EMPTY0, layer);
}
