#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.locale.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.redstone.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.food.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"
#include "Tile.h"

wstring Tile::TILE_DESCRIPTION_PREFIX = L"Tile.";

const float Tile::INDESTRUCTIBLE_DESTROY_TIME = -1.0f;

Tile::SoundType *Tile::SOUND_NORMAL = nullptr;
Tile::SoundType *Tile::SOUND_WOOD = nullptr;
Tile::SoundType *Tile::SOUND_GRAVEL = nullptr;
Tile::SoundType *Tile::SOUND_GRASS = nullptr;
Tile::SoundType *Tile::SOUND_STONE = nullptr;
Tile::SoundType *Tile::SOUND_METAL = nullptr;
Tile::SoundType *Tile::SOUND_GLASS = nullptr;
Tile::SoundType *Tile::SOUND_CLOTH = nullptr;
Tile::SoundType *Tile::SOUND_SAND = nullptr;
Tile::SoundType *Tile::SOUND_SNOW = nullptr;
Tile::SoundType *Tile::SOUND_LADDER = nullptr;
Tile::SoundType *Tile::SOUND_ANVIL = nullptr;

bool Tile::solid[TILE_NUM_COUNT];
int Tile::lightBlock[TILE_NUM_COUNT];
bool Tile::transculent[TILE_NUM_COUNT];
int Tile::lightEmission[TILE_NUM_COUNT];
unsigned char Tile::_sendTileData[TILE_NUM_COUNT];		// 4J changed - was bool, now bitfield to indicate which bits are important to be sent
bool Tile::mipmapEnable[TILE_NUM_COUNT];
bool Tile::propagate[TILE_NUM_COUNT];

Tile **Tile::tiles = nullptr;

Tile *Tile::stone = nullptr;
GrassTile *Tile::grass = nullptr;
Tile *Tile::dirt = nullptr;
Tile *Tile::cobblestone = nullptr;
Tile *Tile::wood = nullptr;
Tile *Tile::sapling = nullptr;
Tile *Tile::unbreakable = nullptr;
LiquidTile *Tile::water = nullptr;
Tile *Tile::calmWater = nullptr;
LiquidTile *Tile::lava = nullptr;
Tile *Tile::calmLava = nullptr;
Tile *Tile::sand = nullptr;
Tile *Tile::gravel = nullptr;
Tile *Tile::goldOre = nullptr;
Tile *Tile::ironOre = nullptr;
Tile *Tile::coalOre = nullptr;
Tile *Tile::treeTrunk = nullptr;
LeafTile *Tile::leaves = nullptr;
Tile *Tile::sponge = nullptr;
Tile *Tile::glass = nullptr;
Tile *Tile::lapisOre = nullptr;
Tile *Tile::lapisBlock = nullptr;
Tile *Tile::dispenser = nullptr;
Tile *Tile::sandStone = nullptr;
Tile *Tile::noteblock = nullptr;
Tile *Tile::bed = nullptr;
Tile *Tile::goldenRail = nullptr;
Tile *Tile::detectorRail = nullptr;
PistonBaseTile *Tile::pistonStickyBase = nullptr;
Tile *Tile::web = nullptr;
TallGrass *Tile::tallgrass = nullptr;
DeadBushTile *Tile::deadBush = nullptr;
PistonBaseTile *Tile::pistonBase = nullptr;
PistonExtensionTile *Tile::pistonExtension = nullptr;
Tile *Tile::wool = nullptr;
PistonMovingPiece *Tile::pistonMovingPiece = nullptr;
Bush *Tile::flower = nullptr;
Bush *Tile::rose = nullptr;
Bush* Tile::blue_orchid = nullptr;
Bush *Tile::mushroom_brown = nullptr;
Bush *Tile::mushroom_red = nullptr;
Tile *Tile::goldBlock = nullptr;
Tile *Tile::ironBlock = nullptr;
HalfSlabTile *Tile::stoneSlab = nullptr;
HalfSlabTile *Tile::stoneSlabHalf = nullptr;
Tile *Tile::redBrick = nullptr;
Tile *Tile::tnt = nullptr;
Tile *Tile::bookshelf = nullptr;
Tile *Tile::mossyCobblestone = nullptr;
Tile *Tile::obsidian = nullptr;
Tile *Tile::torch = nullptr;
FireTile *Tile::fire = nullptr;
Tile *Tile::mobSpawner = nullptr;
Tile *Tile::stairs_wood = nullptr;
ChestTile *Tile::chest = nullptr;
RedStoneDustTile *Tile::redStoneDust = nullptr;
Tile *Tile::diamondOre = nullptr;
Tile *Tile::diamondBlock = nullptr;
Tile *Tile::workBench = nullptr;
Tile *Tile::wheat = nullptr;
Tile *Tile::farmland = nullptr;
Tile *Tile::furnace = nullptr;
Tile *Tile::furnace_lit = nullptr;
Tile *Tile::sign = nullptr;
Tile *Tile::door_wood = nullptr;
Tile *Tile::ladder = nullptr;
Tile *Tile::rail = nullptr;
Tile *Tile::stairs_stone = nullptr;
Tile *Tile::wallSign = nullptr;
Tile *Tile::lever = nullptr;
Tile *Tile::pressurePlate_stone = nullptr;
Tile *Tile::door_iron = nullptr;
Tile *Tile::pressurePlate_wood = nullptr;
Tile *Tile::redStoneOre = nullptr;
Tile *Tile::redStoneOre_lit = nullptr;
Tile *Tile::redstoneTorch_off = nullptr;
Tile *Tile::redstoneTorch_on = nullptr;
Tile *Tile::button = nullptr;
Tile *Tile::topSnow = nullptr;
Tile *Tile::ice = nullptr;
Tile *Tile::snow = nullptr;
Tile *Tile::cactus = nullptr;
Tile *Tile::clay = nullptr;
Tile *Tile::reeds = nullptr;
Tile *Tile::jukebox = nullptr;
Tile *Tile::fence = nullptr;
Tile *Tile::pumpkin = nullptr;
Tile *Tile::netherRack = nullptr;
Tile *Tile::soulsand = nullptr;
Tile *Tile::glowstone = nullptr;
PortalTile *Tile::portalTile = nullptr;
Tile *Tile::litPumpkin = nullptr;
Tile *Tile::cake = nullptr;
RepeaterTile *Tile::diode_off = nullptr;
RepeaterTile *Tile::diode_on = nullptr;
Tile *Tile::stained_glass = nullptr;
Tile *Tile::trapdoor = nullptr;

Tile *Tile::monsterStoneEgg = nullptr;
Tile *Tile::stoneBrick = nullptr;
Tile *Tile::hugeMushroom_brown = nullptr;
Tile *Tile::hugeMushroom_red = nullptr;
Tile *Tile::ironFence = nullptr;
Tile *Tile::thinGlass = nullptr;
Tile *Tile::melon = nullptr;
Tile *Tile::pumpkinStem = nullptr;
Tile *Tile::melonStem = nullptr;
Tile *Tile::vine = nullptr;
Tile *Tile::fenceGate = nullptr;
Tile *Tile::stairs_bricks = nullptr;
Tile *Tile::stairs_stoneBrickSmooth = nullptr;

MycelTile *Tile::mycel = nullptr;
Tile *Tile::waterLily = nullptr;
Tile *Tile::netherBrick = nullptr;
Tile *Tile::netherFence = nullptr;
Tile *Tile::stairs_netherBricks = nullptr;
Tile *Tile::netherStalk = nullptr;
Tile *Tile::enchantTable = nullptr;
Tile *Tile::brewingStand = nullptr;
CauldronTile *Tile::cauldron = nullptr;
Tile *Tile::endPortalTile = nullptr;
Tile *Tile::endPortalFrameTile = nullptr;
Tile *Tile::endStone = nullptr;
Tile *Tile::dragonEgg = nullptr;
Tile *Tile::redstoneLight = nullptr;
Tile *Tile::redstoneLight_lit = nullptr;

// TU9
Tile *Tile::stairs_sandstone = nullptr;
Tile *Tile::woodStairsDark = nullptr;
Tile *Tile::woodStairsBirch = nullptr;
Tile *Tile::woodStairsJungle = nullptr;
Tile *Tile::commandBlock = nullptr;
BeaconTile *Tile::beacon = nullptr;
Tile *Tile::button_wood = nullptr;
HalfSlabTile *Tile::woodSlab = nullptr;
HalfSlabTile *Tile::woodSlabHalf = nullptr;

Tile *Tile::emeraldOre = nullptr;
Tile *Tile::enderChest = nullptr;
TripWireSourceTile *Tile::tripWireSource = nullptr;
Tile *Tile::tripWire = nullptr;
Tile *Tile::emeraldBlock = nullptr;


Tile *Tile::cocoa = nullptr;
Tile *Tile::skull = nullptr;

Tile *Tile::cobbleWall = nullptr;
Tile *Tile::flowerPot = nullptr;
Tile *Tile::carrots = nullptr;
Tile *Tile::potatoes = nullptr;
Tile *Tile::anvil = nullptr;
Tile *Tile::chest_trap = nullptr;
Tile *Tile::weightedPlate_light = nullptr;
Tile *Tile::weightedPlate_heavy = nullptr;
ComparatorTile *Tile::comparator_off = nullptr;
ComparatorTile *Tile::comparator_on = nullptr;

DaylightDetectorTile *Tile::daylightDetector = nullptr;
Tile *Tile::redstoneBlock = nullptr;

Tile *Tile::netherQuartz = nullptr;
HopperTile *Tile::hopper = nullptr;
Tile *Tile::quartzBlock = nullptr;
Tile *Tile::stairs_quartz = nullptr;
Tile *Tile::activatorRail = nullptr;
Tile *Tile::dropper = nullptr;
Tile *Tile::clayHardened_colored = nullptr;
Tile *Tile::stained_glass_pane = nullptr;

Tile *Tile::hayBlock = nullptr;
Tile *Tile::woolCarpet = nullptr;
Tile *Tile::clayHardened = nullptr;
Tile *Tile::coalBlock = nullptr;

Tile *Tile::lightSourceBlock = nullptr;

DWORD Tile::tlsIdxShape = TlsAlloc();

Tile::ThreadStorage::ThreadStorage()
{
	xx0 = yy0 = zz0 = xx1 = yy1 = zz1 = 0.0;
	tileId = 0;
}

void Tile::CreateNewThreadStorage()
{
	ThreadStorage *tls = new ThreadStorage();
	TlsSetValue(Tile::tlsIdxShape, tls);
}

void Tile::ReleaseThreadStorage()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	delete tls;
}

void Tile::staticCtor()
{
#define NEW_TILE(class, id, ...)   (new class(id, ##__VA_ARGS__))
#define DESTROY(x)                  ->setDestroyTime(x)
#define EXPLODE(x)                  ->setExplodeable(x)
#define SOUND(s)                    ->setSoundType(Tile::SOUND_##s)
#define ICON(name)                  ->setIconName(L##name)
#define DESC(desc)                  ->setDescriptionId(desc)
#define USEDESC(usedesc)            ->setUseDescriptionId(usedesc)
#define BASE(type, mat)              ->setBaseItemTypeAndMaterial(Item::eBaseItemType_##type, Item::eMaterial_##mat)
#define DATA(mask)                   ->sendTileData(mask)
#define LIGHTBLOCK(x)                ->setLightBlock(x)
#define LIGHTEMISSION(x)             ->setLightEmission(x)
#define NOMIPMAP                     ->disableMipmap()
#define NOTCOLLECT                    ->setNotCollectStatistics()
#define TICKING                       ->setTicking(true)
#define INDESTRUCTIBLE                 ->setIndestructible()
// ------------------------------------------

	Tile::SOUND_NORMAL = new Tile::SoundType(eMaterialSoundType_STONE, 1, 1);
	Tile::SOUND_WOOD = new Tile::SoundType(eMaterialSoundType_WOOD, 1, 1);
	Tile::SOUND_GRAVEL = new Tile::SoundType(eMaterialSoundType_GRAVEL, 1, 1);
	Tile::SOUND_GRASS = new Tile::SoundType(eMaterialSoundType_GRASS, 1, 1);
	Tile::SOUND_STONE = new Tile::SoundType(eMaterialSoundType_STONE, 1, 1);
	Tile::SOUND_METAL = new Tile::SoundType(eMaterialSoundType_STONE, 1, 1.5f);
	Tile::SOUND_GLASS = new Tile::SoundType(eMaterialSoundType_STONE, 1, 1, eSoundType_RANDOM_GLASS, eSoundType_STEP_STONE);
	Tile::SOUND_CLOTH = new Tile::SoundType(eMaterialSoundType_CLOTH, 1, 1);
	Tile::SOUND_SAND = new Tile::SoundType(eMaterialSoundType_SAND, 1, 1);
	Tile::SOUND_SNOW = new Tile::SoundType(eMaterialSoundType_SNOW, 1, 1);
	Tile::SOUND_LADDER = new Tile::SoundType(eMaterialSoundType_LADDER, 1, 1, eSoundType_DIG_WOOD);
	Tile::SOUND_ANVIL = new Tile::SoundType(eMaterialSoundType_ANVIL, 0.3f, 1, eSoundType_DIG_STONE, eSoundType_RANDOM_ANVIL_LAND);

	Tile::tiles = new Tile * [TILE_NUM_COUNT];
	memset(tiles, 0, sizeof(Tile*) * TILE_NUM_COUNT);

	Tile::stone = NEW_TILE(StoneTile, stone_Id)
		BASE(structblock, stone)
		DESTROY(1.5f) EXPLODE(10) SOUND(STONE) ICON("stone") DESC(IDS_TILE_STONE) USEDESC(IDS_DESC_STONE);

	Tile::grass = static_cast<GrassTile*>(NEW_TILE(GrassTile, grass_Id)
		DESTROY(0.6f) SOUND(GRASS) ICON("grass") DESC(IDS_TILE_GRASS) USEDESC(IDS_DESC_GRASS));

	Tile::dirt = NEW_TILE(DirtTile, dirt_Id)
		DESTROY(0.5f) SOUND(GRAVEL) ICON("dirt") DESC(IDS_TILE_DIRT) USEDESC(IDS_DESC_DIRT);

	Tile::cobblestone = NEW_TILE(Tile, cobblestone_Id, Material::stone)
		BASE(structblock, stone)
		DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("cobblestone") DESC(IDS_TILE_STONE_BRICK) USEDESC(IDS_DESC_STONE_BRICK);

	Tile::wood = static_cast<WoodTile*>(NEW_TILE(WoodTile, wood_Id)
		BASE(structwoodstuff, wood)
		DESTROY(2.0f) EXPLODE(5) SOUND(WOOD) ICON("planks") DESC(IDS_TILE_OAKWOOD_PLANKS) USEDESC(IDS_DESC_WOODENPLANKS) DATA(15));

	Tile::sapling = static_cast<Sapling*>(NEW_TILE(Sapling, sapling_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("sapling") DESC(IDS_TILE_SAPLING) USEDESC(IDS_DESC_SAPLING) DATA(15) NOMIPMAP);

	Tile::unbreakable = NEW_TILE(Tile, unbreakable_Id, Material::stone)
		INDESTRUCTIBLE EXPLODE(6000000) SOUND(STONE) ICON("bedrock") DESC(IDS_TILE_BEDROCK) NOTCOLLECT USEDESC(IDS_DESC_BEDROCK);

	Tile::water = static_cast<LiquidTile*>(NEW_TILE(LiquidTileDynamic, water_Id, Material::water)
		DESTROY(100.0f) LIGHTBLOCK(3) ICON("water_flow") DESC(IDS_TILE_WATER) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_WATER));

	Tile::calmWater = NEW_TILE(LiquidTileStatic, calmWater_Id, Material::water)
		DESTROY(100.0f) LIGHTBLOCK(3) ICON("water_still") DESC(IDS_TILE_WATER) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_WATER);

	Tile::lava = static_cast<LiquidTile*>(NEW_TILE(LiquidTileDynamic, lava_Id, Material::lava)
		DESTROY(0.0f) LIGHTEMISSION(1.0f) LIGHTBLOCK(255) ICON("lava_flow") DESC(IDS_TILE_LAVA) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_LAVA));

	Tile::calmLava = NEW_TILE(LiquidTileStatic, calmLava_Id, Material::lava)
		DESTROY(100.0f) LIGHTEMISSION(1.0f) LIGHTBLOCK(255) ICON("lava_still") DESC(IDS_TILE_LAVA) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_LAVA);

	Tile::sand = NEW_TILE(HeavyTile, sand_Id)
		DESTROY(0.5f) SOUND(SAND) ICON("sand") DESC(IDS_TILE_SAND) USEDESC(IDS_DESC_SAND);

	Tile::gravel = NEW_TILE(GravelTile, gravel_Id)
		DESTROY(0.6f) SOUND(GRAVEL) ICON("gravel") DESC(IDS_TILE_GRAVEL) USEDESC(IDS_DESC_GRAVEL);

	Tile::goldOre = NEW_TILE(OreTile, goldOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("gold_ore") DESC(IDS_TILE_ORE_GOLD) USEDESC(IDS_DESC_ORE_GOLD);

	Tile::ironOre = NEW_TILE(OreTile, ironOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("iron_ore") DESC(IDS_TILE_ORE_IRON) USEDESC(IDS_DESC_ORE_IRON);

	Tile::coalOre = NEW_TILE(OreTile, coalOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("coal_ore") DESC(IDS_TILE_ORE_COAL) USEDESC(IDS_DESC_ORE_COAL);

	Tile::treeTrunk = NEW_TILE(TreeTile, treeTrunk_Id)
		DESTROY(2.0f) SOUND(WOOD) ICON("log") DESC(IDS_TILE_LOG) DATA(15) USEDESC(IDS_DESC_LOG);

	Tile::leaves = static_cast<LeafTile*>(NEW_TILE(LeafTile, leaves_Id)
		DESTROY(0.2f) LIGHTBLOCK(1) SOUND(GRASS) ICON("leaves") DESC(IDS_TILE_LEAVES) DATA(LeafTile::LEAF_TYPE_MASK) USEDESC(IDS_DESC_LEAVES));

	Tile::sponge = NEW_TILE(Sponge, sponge_Id)
		DESTROY(0.6f) SOUND(GRASS) ICON("sponge") DESC(IDS_TILE_SPONGE) USEDESC(IDS_DESC_SPONGE);

	Tile::glass = NEW_TILE(GlassTile, glass_Id, Material::glass, false)
		DESTROY(0.3f) SOUND(GLASS) ICON("glass") DESC(IDS_TILE_GLASS) USEDESC(IDS_DESC_GLASS);

	Tile::lapisOre = NEW_TILE(OreTile, lapisOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("lapis_ore") DESC(IDS_TILE_ORE_LAPIS) USEDESC(IDS_DESC_ORE_LAPIS);

	Tile::lapisBlock = NEW_TILE(Tile, lapisBlock_Id, Material::stone)
		BASE(block, lapis) DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("lapis_block") DESC(IDS_TILE_BLOCK_LAPIS) USEDESC(IDS_DESC_BLOCK_LAPIS);

	Tile::dispenser = NEW_TILE(DispenserTile, dispenser_Id)
		BASE(redstoneContainer, undefined) DESTROY(3.5f) SOUND(STONE) ICON("dispenser") DESC(IDS_TILE_DISPENSER) DATA(15) USEDESC(IDS_DESC_DISPENSER);

	Tile::sandStone = NEW_TILE(SandStoneTile, sandStone_Id)
		BASE(structblock, sand) SOUND(STONE) DESTROY(0.8f) DATA(15) ICON("sandstone") DESC(IDS_TILE_SANDSTONE) USEDESC(IDS_DESC_SANDSTONE) DATA(15); // дважды DATA? оставим как было

	Tile::noteblock = NEW_TILE(NoteBlockTile, noteblock_Id)
		DESTROY(0.8f) ICON("noteblock") DESC(IDS_TILE_MUSIC_BLOCK) DATA(15) USEDESC(IDS_DESC_NOTEBLOCK);

	Tile::bed = NEW_TILE(BedTile, bed_Id)
		DESTROY(0.2f) ICON("bed") DESC(IDS_TILE_BED) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_BED);

	Tile::goldenRail = NEW_TILE(PoweredRailTile, goldenRail_Id)
		BASE(rail, gold) DESTROY(0.7f) SOUND(METAL) ICON("rail_golden") DESC(IDS_TILE_GOLDEN_RAIL) DATA(15) USEDESC(IDS_DESC_POWEREDRAIL) NOMIPMAP;

	Tile::detectorRail = NEW_TILE(DetectorRailTile, detectorRail_Id)
		BASE(rail, detector) DESTROY(0.7f) SOUND(METAL) ICON("rail_detector") DESC(IDS_TILE_DETECTOR_RAIL) DATA(15) USEDESC(IDS_DESC_DETECTORRAIL) NOMIPMAP;

	Tile::pistonStickyBase = static_cast<PistonBaseTile*>(NEW_TILE(PistonBaseTile, pistonStickyBase_Id, true)
		BASE(piston, stickypiston) ICON("pistonStickyBase") DESC(IDS_TILE_PISTON_STICK_BASE) USEDESC(IDS_DESC_STICKY_PISTON) DATA(15));

	Tile::web = NEW_TILE(WebTile, web_Id)
		LIGHTBLOCK(1) DESTROY(4.0f) ICON("web") DESC(IDS_TILE_WEB) USEDESC(IDS_DESC_WEB);

	Tile::tallgrass = static_cast<TallGrass*>(NEW_TILE(TallGrass, tallgrass_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("tallgrass") DESC(IDS_TILE_TALL_GRASS) USEDESC(IDS_DESC_TALL_GRASS) NOMIPMAP);

	Tile::deadBush = static_cast<DeadBushTile*>(NEW_TILE(DeadBushTile, deadBush_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("deadbush") DESC(IDS_TILE_DEAD_BUSH) USEDESC(IDS_DESC_DEAD_BUSH) NOMIPMAP);

	Tile::pistonBase = static_cast<PistonBaseTile*>(NEW_TILE(PistonBaseTile, pistonBase_Id, false)
		BASE(piston, piston) ICON("pistonBase") DESC(IDS_TILE_PISTON_BASE) USEDESC(IDS_DESC_PISTON) DATA(15));

	Tile::pistonExtension = static_cast<PistonExtensionTile*>(NEW_TILE(PistonExtensionTile, pistonExtensionPiece_Id)
		DESC(IDS_TILE_PISTON_BASE) USEDESC(-1) DATA(15));

	Tile::wool = static_cast<ColoredTile*>(NEW_TILE(ColoredTile, wool_Id, Material::cloth)
		BASE(cloth, cloth) DESTROY(0.8f) SOUND(CLOTH) ICON("wool_colored") DESC(IDS_TILE_CLOTH) DATA(15) USEDESC(IDS_DESC_WOOL));

	Tile::pistonMovingPiece = static_cast<PistonMovingPiece*>(NEW_TILE(PistonMovingPiece, pistonMovingPiece_Id)
		DESC(IDS_TILE_PISTON_BASE) USEDESC(-1));

	Tile::flower = static_cast<Bush*>(NEW_TILE(Bush, flower_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("flower_dandelion") DESC(IDS_TILE_FLOWER) USEDESC(IDS_DESC_FLOWER) NOMIPMAP);

	Tile::rose = static_cast<Bush*>(NEW_TILE(Bush, rose_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("flower_rose") DESC(IDS_TILE_ROSE) USEDESC(IDS_DESC_FLOWER) NOMIPMAP);
	
	Tile::blue_orchid = static_cast<Bush*>(NEW_TILE(Bush, blue_orchid_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("blue_orchid") DESC(IDS_TILE_BLUE_ORCHID) USEDESC(IDS_DESC_FLOWER) NOMIPMAP);

	Tile::mushroom_brown = static_cast<Bush*>(NEW_TILE(Mushroom, mushroom_brown_Id)
		DESTROY(0.0f) SOUND(GRASS) LIGHTEMISSION(2 / 16.0f) ICON("mushroom_brown") DESC(IDS_TILE_MUSHROOM) USEDESC(IDS_DESC_MUSHROOM) NOMIPMAP);

	Tile::mushroom_red = static_cast<Bush*>(NEW_TILE(Mushroom, mushroom_red_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("mushroom_red") DESC(IDS_TILE_MUSHROOM) USEDESC(IDS_DESC_MUSHROOM) NOMIPMAP);

	Tile::goldBlock = NEW_TILE(MetalTile, goldBlock_Id)
		BASE(block, gold) DESTROY(3.0f) EXPLODE(10) SOUND(METAL) ICON("gold_block") DESC(IDS_TILE_BLOCK_GOLD) USEDESC(IDS_DESC_BLOCK_GOLD);

	Tile::ironBlock = NEW_TILE(MetalTile, ironBlock_Id)
		BASE(block, iron) DESTROY(5.0f) EXPLODE(10) SOUND(METAL) ICON("iron_block") DESC(IDS_TILE_BLOCK_IRON) USEDESC(IDS_DESC_BLOCK_IRON);

	Tile::stoneSlab = static_cast<HalfSlabTile*>(NEW_TILE(StoneSlabTile, stoneSlab_Id, true)
		BASE(slab, stone) DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("stoneSlab") DESC(IDS_TILE_STONESLAB) USEDESC(IDS_DESC_SLAB));

	Tile::stoneSlabHalf = static_cast<HalfSlabTile*>(NEW_TILE(StoneSlabTile, stoneSlabHalf_Id, false)
		BASE(halfslab, stone) DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("stoneSlab") DESC(IDS_TILE_STONESLAB) USEDESC(IDS_DESC_HALFSLAB));

	Tile::redBrick = NEW_TILE(Tile, redBrick_Id, Material::stone)
		BASE(structblock, brick) DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("brick") DESC(IDS_TILE_BRICK) USEDESC(IDS_DESC_BRICK);

	Tile::tnt = NEW_TILE(TntTile, tnt_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("tnt") DESC(IDS_TILE_TNT) USEDESC(IDS_DESC_TNT);

	Tile::bookshelf = NEW_TILE(BookshelfTile, bookshelf_Id)
		BASE(paper, bookshelf) DESTROY(1.5f) SOUND(WOOD) ICON("bookshelf") DESC(IDS_TILE_BOOKSHELF) USEDESC(IDS_DESC_BOOKSHELF);

	Tile::mossyCobblestone = NEW_TILE(Tile, mossyCobblestone_Id, Material::stone)
		DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("cobblestone_mossy") DESC(IDS_TILE_STONE_MOSS) USEDESC(IDS_DESC_MOSS_STONE);

	Tile::obsidian = NEW_TILE(ObsidianTile, obsidian_Id)
		DESTROY(50.0f) EXPLODE(2000) SOUND(STONE) ICON("obsidian") DESC(IDS_TILE_OBSIDIAN) USEDESC(IDS_DESC_OBSIDIAN);

	Tile::torch = NEW_TILE(TorchTile, torch_Id)
		BASE(torch, wood) DESTROY(0.0f) LIGHTEMISSION(15 / 16.0f) SOUND(WOOD) ICON("torch_on") DESC(IDS_TILE_TORCH) USEDESC(IDS_DESC_TORCH) NOMIPMAP;

	Tile::fire = static_cast<FireTile*>((NEW_TILE(FireTile, fire_Id) DESTROY(0.0f) LIGHTEMISSION(1.0f) SOUND(WOOD))->setIconName(L"fire")->setDescriptionId(IDS_TILE_FIRE)->setNotCollectStatistics()->setUseDescriptionId(-1));

	Tile::mobSpawner = NEW_TILE(MobSpawnerTile, mobSpawner_Id)
		DESTROY(5.0f) SOUND(METAL) ICON("mob_spawner") DESC(IDS_TILE_MOB_SPAWNER) NOTCOLLECT USEDESC(IDS_DESC_MOB_SPAWNER);

	Tile::stairs_wood = NEW_TILE(StairTile, stairs_wood_Id, Tile::wood, 0)
		BASE(stairs, wood) ICON("stairsWood") DESC(IDS_TILE_STAIRS_WOOD) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::chest = static_cast<ChestTile*>(NEW_TILE(ChestTile, chest_Id, ChestTile::TYPE_BASIC)
		BASE(chest, wood) DESTROY(2.5f) SOUND(WOOD) ICON("chest") DESC(IDS_TILE_CHEST) DATA(15) USEDESC(IDS_DESC_CHEST));

	Tile::redStoneDust = static_cast<RedStoneDustTile*>(NEW_TILE(RedStoneDustTile, redStoneDust_Id)
		DESTROY(0.0f) SOUND(NORMAL) ICON("redstone_dust") DESC(IDS_TILE_REDSTONE_DUST) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_REDSTONE_DUST));

	Tile::diamondOre = NEW_TILE(OreTile, diamondOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("diamond_ore") DESC(IDS_TILE_ORE_DIAMOND) USEDESC(IDS_DESC_ORE_DIAMOND);

	Tile::diamondBlock = NEW_TILE(MetalTile, diamondBlock_Id)
		BASE(block, diamond) DESTROY(5.0f) EXPLODE(10) SOUND(METAL) ICON("diamond_block") DESC(IDS_TILE_BLOCK_DIAMOND) USEDESC(IDS_DESC_BLOCK_DIAMOND);

	Tile::workBench = NEW_TILE(WorkbenchTile, workBench_Id)
		BASE(device, wood) DESTROY(2.5f) SOUND(WOOD) ICON("crafting_table") DESC(IDS_TILE_WORKBENCH) USEDESC(IDS_DESC_CRAFTINGTABLE);

	Tile::wheat = NEW_TILE(CropTile, wheat_Id)
		ICON("wheat") DESC(IDS_TILE_CROPS) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_CROPS) NOMIPMAP;

	Tile::farmland = NEW_TILE(FarmTile, farmland_Id)
		DESTROY(0.6f) SOUND(GRAVEL) ICON("farmland") DESC(IDS_TILE_FARMLAND) USEDESC(IDS_DESC_FARMLAND) DATA(15);

	Tile::furnace = NEW_TILE(FurnaceTile, furnace_Id, false)
		BASE(device, stone) DESTROY(3.5f) SOUND(STONE) ICON("furnace") DESC(IDS_TILE_FURNACE) DATA(15) USEDESC(IDS_DESC_FURNACE);

	Tile::furnace_lit = NEW_TILE(FurnaceTile, furnace_lit_Id, true)
		DESTROY(3.5f) SOUND(STONE) LIGHTEMISSION(14 / 16.0f) ICON("furnace") DESC(IDS_TILE_FURNACE) DATA(15) USEDESC(IDS_DESC_FURNACE);

	Tile::sign = NEW_TILE(SignTile, sign_Id, eTYPE_SIGNTILEENTITY, true)
		DESTROY(1.0f) SOUND(WOOD) ICON("sign") DESC(IDS_TILE_SIGN) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_SIGN);

	Tile::door_wood = NEW_TILE(DoorTile, door_wood_Id, Material::wood)
		DESTROY(3.0f) SOUND(WOOD) ICON("door_wood") DESC(IDS_TILE_DOOR_WOOD) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_DOOR_WOOD);

	Tile::ladder = NEW_TILE(LadderTile, ladder_Id)
		DESTROY(0.4f) SOUND(LADDER) ICON("ladder") DESC(IDS_TILE_LADDER) DATA(15) USEDESC(IDS_DESC_LADDER) NOMIPMAP;

	Tile::rail = NEW_TILE(RailTile, rail_Id)
		BASE(rail, iron) DESTROY(0.7f) SOUND(METAL) ICON("rail_normal") DESC(IDS_TILE_RAIL) DATA(15) USEDESC(IDS_DESC_RAIL) NOMIPMAP;

	Tile::stairs_stone = NEW_TILE(StairTile, stairs_stone_Id, Tile::cobblestone, 0)
		BASE(stairs, stone) ICON("stairsStone") DESC(IDS_TILE_STAIRS_STONE) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::wallSign = NEW_TILE(SignTile, wallSign_Id, eTYPE_SIGNTILEENTITY, false)
		DESTROY(1.0f) SOUND(WOOD) ICON("sign") DESC(IDS_TILE_SIGN) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_SIGN);

	Tile::lever = NEW_TILE(LeverTile, lever_Id)
		BASE(lever, wood) DESTROY(0.5f) SOUND(WOOD) ICON("lever") DESC(IDS_TILE_LEVER) DATA(15) USEDESC(IDS_DESC_LEVER);

	Tile::pressurePlate_stone = static_cast<Tile*>(NEW_TILE(PressurePlateTile, pressurePlate_stone_Id, L"stone", Material::stone, PressurePlateTile::mobs)
		BASE(pressureplate, stone) DESTROY(0.5f) SOUND(STONE) DESC(IDS_TILE_PRESSURE_PLATE) DATA(15) USEDESC(IDS_DESC_PRESSUREPLATE));

	Tile::door_iron = NEW_TILE(DoorTile, door_iron_Id, Material::metal)
		DESTROY(5.0f) SOUND(METAL) ICON("door_iron") DESC(IDS_TILE_DOOR_IRON) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_DOOR_IRON);

	Tile::pressurePlate_wood = NEW_TILE(PressurePlateTile, pressurePlate_wood_Id, L"planks_oak", Material::wood, PressurePlateTile::everything)
		BASE(pressureplate, wood) DESTROY(0.5f) SOUND(WOOD) DESC(IDS_TILE_PRESSURE_PLATE) DATA(15) USEDESC(IDS_DESC_PRESSUREPLATE);

	Tile::redStoneOre = NEW_TILE(RedStoneOreTile, redStoneOre_Id, false)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("redstone_ore") DESC(IDS_TILE_ORE_REDSTONE) DATA(15) USEDESC(IDS_DESC_ORE_REDSTONE);

	Tile::redStoneOre_lit = NEW_TILE(RedStoneOreTile, redStoneOre_lit_Id, true)
		LIGHTEMISSION(10 / 16.0f) DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("redstone_ore") DESC(IDS_TILE_ORE_REDSTONE) DATA(15) USEDESC(IDS_DESC_ORE_REDSTONE);

	Tile::redstoneTorch_off = NEW_TILE(NotGateTile, redstoneTorch_off_Id, false)
		DESTROY(0.0f) SOUND(WOOD) ICON("redstone_torch_off") DESC(IDS_TILE_NOT_GATE) DATA(15) USEDESC(IDS_DESC_REDSTONETORCH) NOMIPMAP;

	Tile::redstoneTorch_on = NEW_TILE(NotGateTile, redstoneTorch_on_Id, true)
		DESTROY(0.0f) LIGHTEMISSION(8 / 16.0f) SOUND(WOOD) ICON("redstone_torch_on") DESC(IDS_TILE_NOT_GATE) DATA(15) USEDESC(IDS_DESC_REDSTONETORCH) NOMIPMAP;

	Tile::button = NEW_TILE(StoneButtonTile, button_stone_Id)
		BASE(button, stone) DESTROY(0.5f) SOUND(STONE) ICON("button") DESC(IDS_TILE_BUTTON) DATA(15) USEDESC(IDS_DESC_BUTTON);

	Tile::topSnow = NEW_TILE(TopSnowTile, topSnow_Id)
		BASE(structblock, snow) DESTROY(0.1f) SOUND(SNOW) ICON("snow") DESC(IDS_TILE_SNOW) USEDESC(IDS_DESC_TOP_SNOW) DATA(15) LIGHTBLOCK(0);

	Tile::ice = NEW_TILE(IceTile, ice_Id)
		DESTROY(0.5f) LIGHTBLOCK(3) SOUND(GLASS) ICON("ice") DESC(IDS_TILE_ICE) USEDESC(IDS_DESC_ICE);

	Tile::snow = NEW_TILE(SnowTile, snow_Id)
		BASE(structblock, snow) DESTROY(0.2f) SOUND(CLOTH) ICON("snow") DESC(IDS_TILE_SNOW) USEDESC(IDS_DESC_SNOW);

	Tile::cactus = NEW_TILE(CactusTile, cactus_Id)
		DESTROY(0.4f) SOUND(CLOTH) ICON("cactus") DESC(IDS_TILE_CACTUS) USEDESC(IDS_DESC_CACTUS) NOMIPMAP;

	Tile::clay = NEW_TILE(ClayTile, clay_Id)
		BASE(structblock, clay) DESTROY(0.6f) SOUND(GRAVEL) ICON("clay") DESC(IDS_TILE_CLAY) USEDESC(IDS_DESC_CLAY_TILE);

	Tile::reeds = NEW_TILE(ReedTile, reeds_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("reeds") DESC(IDS_TILE_REEDS) NOTCOLLECT USEDESC(IDS_DESC_REEDS) NOMIPMAP;

	Tile::jukebox = NEW_TILE(JukeboxTile, jukebox_Id)
		DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("jukebox") DESC(IDS_TILE_JUKEBOX) DATA(15) USEDESC(IDS_DESC_JUKEBOX);

	Tile::fence = NEW_TILE(FenceTile, fence_Id, L"planks_oak", Material::wood)
		BASE(fence, wood) DESTROY(2.0f) EXPLODE(5) SOUND(WOOD) DESC(IDS_TILE_FENCE) USEDESC(IDS_DESC_FENCE);

	Tile::pumpkin = NEW_TILE(PumpkinTile, pumpkin_Id, false)
		DESTROY(1.0f) SOUND(WOOD) ICON("pumpkin") DESC(IDS_TILE_PUMPKIN) DATA(15) USEDESC(IDS_DESC_PUMPKIN);

	Tile::netherRack = NEW_TILE(NetherrackTile, netherRack_Id)
		DESTROY(0.4f) SOUND(STONE) ICON("netherrack") DESC(IDS_TILE_HELL_ROCK) USEDESC(IDS_DESC_HELL_ROCK);

	Tile::soulsand = NEW_TILE(SoulSandTile, soulsand_Id)
		DESTROY(0.5f) SOUND(SAND) ICON("soul_sand") DESC(IDS_TILE_HELL_SAND) USEDESC(IDS_DESC_HELL_SAND);

	Tile::glowstone = NEW_TILE(Glowstonetile, glowstone_Id, Material::glass)
		BASE(torch, glowstone) DESTROY(0.3f) SOUND(GLASS) LIGHTEMISSION(1.0f) ICON("glowstone") DESC(IDS_TILE_LIGHT_GEM) USEDESC(IDS_DESC_GLOWSTONE);

	Tile::portalTile = static_cast<PortalTile*>(NEW_TILE(PortalTile, portalTile_Id)
		DESTROY(-1) SOUND(GLASS) LIGHTEMISSION(0.75f) ICON("portal") DESC(IDS_TILE_PORTAL) USEDESC(IDS_DESC_PORTAL));

	Tile::litPumpkin = NEW_TILE(PumpkinTile, litPumpkin_Id, true)
		BASE(torch, pumpkin) DESTROY(1.0f) SOUND(WOOD) LIGHTEMISSION(1.0f) ICON("pumpkin") DESC(IDS_TILE_LIT_PUMPKIN) DATA(15) USEDESC(IDS_DESC_JACKOLANTERN);

	Tile::cake = NEW_TILE(CakeTile, cake_Id)
		DESTROY(0.5f) SOUND(CLOTH) ICON("cake") DESC(IDS_TILE_CAKE) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_CAKE);

	Tile::diode_off = static_cast<RepeaterTile*>(NEW_TILE(RepeaterTile, diode_off_Id, false)
		DESTROY(0.0f) SOUND(WOOD) ICON("repeater_off") DESC(IDS_ITEM_DIODE) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_REDSTONEREPEATER) NOMIPMAP);

	Tile::diode_on = static_cast<RepeaterTile*>(NEW_TILE(RepeaterTile, diode_on_Id, true)
		DESTROY(0.0f) LIGHTEMISSION(10 / 16.0f) SOUND(WOOD) ICON("repeater_on") DESC(IDS_ITEM_DIODE) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_REDSTONEREPEATER) NOMIPMAP);

	Tile::stained_glass = NEW_TILE(StainedGlassBlock, stained_glass_Id, Material::glass)
		BASE(glass, glass) DESTROY(0.3f) SOUND(GLASS) ICON("glass") DESC(IDS_TILE_STAINED_GLASS) USEDESC(IDS_DESC_STAINED_GLASS);

	Tile::trapdoor = NEW_TILE(TrapDoorTile, trapdoor_Id, Material::wood)
		BASE(door, trap) DESTROY(3.0f) SOUND(WOOD) ICON("trapdoor") DESC(IDS_TILE_TRAPDOOR) NOTCOLLECT DATA(15) USEDESC(IDS_DESC_TRAPDOOR);

	Tile::monsterStoneEgg = NEW_TILE(StoneMonsterTile, monsterStoneEgg_Id)
		DESTROY(0.75f) ICON("monsterStoneEgg") DESC(IDS_TILE_STONE_SILVERFISH) USEDESC(IDS_DESC_STONE_SILVERFISH);

	Tile::stoneBrick = NEW_TILE(SmoothStoneBrickTile, stoneBrick_Id)
		BASE(structblock, stoneSmooth) DESTROY(1.5f) EXPLODE(10) SOUND(STONE) ICON("stonebrick") DESC(IDS_TILE_STONE_BRICK_SMOOTH) USEDESC(IDS_DESC_STONE_BRICK_SMOOTH);

	Tile::hugeMushroom_brown = NEW_TILE(HugeMushroomTile, hugeMushroom_brown_Id, Material::wood, HugeMushroomTile::MUSHROOM_TYPE_BROWN)
		DESTROY(0.2f) SOUND(WOOD) ICON("mushroom_block") DESC(IDS_TILE_HUGE_MUSHROOM_1) USEDESC(IDS_DESC_MUSHROOM) DATA(15);

	Tile::hugeMushroom_red = NEW_TILE(HugeMushroomTile, hugeMushroom_red_Id, Material::wood, HugeMushroomTile::MUSHROOM_TYPE_RED)
		DESTROY(0.2f) SOUND(WOOD) ICON("mushroom_block") DESC(IDS_TILE_HUGE_MUSHROOM_2) USEDESC(IDS_DESC_MUSHROOM) DATA(15);

	Tile::ironFence = NEW_TILE(ThinFenceTile, ironFence_Id, L"iron_bars", L"iron_bars", Material::metal, true)
		BASE(fence, iron) DESTROY(5.0f) EXPLODE(10) SOUND(METAL) DESC(IDS_TILE_IRON_FENCE) USEDESC(IDS_DESC_IRON_FENCE);

	Tile::thinGlass = NEW_TILE(ThinFenceTile, thinGlass_Id, L"glass", L"glass_pane_top", Material::glass, false)
		DESTROY(0.3f) SOUND(GLASS) DESC(IDS_TILE_THIN_GLASS) USEDESC(IDS_DESC_THIN_GLASS);

	Tile::melon = NEW_TILE(MelonTile, melon_Id)
		DESTROY(1.0f) SOUND(WOOD) ICON("melon") DESC(IDS_TILE_MELON) USEDESC(IDS_DESC_MELON_BLOCK);

	Tile::pumpkinStem = NEW_TILE(StemTile, pumpkinStem_Id, Tile::pumpkin)
		DESTROY(0.0f) SOUND(WOOD) ICON("pumpkin_stem") DESC(IDS_TILE_PUMPKIN_STEM) DATA(15);

	Tile::melonStem = NEW_TILE(StemTile, melonStem_Id, Tile::melon)
		DESTROY(0.0f) SOUND(WOOD) ICON("melon_stem") DESC(IDS_TILE_MELON_STEM) DATA(15);

	Tile::vine = NEW_TILE(VineTile, vine_Id)
		DESTROY(0.2f) SOUND(GRASS) ICON("vine") DESC(IDS_TILE_VINE) USEDESC(IDS_DESC_VINE) DATA(15);

	Tile::fenceGate = NEW_TILE(FenceGateTile, fenceGate_Id)
		DESTROY(2.0f) EXPLODE(5) SOUND(WOOD) ICON("fenceGate") DESC(IDS_TILE_FENCE_GATE) DATA(15) USEDESC(IDS_DESC_FENCE_GATE);

	Tile::stairs_bricks = NEW_TILE(StairTile, stairs_bricks_Id, Tile::redBrick, 0)
		BASE(stairs, brick) ICON("stairsBrick") DESC(IDS_TILE_STAIRS_BRICKS) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::stairs_stoneBrickSmooth = NEW_TILE(StairTile, stairs_stoneBrick_Id, Tile::stoneBrick, 0)
		BASE(stairs, stoneSmooth) ICON("stairsStoneBrickSmooth") DESC(IDS_TILE_STAIRS_STONE_BRICKS_SMOOTH) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::mycel = static_cast<MycelTile*>(NEW_TILE(MycelTile, mycel_Id)
		DESTROY(0.6f) SOUND(GRASS) ICON("mycelium") DESC(IDS_TILE_MYCEL) USEDESC(IDS_DESC_MYCEL));

	Tile::waterLily = NEW_TILE(WaterlilyTile, waterLily_Id)
		DESTROY(0.0f) SOUND(GRASS) ICON("waterlily") DESC(IDS_TILE_WATERLILY) USEDESC(IDS_DESC_WATERLILY);

	Tile::netherBrick = NEW_TILE(Tile, netherBrick_Id, Material::stone)
		BASE(structblock, netherbrick) DESTROY(2.0f) EXPLODE(10) SOUND(STONE) ICON("nether_brick") DESC(IDS_TILE_NETHERBRICK) USEDESC(IDS_DESC_NETHERBRICK);

	Tile::netherFence = NEW_TILE(FenceTile, netherFence_Id, L"nether_brick", Material::stone)
		BASE(fence, netherbrick) DESTROY(2.0f) EXPLODE(10) SOUND(STONE) DESC(IDS_TILE_NETHERFENCE) USEDESC(IDS_DESC_NETHERFENCE);

	Tile::stairs_netherBricks = NEW_TILE(StairTile, stairs_netherBricks_Id, Tile::netherBrick, 0)
		BASE(stairs, netherbrick) ICON("stairsNetherBrick") DESC(IDS_TILE_STAIRS_NETHERBRICK) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::netherStalk = NEW_TILE(NetherWartTile, netherStalk_Id)
		ICON("nether_wart") DESC(IDS_TILE_NETHERSTALK) DATA(15) USEDESC(IDS_DESC_NETHERSTALK);

	Tile::enchantTable = NEW_TILE(EnchantmentTableTile, enchantTable_Id)
		BASE(device, magic) DESTROY(5.0f) EXPLODE(2000) ICON("enchanting_table") DESC(IDS_TILE_ENCHANTMENTTABLE) USEDESC(IDS_DESC_ENCHANTMENTTABLE);

	Tile::brewingStand = NEW_TILE(BrewingStandTile, brewingStand_Id)
		BASE(device, blaze) DESTROY(0.5f) LIGHTEMISSION(2 / 16.0f) ICON("brewing_stand") DESC(IDS_TILE_BREWINGSTAND) DATA(15) USEDESC(IDS_DESC_BREWING_STAND);

	Tile::cauldron = static_cast<CauldronTile*>(NEW_TILE(CauldronTile, cauldron_Id)
		DESTROY(2.0f) ICON("cauldron") DESC(IDS_TILE_CAULDRON) DATA(15) USEDESC(IDS_DESC_CAULDRON));

	Tile::endPortalTile = NEW_TILE(TheEndPortal, endPortalTile_Id, Material::portal)
		DESTROY(INDESTRUCTIBLE_DESTROY_TIME) EXPLODE(6000000) DESC(IDS_TILE_END_PORTAL) USEDESC(IDS_DESC_END_PORTAL);

	Tile::endPortalFrameTile = NEW_TILE(TheEndPortalFrameTile, endPortalFrameTile_Id)
		SOUND(GLASS) LIGHTEMISSION(2 / 16.0f) DESTROY(INDESTRUCTIBLE_DESTROY_TIME) ICON("endframe") DESC(IDS_TILE_ENDPORTALFRAME) DATA(15) EXPLODE(6000000) USEDESC(IDS_DESC_ENDPORTALFRAME);

	Tile::endStone = NEW_TILE(Tile, endStone_Id, Material::stone)
		DESTROY(3.0f) EXPLODE(15) SOUND(STONE) ICON("end_stone") DESC(IDS_TILE_WHITESTONE) USEDESC(IDS_DESC_WHITESTONE);

	Tile::dragonEgg = NEW_TILE(EggTile, dragonEgg_Id)
		DESTROY(3.0f) EXPLODE(15) SOUND(STONE) LIGHTEMISSION(2.0f / 16.0f) ICON("dragon_egg") DESC(IDS_TILE_DRAGONEGG) USEDESC(IDS_DESC_DRAGONEGG);

	Tile::redstoneLight = NEW_TILE(RedlightTile, redstoneLight_Id, false)
		DESTROY(0.3f) SOUND(GLASS) ICON("redstone_lamp_off") DESC(IDS_TILE_REDSTONE_LIGHT) USEDESC(IDS_DESC_REDSTONE_LIGHT);

	Tile::redstoneLight_lit = NEW_TILE(RedlightTile, redstoneLight_lit_Id, true)
		DESTROY(0.3f) SOUND(GLASS) ICON("redstone_lamp_on") DESC(IDS_TILE_REDSTONE_LIGHT) USEDESC(IDS_DESC_REDSTONE_LIGHT);

	Tile::woodSlab = static_cast<HalfSlabTile*>(NEW_TILE(WoodSlabTile, woodSlab_Id, true)
		BASE(slab, wood) DESTROY(2.0f) EXPLODE(5) SOUND(WOOD) ICON("woodSlab") DESC(IDS_DESC_WOODSLAB) USEDESC(IDS_DESC_WOODSLAB));

	Tile::woodSlabHalf = static_cast<HalfSlabTile*>(NEW_TILE(WoodSlabTile, woodSlabHalf_Id, false)
		BASE(halfslab, wood) DESTROY(2.0f) EXPLODE(5) SOUND(WOOD) ICON("woodSlab") DESC(IDS_DESC_WOODSLAB) USEDESC(IDS_DESC_WOODSLAB));

	Tile::cocoa = NEW_TILE(CocoaTile, cocoa_Id)
		DESTROY(0.2f) EXPLODE(5) SOUND(WOOD) ICON("cocoa") DATA(15) DESC(IDS_TILE_COCOA) USEDESC(IDS_DESC_COCOA);

	Tile::stairs_sandstone = NEW_TILE(StairTile, stairs_sandstone_Id, Tile::sandStone, 0)
		BASE(stairs, sand) ICON("stairsSandstone") DESC(IDS_TILE_STAIRS_SANDSTONE) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::emeraldOre = NEW_TILE(OreTile, emeraldOre_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("emerald_ore") DESC(IDS_TILE_EMERALDORE) USEDESC(IDS_DESC_EMERALDORE);

	Tile::enderChest = NEW_TILE(EnderChestTile, enderChest_Id)
		BASE(chest, ender) DESTROY(22.5f) EXPLODE(1000) SOUND(STONE) ICON("enderChest") DATA(15) LIGHTEMISSION(.5f) DESC(IDS_TILE_ENDERCHEST) USEDESC(IDS_DESC_ENDERCHEST);

	Tile::tripWireSource = static_cast<TripWireSourceTile*>(NEW_TILE(TripWireSourceTile, tripWireSource_Id)
		BASE(lever, undefined) ICON("trip_wire_source") DATA(15) DESC(IDS_TILE_TRIPWIRE_SOURCE) USEDESC(IDS_DESC_TRIPWIRE_SOURCE));

	Tile::tripWire = NEW_TILE(TripWireTile, tripWire_Id)
		ICON("trip_wire") DATA(15) DESC(IDS_TILE_TRIPWIRE) USEDESC(IDS_DESC_TRIPWIRE);

	Tile::emeraldBlock = NEW_TILE(MetalTile, emeraldBlock_Id)
		BASE(block, emerald) DESTROY(5.0f) EXPLODE(10) SOUND(METAL) ICON("emerald_block") DESC(IDS_TILE_EMERALDBLOCK) USEDESC(IDS_DESC_EMERALDBLOCK);

	Tile::woodStairsDark = NEW_TILE(StairTile, stairs_sprucewood_Id, Tile::wood, TreeTile::DARK_TRUNK)
		BASE(stairs, sprucewood) ICON("stairsWoodSpruce") DESC(IDS_TILE_STAIRS_SPRUCEWOOD) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::woodStairsBirch = NEW_TILE(StairTile, stairs_birchwood_Id, Tile::wood, TreeTile::BIRCH_TRUNK)
		BASE(stairs, birchwood) ICON("stairsWoodBirch") DESC(IDS_TILE_STAIRS_BIRCHWOOD) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::woodStairsJungle = NEW_TILE(StairTile, stairs_junglewood_Id, Tile::wood, TreeTile::JUNGLE_TRUNK)
		BASE(stairs, junglewood) ICON("stairsWoodJungle") DESC(IDS_TILE_STAIRS_JUNGLEWOOD) DATA(15) USEDESC(IDS_DESC_STAIRS);

	Tile::commandBlock = NEW_TILE(CommandBlock, commandBlock_Id)
		INDESTRUCTIBLE EXPLODE(6000000) ICON("command_block") DESC(IDS_TILE_COMMAND_BLOCK) USEDESC(IDS_DESC_COMMAND_BLOCK);

	Tile::beacon = static_cast<BeaconTile*>(NEW_TILE(BeaconTile, beacon_Id)
		LIGHTEMISSION(1.0f) ICON("beacon") DESC(IDS_TILE_BEACON) USEDESC(IDS_DESC_BEACON));

	Tile::cobbleWall = NEW_TILE(WallTile, cobbleWall_Id, Tile::stoneBrick)
		BASE(fence, stone) ICON("cobbleWall") DESC(IDS_TILE_COBBLESTONE_WALL) USEDESC(IDS_DESC_COBBLESTONE_WALL);

	Tile::flowerPot = NEW_TILE(FlowerPotTile, flowerPot_Id)
		DESTROY(0.0f) SOUND(NORMAL) ICON("flower_pot") DESC(IDS_TILE_FLOWERPOT) USEDESC(IDS_DESC_FLOWERPOT);

	Tile::carrots = NEW_TILE(CarrotTile, carrots_Id)
		ICON("carrots") DESC(IDS_TILE_CARROTS) USEDESC(IDS_DESC_CARROTS) NOMIPMAP;

	Tile::potatoes = NEW_TILE(PotatoTile, potatoes_Id)
		ICON("potatoes") DESC(IDS_TILE_POTATOES) USEDESC(IDS_DESC_POTATO) NOMIPMAP;

	Tile::button_wood = NEW_TILE(WoodButtonTile, button_wood_Id)
		BASE(button, wood) DESTROY(0.5f) SOUND(WOOD) ICON("button") DESC(IDS_TILE_BUTTON) DATA(15) USEDESC(IDS_DESC_BUTTON);

	Tile::skull = NEW_TILE(SkullTile, skull_Id)
		DESTROY(1.0f) SOUND(STONE) ICON("skull") DESC(IDS_TILE_SKULL) USEDESC(IDS_DESC_SKULL);

	Tile::anvil = NEW_TILE(AnvilTile, anvil_Id)
		BASE(device, iron) DESTROY(5.0f) SOUND(ANVIL) EXPLODE(2000) ICON("anvil") DATA(15) DESC(IDS_TILE_ANVIL) USEDESC(IDS_DESC_ANVIL);

	Tile::chest_trap = NEW_TILE(ChestTile, chest_trap_Id, ChestTile::TYPE_TRAP)
		BASE(chest, trap) DESTROY(2.5f) SOUND(WOOD) DESC(IDS_TILE_CHEST_TRAP) USEDESC(IDS_DESC_CHEST_TRAP);

	Tile::weightedPlate_light = NEW_TILE(WeightedPressurePlateTile, weightedPlate_light_Id, L"gold_block", Material::metal, Redstone::SIGNAL_MAX)
		BASE(pressureplate, gold) DESTROY(0.5f) SOUND(WOOD) DESC(IDS_TILE_WEIGHTED_PLATE_LIGHT) USEDESC(IDS_DESC_WEIGHTED_PLATE_LIGHT);

	Tile::weightedPlate_heavy = NEW_TILE(WeightedPressurePlateTile, weightedPlate_heavy_Id, L"iron_block", Material::metal, Redstone::SIGNAL_MAX * 10)
		BASE(pressureplate, iron) DESTROY(0.5f) SOUND(WOOD) DESC(IDS_TILE_WEIGHTED_PLATE_HEAVY) USEDESC(IDS_DESC_WEIGHTED_PLATE_HEAVY);

	Tile::comparator_off = static_cast<ComparatorTile*>(NEW_TILE(ComparatorTile, comparator_off_Id, false)
		DESTROY(0.0f) SOUND(WOOD) ICON("comparator_off") DESC(IDS_TILE_COMPARATOR) USEDESC(IDS_DESC_COMPARATOR));

	Tile::comparator_on = static_cast<ComparatorTile*>(NEW_TILE(ComparatorTile, comparator_on_Id, true)
		DESTROY(0.0f) LIGHTEMISSION(10 / 16.0f) SOUND(WOOD) ICON("comparator_on") DESC(IDS_TILE_COMPARATOR) USEDESC(IDS_DESC_COMPARATOR));

	Tile::daylightDetector = static_cast<DaylightDetectorTile*>(NEW_TILE(DaylightDetectorTile, daylightDetector_Id)
		DESTROY(0.2f) SOUND(WOOD) ICON("daylight_detector") DESC(IDS_TILE_DAYLIGHT_DETECTOR) USEDESC(IDS_DESC_DAYLIGHT_DETECTOR));

	Tile::redstoneBlock = NEW_TILE(PoweredMetalTile, redstoneBlock_Id)
		BASE(block, redstone) DESTROY(5.0f) EXPLODE(10) SOUND(METAL) ICON("redstone_block") DESC(IDS_TILE_REDSTONE_BLOCK) USEDESC(IDS_DESC_REDSTONE_BLOCK);

	Tile::netherQuartz = NEW_TILE(OreTile, netherQuartz_Id)
		DESTROY(3.0f) EXPLODE(5) SOUND(STONE) ICON("quartz_ore") DESC(IDS_TILE_NETHER_QUARTZ) USEDESC(IDS_DESC_NETHER_QUARTZ_ORE);

	Tile::hopper = static_cast<HopperTile*>(NEW_TILE(HopperTile, hopper_Id)
		BASE(redstoneContainer, undefined) DESTROY(3.0f) EXPLODE(8) SOUND(WOOD) ICON("hopper") DESC(IDS_TILE_HOPPER) USEDESC(IDS_DESC_HOPPER));

	Tile::quartzBlock = NEW_TILE(QuartzBlockTile, quartzBlock_Id)
		BASE(structblock, quartz) SOUND(STONE) DESTROY(0.8f) ICON("quartz_block") DESC(IDS_TILE_QUARTZ_BLOCK) USEDESC(IDS_DESC_QUARTZ_BLOCK);

	Tile::stairs_quartz = NEW_TILE(StairTile, stairs_quartz_Id, Tile::quartzBlock, QuartzBlockTile::TYPE_DEFAULT)
		BASE(stairs, quartz) ICON("stairsQuartz") DESC(IDS_TILE_STAIRS_QUARTZ) USEDESC(IDS_DESC_STAIRS);

	Tile::activatorRail = NEW_TILE(PoweredRailTile, activatorRail_Id)
		DESTROY(0.7f) SOUND(METAL) ICON("rail_activator") DESC(IDS_TILE_ACTIVATOR_RAIL) USEDESC(IDS_DESC_ACTIVATOR_RAIL);

	Tile::dropper = NEW_TILE(DropperTile, dropper_Id)
		BASE(redstoneContainer, undefined) DESTROY(3.5f) SOUND(STONE) ICON("dropper") DESC(IDS_TILE_DROPPER) USEDESC(IDS_DESC_DROPPER);

	Tile::clayHardened_colored = static_cast<ColoredTile*>(NEW_TILE(ColoredTile, clayHardened_colored_Id, Material::stone)
		BASE(clay, clay) DESTROY(1.25f) EXPLODE(7) SOUND(STONE) ICON("hardened_clay_stained") DESC(IDS_TILE_STAINED_CLAY) USEDESC(IDS_DESC_STAINED_CLAY));

	Tile::stained_glass_pane = NEW_TILE(StainedGlassPaneBlock, stained_glass_pane_Id)
		BASE(glass, glass) DESTROY(0.3f) SOUND(GLASS) ICON("glass") DESC(IDS_TILE_STAINED_GLASS_PANE) USEDESC(IDS_DESC_STAINED_GLASS_PANE);

	Tile::hayBlock = NEW_TILE(HayBlockTile, hayBlock_Id)
		BASE(block, wheat) DESTROY(0.5f) SOUND(GRASS) ICON("hay_block") DESC(IDS_TILE_HAY) USEDESC(IDS_DESC_HAY);

	Tile::woolCarpet = NEW_TILE(WoolCarpetTile, woolCarpet_Id)
		BASE(carpet, cloth) DESTROY(0.1f) SOUND(CLOTH) ICON("woolCarpet") LIGHTBLOCK(0) DESC(IDS_TILE_CARPET) USEDESC(IDS_DESC_CARPET);

	Tile::clayHardened = NEW_TILE(Tile, clayHardened_Id, Material::stone)
		BASE(clay, clay) DESTROY(1.25f) EXPLODE(7) SOUND(STONE) ICON("hardened_clay") DESC(IDS_TILE_HARDENED_CLAY) USEDESC(IDS_DESC_HARDENED_CLAY);

	Tile::coalBlock = NEW_TILE(Tile, coalBlock_Id, Material::stone)
		BASE(block, coal) DESTROY(5.0f) EXPLODE(10) SOUND(STONE) ICON("coal_block") DESC(IDS_TILE_COAL) USEDESC(IDS_DESC_COAL_BLOCK);

	Tile::lightSourceBlock = NEW_TILE(LightSourceTile, lightSourceBlock_Id)
		BASE(torch, wood) DESTROY(0.0f) LIGHTEMISSION(15 / 16.0f) SOUND(WOOD) ICON("torch_on") DESC(IDS_TILE_TORCH) USEDESC(IDS_DESC_TORCH) NOMIPMAP;

	// --- Special cases (Item::items) ---
	Item::items[wool_Id] = (new WoolTileItem(Tile::wool_Id - 256))->setIconName(L"cloth")->setDescriptionId(IDS_TILE_CLOTH)->setUseDescriptionId(IDS_DESC_WOOL);
	Item::items[clayHardened_colored_Id] = (new WoolTileItem(Tile::clayHardened_colored_Id - 256))->setIconName(L"clayHardenedStained")->setDescriptionId(IDS_TILE_STAINED_CLAY)->setUseDescriptionId(IDS_DESC_STAINED_CLAY);
	Item::items[stained_glass_Id] = (new WoolTileItem(Tile::stained_glass_Id - 256))->setIconName(L"stainedGlass")->setDescriptionId(IDS_TILE_STAINED_GLASS)->setUseDescriptionId(IDS_DESC_STAINED_GLASS);
	Item::items[stained_glass_pane_Id] = (new WoolTileItem(Tile::stained_glass_pane_Id - 256))->setIconName(L"stainedGlassPane")->setDescriptionId(IDS_TILE_STAINED_GLASS_PANE)->setUseDescriptionId(IDS_DESC_STAINED_GLASS_PANE);
	Item::items[woolCarpet_Id] = (new WoolTileItem(Tile::woolCarpet_Id - 256))->setIconName(L"woolCarpet")->setDescriptionId(IDS_TILE_CARPET)->setUseDescriptionId(IDS_DESC_CARPET);
	Item::items[treeTrunk_Id] = (new MultiTextureTileItem(Tile::treeTrunk_Id - 256, treeTrunk, (int*)TreeTile::TREE_NAMES, 4))->setIconName(L"log")->setDescriptionId(IDS_TILE_LOG)->setUseDescriptionId(IDS_DESC_LOG);
	Item::items[wood_Id] = (new MultiTextureTileItem(Tile::wood_Id - 256, Tile::wood, (int*)WoodTile::WOOD_NAMES, 4, IDS_TILE_PLANKS))->setIconName(L"wood")->setDescriptionId(IDS_TILE_OAKWOOD_PLANKS)->setUseDescriptionId(IDS_DESC_LOG); //  <- TODO
	Item::items[monsterStoneEgg_Id] = (new MultiTextureTileItem(Tile::monsterStoneEgg_Id - 256, monsterStoneEgg, (int*)StoneMonsterTile::STONE_MONSTER_NAMES, 3))->setIconName(L"monsterStoneEgg")->setDescriptionId(IDS_TILE_STONE_SILVERFISH)->setUseDescriptionId(IDS_DESC_STONE_SILVERFISH);
	Item::items[stoneBrick_Id] = (new MultiTextureTileItem(Tile::stoneBrick_Id - 256, stoneBrick, (int*)SmoothStoneBrickTile::SMOOTH_STONE_BRICK_NAMES, 4))->setIconName(L"stonebricksmooth")->setDescriptionId(IDS_TILE_STONE_BRICK_SMOOTH);
	Item::items[sandStone_Id] = (new MultiTextureTileItem(sandStone_Id - 256, sandStone, SandStoneTile::SANDSTONE_NAMES, SandStoneTile::SANDSTONE_BLOCK_NAMES))->setIconName(L"sandStone")->setDescriptionId(IDS_TILE_SANDSTONE)->setUseDescriptionId(IDS_DESC_SANDSTONE);
	Item::items[quartzBlock_Id] = (new MultiTextureTileItem(quartzBlock_Id - 256, quartzBlock, QuartzBlockTile::BLOCK_NAMES, QuartzBlockTile::QUARTZ_BLOCK_NAMES))->setIconName(L"quartzBlock")->setDescriptionId(IDS_TILE_QUARTZ_BLOCK)->setUseDescriptionId(IDS_DESC_QUARTZ_BLOCK);
	Item::items[stoneSlabHalf_Id] = (new StoneSlabTileItem(Tile::stoneSlabHalf_Id - 256, Tile::stoneSlabHalf, Tile::stoneSlab, false))->setIconName(L"stoneSlab")->setDescriptionId(IDS_TILE_STONESLAB)->setUseDescriptionId(IDS_DESC_HALFSLAB);
	Item::items[stoneSlab_Id] = (new StoneSlabTileItem(Tile::stoneSlab_Id - 256, Tile::stoneSlabHalf, Tile::stoneSlab, true))->setIconName(L"stoneSlab")->setDescriptionId(IDS_DESC_STONESLAB)->setUseDescriptionId(IDS_DESC_SLAB);
	Item::items[woodSlabHalf_Id] = (new StoneSlabTileItem(Tile::woodSlabHalf_Id - 256, Tile::woodSlabHalf, Tile::woodSlab, false))->setIconName(L"woodSlab")->setDescriptionId(IDS_DESC_WOODSLAB)->setUseDescriptionId(IDS_DESC_WOODSLAB);
	Item::items[woodSlab_Id] = (new StoneSlabTileItem(Tile::woodSlab_Id - 256, Tile::woodSlabHalf, Tile::woodSlab, true))->setIconName(L"woodSlab")->setDescriptionId(IDS_DESC_WOODSLAB)->setUseDescriptionId(IDS_DESC_WOODSLAB);
	Item::items[sapling_Id] = (new MultiTextureTileItem(Tile::sapling_Id - 256, Tile::sapling, Sapling::SAPLING_NAMES, 4))->setIconName(L"sapling")->setDescriptionId(IDS_TILE_SAPLING)->setUseDescriptionId(IDS_DESC_SAPLING);
	Item::items[leaves_Id] = (new LeafTileItem(Tile::leaves_Id - 256))->setIconName(L"leaves")->setDescriptionId(IDS_TILE_LEAVES)->setUseDescriptionId(IDS_DESC_LEAVES);
	Item::items[vine_Id] = (new ColoredTileItem(Tile::vine_Id - 256, false))->setDescriptionId(IDS_TILE_VINE)->setUseDescriptionId(IDS_DESC_VINE);
	int idsData[3] = { IDS_TILE_SHRUB, IDS_TILE_TALL_GRASS, IDS_TILE_FERN };
	intArray ids = intArray(idsData, 3);
	Item::items[tallgrass_Id] = static_cast<ColoredTileItem*>((new ColoredTileItem(Tile::tallgrass_Id - 256, true))->setDescriptionId(IDS_TILE_TALL_GRASS))->setDescriptionPostfixes(ids);
	Item::items[topSnow_Id] = (new SnowItem(topSnow_Id - 256, topSnow));
	Item::items[waterLily_Id] = (new WaterLilyTileItem(Tile::waterLily_Id - 256));
	Item::items[pistonBase_Id] = (new PistonTileItem(Tile::pistonBase_Id - 256))->setDescriptionId(IDS_TILE_PISTON_BASE)->setUseDescriptionId(IDS_DESC_PISTON);
	Item::items[pistonStickyBase_Id] = (new PistonTileItem(Tile::pistonStickyBase_Id - 256))->setDescriptionId(IDS_TILE_PISTON_STICK_BASE)->setUseDescriptionId(IDS_DESC_STICKY_PISTON);
	Item::items[cobbleWall_Id] = (new MultiTextureTileItem(cobbleWall_Id - 256, cobbleWall, (int*)WallTile::COBBLE_NAMES, 2))->setDescriptionId(IDS_TILE_COBBLESTONE_WALL)->setUseDescriptionId(IDS_DESC_COBBLESTONE_WALL);
	Item::items[anvil_Id] = (new AnvilTileItem(anvil))->setDescriptionId(IDS_TILE_ANVIL)->setUseDescriptionId(IDS_DESC_ANVIL);

	for (int i = 0; i < 256; i++)
	{
		if (Tile::tiles[i] != nullptr)
		{
			if (Item::items[i] == nullptr)
			{
				Item::items[i] = new TileItem(i - 256);
				Tile::tiles[i]->init();
			}

			bool propagate = false;
			if (i > 0 && Tile::tiles[i]->getRenderShape() == Tile::SHAPE_STAIRS) propagate = true;
			if (i > 0 && dynamic_cast<HalfSlabTile*>(Tile::tiles[i]) != nullptr)
			{
				propagate = true;
			}
			if (i == Tile::farmland_Id) propagate = true;
			if (Tile::transculent[i])
			{
				propagate = true;
			}
			if (Tile::lightBlock[i] == 0)
			{
				propagate = true;
			}
			Tile::propagate[i] = propagate;
		}
	}
	Tile::transculent[0] = true;

	Stats::buildItemStats();

#undef NEW_TILE
#undef DESTROY
#undef EXPLODE
#undef SOUND
#undef ICON
#undef DESC
#undef USEDESC
#undef BASE
#undef DATA
#undef LIGHTBLOCK
#undef LIGHTEMISSION
#undef NOMIPMAP
#undef NOTCOLLECT
#undef TICKING
#undef INDESTRUCTIBLE
}

// 4J - added for common ctor code
void Tile::_init(int id, Material *material, bool isSolidRender)
{
	destroySpeed = 0.0f;
	explosionResistance = 0.0f;
	isInventoryItem = true;
	collectStatistics = true;

	// 4J Stu - Removed these in favour of TLS versions
	//xx0 = yy0 = zz0 = xx1 = yy1 = zz1 = 0;

	soundType = Tile::SOUND_NORMAL;
	gravity = 1.0f;
	friction = 0.6f;
	_isTicking = false;
	_isEntityTile = false;

	/*	4J - TODO
	if (Tile.tiles[id] != null)
	{
	throw new IllegalArgumentException("Slot " + id + " is already occupied by " + Tile.tiles[id] + " when adding " + this);
	}
	*/
	this->material = material;
	Tile::tiles[id] = this;
	this->id = id;
	updateDefaultShape();
	// 4J - note these used to call isSolidRender(), but that always calls Tile::isSolidRender in C++ so have added as a parameter that can be varied from
	// derived ctors
	solid[id] = isSolidRender;
	lightBlock[id] = isSolidRender ? 255 : 0;
	transculent[id] = !material->blocksLight();
	mipmapEnable[id] = true;	// 4J added
	iconName = L"";
}

Tile::Tile(int id, Material *material, bool isSolidRender)
{
	_init(id,material, isSolidRender);
	m_iMaterial=Item::eMaterial_undefined;
	m_iBaseItemType=Item::eBaseItemType_undefined;
	icon = nullptr;
}

Tile *Tile::sendTileData(unsigned char importantMask/*=15*/)
{
	Tile::_sendTileData[id] = importantMask;		// 4J - changed was bool, now bitfield to indicate which bits are important to be sent. Default behaviour with this method is all 4 bits
	return this;
}

void Tile::init()
{
}


// 4J-PB - adding so we can class different items together for the new crafting menu
// so pickaxe_stone would get tagged with pickaxe and stone
Tile *Tile::setBaseItemTypeAndMaterial(int iType,int iMaterial)
{
	this->m_iBaseItemType = iType;
	this->m_iMaterial = iMaterial;
	return this;
}

int Tile::getBaseItemType()
{
	return this->m_iBaseItemType;
}

int Tile::getMaterial()
{
	return this->m_iMaterial;
}


Tile *Tile::setSoundType(const SoundType *soundType)
{
	this->soundType = soundType;
	return this;
}

Tile *Tile::setLightBlock(int i)
{
	lightBlock[id] = i;
	return this;
}

Tile *Tile::setLightEmission(float f)
{
	Tile::lightEmission[id] = static_cast<int>(Level::MAX_BRIGHTNESS * f);
	return this;
}

Tile *Tile::setExplodeable(float explosionResistance)
{
	this->explosionResistance = explosionResistance * 3;
	return this;
}

bool Tile::isSolidBlockingTile(int t)
{
	Tile *tile = Tile::tiles[t];
	if (tile == nullptr) return false;
	return tile->material->isSolidBlocking() && tile->isCubeShaped() && !tile->isSignalSource();
}

bool Tile::isCubeShaped()
{
	return true;
}

bool Tile::isPathfindable(LevelSource *level, int x, int y, int z)
{
	return !material->blocksMotion();
}

int Tile::getRenderShape()
{
	return SHAPE_BLOCK;
}

Tile *Tile::setDestroyTime(float destroySpeed)
{
	this->destroySpeed = destroySpeed;
	if (explosionResistance < destroySpeed * 5) explosionResistance = destroySpeed * 5;
	return this;
}

Tile *Tile::setIndestructible()
{
	setDestroyTime(INDESTRUCTIBLE_DESTROY_TIME);
	return this;
}

float Tile::getDestroySpeed(Level *level, int x, int y, int z)
{
	return destroySpeed;
}

Tile *Tile::setTicking(bool tick)
{
	_isTicking = tick;
	return this;
}

bool Tile::isTicking()
{
	return _isTicking;
}

bool Tile::isEntityTile()
{
	return _isEntityTile;
}

Tile *Tile::disableMipmap()
{
	mipmapEnable[id] = false;
	return this;
}

void Tile::setShape(float x0, float y0, float z0, float x1, float y1, float z1)
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	tls->xx0 = x0;
	tls->yy0 = y0;
	tls->zz0 = z0;
	tls->xx1 = x1;
	tls->yy1 = y1;
	tls->zz1 = z1;
	tls->tileId = this->id;

	//this->xx0 = x0;
	//this->yy0 = y0;
	//this->zz0 = z0;
	//this->xx1 = x1;
	//this->yy1 = y1;
	//this->zz1 = z1;
}

float Tile::getBrightness(LevelSource *level, int x, int y, int z)
{
	// Lighting fix brought forward from ~1.5 here - used to use the lightEmission level for this tile rather than getting the for the passed in x/y/z coords
	return level->getBrightness(x, y, z, lightEmission[level->getTile(x,y,z)]);
}

// 4J - brought forward from 1.8.2
int Tile::getLightColor(LevelSource *level, int x, int y, int z, int tileId/*=-1*/)
{
	// Lighting fix brought forward from ~1.5 here - used to use the lightEmission level for this tile rather than getting the for the passed in x/y/z coords
	if( tileId == -1 )
	{
		return level->getLightColor(x, y, z, lightEmission[level->getTile(x,y,z)], -1);
	}
	else
	{
		return level->getLightColor(x, y, z, lightEmission[tileId], tileId);
	}
}

bool Tile::isFaceVisible(Level *level, int x, int y, int z, int f)
{
	if (f == 0) y--;
	if (f == 1) y++;
	if (f == 2) z--;
	if (f == 3) z++;
	if (f == 4) x--;
	if (f == 5) x++;
	return !level->isSolidRenderTile(x, y, z);
}

bool Tile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	if (face == 0 && tls->yy0 > 0) return true;
	if (face == 1 && tls->yy1 < 1) return true;
	if (face == 2 && tls->zz0 > 0) return true;
	if (face == 3 && tls->zz1 < 1) return true;
	if (face == 4 && tls->xx0 > 0) return true;
	if (face == 5 && tls->xx1 < 1) return true;
	return (!level->isSolidRenderTile(x, y, z));
}

// AP - added this function so we can generate the faceFlags for a block in a single fast function
int Tile::getFaceFlags(LevelSource *level, int x, int y, int z)
{
	int faceFlags = 0;

	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();

	if( tls->yy0 > 0 || (!level->isSolidRenderTile(x, y - 1, z))) faceFlags |= 0x01;
	if( tls->yy1 < 1 || (!level->isSolidRenderTile(x, y + 1, z))) faceFlags |= 0x02;
	if( tls->zz0 > 0 || (!level->isSolidRenderTile(x, y, z - 1))) faceFlags |= 0x04;
	if( tls->zz1 < 1 || (!level->isSolidRenderTile(x, y, z + 1))) faceFlags |= 0x08;
	if( tls->xx0 > 0 || (!level->isSolidRenderTile(x - 1, y, z))) faceFlags |= 0x10;
	if( tls->xx1 < 1 || (!level->isSolidRenderTile(x + 1, y, z))) faceFlags |= 0x20;

	return faceFlags;
}

bool Tile::isSolidFace(LevelSource *level, int x, int y, int z, int face)
{
	return (level->getMaterial(x, y, z)->isSolid());
}

Icon *Tile::getTexture(LevelSource *level, int x, int y, int z, int face)
{
	// 4J - addition here to make rendering big blocks of leaves more efficient. Normally leaves never consider themselves as solid, so
	// blocks of leaves will have all sides of each block completely visible. Changing to consider as solid if this block is surrounded by
	// other leaves (or solid things). This is paired with another change in  Level::isSolidRenderTile/Region::isSolidRenderTile which makes things solid
	// code-wise (ie for determining visible sides of neighbouring blocks). This change just makes the texture a solid one (tex + 1) which
	// we already have in the texture map for doing non-fancy graphics. Note: this tile-specific code is here rather than making some new virtual
	// method in the tiles, for the sake of efficiency - I don't imagine we'll be doing much more of this sort of thing

	int tileId = level->getTile(x, y, z);
	int tileData = level->getData(x, y, z);

	if( tileId == Tile::leaves_Id )
	{
		bool opaque = true;

		int axo[6] = { 1,-1, 0, 0, 0, 0};
		int ayo[6] = { 0, 0, 1,-1, 0, 0};
		int azo[6] = { 0, 0, 0, 0, 1,-1};
		for( int i = 0; (i < 6) && opaque; i++ )
		{
			int t = level->getTile(x + axo[i], y + ayo[i] , z + azo[i]);
			if( ( t != Tile::leaves_Id ) && ( ( Tile::tiles[t] == nullptr ) || !Tile::tiles[t]->isSolidRender() ) )
			{
				opaque = false;
			}
		}

		Icon *icon = nullptr;
		if(opaque)
		{
			Tile::leaves->setFancy(false);
			icon = getTexture(face, tileData);
			Tile::leaves->setFancy(true);
		}
		else
		{
			icon = getTexture(face, tileData);
		}
		return icon;
	}
	return getTexture(face, tileData);
}

Icon *Tile::getTexture(int face, int data)
{
	return icon;
}

Icon *Tile::getTexture(int face)
{
	return getTexture(face, 0);
}

AABB *Tile::getTileAABB(Level *level, int x, int y, int z)
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	if (tls == nullptr)
		return AABB::newTemp(x, y, z, x, y, z);

	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) 
		updateDefaultShape();

	return AABB::newTemp(x + tls->xx0, y + tls->yy0, z + tls->zz0, x + tls->xx1, y + tls->yy1, z + tls->zz1);
}

void Tile::addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source)
{
	AABB *aabb = getAABB(level, x, y, z);
	if (aabb != nullptr && box->intersects(aabb)) boxes->push_back(aabb);
}

AABB *Tile::getAABB(Level *level, int x, int y, int z)
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));

	if (tls == nullptr)
		return AABB::newTemp(x, y, z, x, y, z);

	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) 
		updateDefaultShape();

	return AABB::newTemp(x + tls->xx0, y + tls->yy0, z + tls->zz0, x + tls->xx1, y + tls->yy1, z + tls->zz1);
}

bool Tile::isSolidRender(bool isServerLevel)
{
	return true;
}

bool Tile::mayPick(int data, bool liquid)
{
	return mayPick();
}

bool Tile::mayPick()
{
	return true;
}

void Tile::tick(Level *level, int x, int y, int z, Random *random)
{
}

void Tile::animateTick(Level *level, int x, int y, int z, Random *random)
{
}

void Tile::destroy(Level *level, int x, int y, int z, int data)
{
}

void Tile::neighborChanged(Level *level, int x, int y, int z, int type)
{
}

void Tile::addLights(Level *level, int x, int y, int z)
{
}

int Tile::getTickDelay(Level *level)
{
	return 10;
}

void Tile::onPlace(Level *level, int x, int y, int z)
{
}

void Tile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
}

int Tile::getResourceCount(Random *random)
{
	return 1;
}

int Tile::getResource(int data, Random *random, int playerBonusLevel)
{
	return id;
}

float Tile::getDestroyProgress(shared_ptr<Player> player, Level *level, int x, int y, int z)
{
	float destroySpeed = getDestroySpeed(level, x, y, z);
	if (destroySpeed < 0) return 0;
	if (!player->canDestroy(this))
	{
		return player->getDestroySpeed(this, false) / destroySpeed / 100.0f;
	}
	return (player->getDestroySpeed(this, true) / destroySpeed) / 30;
}

void Tile::spawnResources(Level *level, int x, int y, int z, int data, int playerBonusLevel)
{
	spawnResources(level, x, y, z, data, 1, playerBonusLevel);
}

void Tile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	if (level->isClientSide) return;
	int count = getResourceCountForLootBonus(playerBonusLevel, level->random);
	for (int i = 0; i < count; i++)
	{
		if (level->random->nextFloat() > odds) continue;
		int type = getResource(data, level->random, playerBonusLevel);
		if (type <= 0) continue;

		popResource(level, x, y, z, std::make_shared<ItemInstance>(type, 1, getSpawnResourcesAuxValue(data)));
	}
}

void Tile::popResource(Level *level, int x, int y, int z, shared_ptr<ItemInstance> itemInstance)
{
	if( level->isClientSide || !level->getGameRules()->getBoolean(GameRules::RULE_DOTILEDROPS) ) return;

	float s = 0.7f;
	double xo = level->random->nextFloat() * s + (1 - s) * 0.5;
	double yo = level->random->nextFloat() * s + (1 - s) * 0.5;
	double zo = level->random->nextFloat() * s + (1 - s) * 0.5;
	shared_ptr<ItemEntity> item = std::make_shared<ItemEntity>(level, x + xo, y + yo, z + zo, itemInstance);
	item->throwTime = 10;
	level->addEntity(item);
}

// Brought forward for TU7
void Tile::popExperience(Level *level, int x, int y, int z, int amount)
{
	if (!level->isClientSide)
	{
		while (amount > 0)
		{
			int newCount = ExperienceOrb::getExperienceValue(amount);
			amount -= newCount;
			level->addEntity(std::make_shared<ExperienceOrb>(level, x + .5, y + .5, z + .5, newCount));
		}
	}
}

int Tile::getSpawnResourcesAuxValue(int data)
{
	return 0;
}

float Tile::getExplosionResistance(shared_ptr<Entity> source)
{
	return explosionResistance / 5.0f;
}

HitResult *Tile::clip(Level *level, int xt, int yt, int zt, Vec3 *a, Vec3 *b)
{
	updateShape(level, xt, yt, zt);

	a = a->add(-xt, -yt, -zt);
	b = b->add(-xt, -yt, -zt);

	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	Vec3 *xh0 = a->clipX(b, tls->xx0);
	Vec3 *xh1 = a->clipX(b, tls->xx1);

	Vec3 *yh0 = a->clipY(b, tls->yy0);
	Vec3 *yh1 = a->clipY(b, tls->yy1);

	Vec3 *zh0 = a->clipZ(b, tls->zz0);
	Vec3 *zh1 = a->clipZ(b, tls->zz1);

	Vec3 *closest = nullptr;

	if (containsX(xh0) && (closest == nullptr || a->distanceToSqr(xh0) < a->distanceToSqr(closest))) closest = xh0;
	if (containsX(xh1) && (closest == nullptr || a->distanceToSqr(xh1) < a->distanceToSqr(closest))) closest = xh1;
	if (containsY(yh0) && (closest == nullptr || a->distanceToSqr(yh0) < a->distanceToSqr(closest))) closest = yh0;
	if (containsY(yh1) && (closest == nullptr || a->distanceToSqr(yh1) < a->distanceToSqr(closest))) closest = yh1;
	if (containsZ(zh0) && (closest == nullptr || a->distanceToSqr(zh0) < a->distanceToSqr(closest))) closest = zh0;
	if (containsZ(zh1) && (closest == nullptr || a->distanceToSqr(zh1) < a->distanceToSqr(closest))) closest = zh1;

	if (closest == nullptr) return nullptr;

	int face = -1;

	if (closest == xh0) face = Facing::WEST;
	if (closest == xh1) face = Facing::EAST;
	if (closest == yh0) face = Facing::DOWN;
	if (closest == yh1) face = Facing::UP;
	if (closest == zh0) face = Facing::NORTH;
	if (closest == zh1) face = Facing::SOUTH;

	return new HitResult(xt, yt, zt, face, closest->add(xt, yt, zt));
}

bool Tile::containsX(Vec3 *v)
{
	if( v == nullptr) return false;

	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return v->y >= tls->yy0 && v->y <= tls->yy1 && v->z >= tls->zz0 && v->z <= tls->zz1;
}

bool Tile::containsY(Vec3 *v)
{
	if( v == nullptr) return false;

	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return v->x >= tls->xx0 && v->x <= tls->xx1 && v->z >= tls->zz0 && v->z <= tls->zz1;
}

bool Tile::containsZ(Vec3 *v)
{
	if( v == nullptr) return false;

	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return v->x >= tls->xx0 && v->x <= tls->xx1 && v->y >= tls->yy0 && v->y <= tls->yy1;
}

void Tile::wasExploded(Level *level, int x, int y, int z, Explosion *explosion)
{
}

bool Tile::mayPlace(Level *level, int x, int y, int z, int face, shared_ptr<ItemInstance> item)
{
	return mayPlace(level, x, y, z, face);
}

int Tile::getRenderLayer()
{
	return 0;
}

bool Tile::mayPlace(Level *level, int x, int y, int z, int face)
{
	return mayPlace(level, x, y, z);
}

bool Tile::mayPlace(Level *level, int x, int y, int z)
{
	int t = level->getTile(x, y, z);
	return t == 0 || Tile::tiles[t]->material->isReplaceable();
}

// 4J-PB - Adding a TestUse for tooltip display
bool Tile::TestUse()
{
	return false;
}

bool Tile::TestUse(Level *level, int x, int y, int z, shared_ptr<Player> player)
{
	return false;
}

bool Tile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	return false;
}

void Tile::stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
}

int Tile::getPlacedOnFaceDataValue(Level *level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, int itemValue)
{
	return itemValue;
}

void Tile::prepareRender(Level *level, int x, int y, int z)
{
}

void Tile::attack(Level *level, int x, int y, int z, shared_ptr<Player> player)
{
}

void Tile::handleEntityInside(Level *level, int x, int y, int z, shared_ptr<Entity> e, Vec3 *current)
{
}

void Tile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity) // 4J added forceData, forceEntity param
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
}

double Tile::getShapeX0()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->xx0;
}

double Tile::getShapeX1()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->xx1;
}

double Tile::getShapeY0()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->yy0;
}

double Tile::getShapeY1()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->yy1;
}

double Tile::getShapeZ0()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->zz0;
}

double Tile::getShapeZ1()
{
	ThreadStorage *tls = static_cast<ThreadStorage *>(TlsGetValue(Tile::tlsIdxShape));
	// 4J Stu - Added this so that the TLS shape is correct for this tile
	if(tls->tileId != this->id) updateDefaultShape();
	return tls->zz1;
}

int Tile::getColor() const
{
	return 0xffffff;
}

int Tile::getColor(int auxData)
{
	return 0xffffff;
}

int Tile::getColor(LevelSource *level, int x, int y, int z)
{
	return 0xffffff;
}

int Tile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return 0xffffff;
}

int Tile::getSignal(LevelSource *level, int x, int y, int z, int dir)
{
	return Redstone::SIGNAL_NONE;
}

bool Tile::isSignalSource()
{
	return false;
}

void Tile::entityInside(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
}

int Tile::getDirectSignal(LevelSource *level, int x, int y, int z, int dir)
{
	return Redstone::SIGNAL_NONE;
}

void Tile::updateDefaultShape()
{
	setShape(0,0,0,1,1,1);
}

void Tile::playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data)
{
	// 4J Stu - Special case - only record a crop destroy if is fully grown
	if( id==Tile::wheat_Id )
	{
		if( Tile::wheat->getResource(data, nullptr, 0) > 0 )
			player->awardStat(
			GenericStats::blocksMined(id),
			GenericStats::param_blocksMined(id,data,1)
			);
	}
	else if (id == Tile::potatoes_Id)
	{
		if (Tile::potatoes->getResource(data, nullptr, 0) > 0)
			player->awardStat(
			GenericStats::blocksMined(id),
			GenericStats::param_blocksMined(id,data,1)
			);
	}
	else if (id == Tile::carrots_Id)
	{
		if (Tile::potatoes->getResource(data, nullptr, 0) > 0)
			player->awardStat(
			GenericStats::blocksMined(id),
			GenericStats::param_blocksMined(id,data,1)
			);
	}
	else
	{
		player->awardStat(
			GenericStats::blocksMined(id),
			GenericStats::param_blocksMined(id,data,1)
			);
	}
	player->awardStat(GenericStats::totalBlocksMined(), GenericStats::param_noArgs());	// 4J : WESTY : Added for other award.
	player->causeFoodExhaustion(FoodConstants::EXHAUSTION_MINE);

	if( id == Tile::treeTrunk_Id )
		player->awardStat(GenericStats::mineWood(), GenericStats::param_noArgs());


	if (isSilkTouchable() && EnchantmentHelper::hasSilkTouch(player))
	{
		shared_ptr<ItemInstance> item = getSilkTouchItemInstance(data);
		if (item != nullptr)
		{
			popResource(level, x, y, z, item);
		}
	}
	else
	{
		int playerBonusLevel = EnchantmentHelper::getDiggingLootBonus(player);
		spawnResources(level, x, y, z, data, playerBonusLevel);
	}
}

bool Tile::isSilkTouchable()
{
	return isCubeShaped() && !_isEntityTile;
}

shared_ptr<ItemInstance> Tile::getSilkTouchItemInstance(int data)
{
	int popData = 0;
	if (id >= 0 && id < Item::items.length && Item::items[id]->isStackedByData())
	{
		popData = data;
	}
	return std::make_shared<ItemInstance>(id, 1, popData);
}

int Tile::getResourceCountForLootBonus(int bonusLevel, Random *random)
{
	return getResourceCount(random);
}

bool Tile::canSurvive(Level *level, int x, int y, int z)
{
	return true;
}

void Tile::setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance)
{
}

void Tile::finalizePlacement(Level *level, int x, int y, int z, int data)
{
}

Tile *Tile::setDescriptionId(unsigned int id)
{
	this->descriptionId = id;
	return this;
}

wstring Tile::getName()
{
	return L"";//I18n::get(getDescriptionId() + L".name");
}

unsigned int Tile::getDescriptionId(int iData /*= -1*/)
{
	return descriptionId;
}

Tile *Tile::setUseDescriptionId(unsigned int id)
{
	this->useDescriptionId = id;
	return this;
}

unsigned int Tile::getUseDescriptionId()
{
	return useDescriptionId;
}

bool Tile::triggerEvent(Level *level, int x, int y, int z, int b0, int b1)
{
	return false;
}

bool Tile::isCollectStatistics()
{
	return collectStatistics;
}

Tile *Tile::setNotCollectStatistics()
{
	collectStatistics = false;
	return this;
}

int Tile::getPistonPushReaction()
{
	return material->getPushReaction();
}

// 4J - brought forward from 1.8.2
float Tile::getShadeBrightness(LevelSource *level, int x, int y, int z)
{
	return level->isSolidBlockingTile(x, y, z) ? 0.2f : 1.0f;
}

void Tile::fallOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity, float fallDistance)
{
}

int Tile::cloneTileId(Level *level, int x, int y, int z)
{
	return id;
}

int Tile::cloneTileData(Level *level, int x, int y, int z)
{
	return getSpawnResourcesAuxValue(level->getData(x, y, z));
}

void Tile::playerWillDestroy(Level *level, int x, int y, int z, int data, shared_ptr<Player> player)
{
}

void Tile::onRemoving(Level *level, int x, int y, int z, int data)
{
}

void Tile::handleRain(Level *level, int x, int y, int z)
{
}

void Tile::levelTimeChanged(Level *level, int64_t delta, int64_t newTime)
{
}

bool Tile::useOwnCloneData()
{
	return false;
}

bool Tile::canInstantlyTick()
{
	return true;
}

bool Tile::dropFromExplosion(Explosion *explosion)
{
	return true;
}

bool Tile::isMatching(int id)
{
	return this->id == id;
}

bool Tile::isMatching(int tileIdA, int tileIdB)
{
	if (tileIdA == tileIdB)
	{
		return true;
	}
	if (tileIdA == 0 || tileIdB == 0 || tiles[tileIdA] == nullptr || tiles[tileIdB] == nullptr)
	{
		return false;
	}
	return tiles[tileIdA]->isMatching(tileIdB);
}

bool Tile::hasAnalogOutputSignal()
{
	return false;
}

int Tile::getAnalogOutputSignal(Level *level, int x, int y, int z, int dir)
{
	return Redstone::SIGNAL_NONE;
}

Tile *Tile::setIconName(const wstring &iconName)
{
	this->iconName = iconName;
	return this;
}

wstring Tile::getIconName()
{
	return iconName.empty() ? L"MISSING_ICON_TILE_" + std::to_wstring(id) + L"_" + std::to_wstring(descriptionId) : iconName;
}

void Tile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName());
}

wstring Tile::getTileItemIconName()
{
	return L"";
}

Tile::SoundType::SoundType(eMATERIALSOUND_TYPE eMaterialSound, float volume, float pitch, int iBreakSound, int iPlaceSound)
{
	this->eMaterialSound = eMaterialSound;
	if( iBreakSound>-1 )
	{
		this->iBreakSound = iBreakSound;
	}
	else
	{
		switch(eMaterialSound)
		{
		case eMaterialSoundType_STONE:
			this->iBreakSound=eSoundType_DIG_STONE;
			break;
		case eMaterialSoundType_WOOD:
			this->iBreakSound=eSoundType_DIG_WOOD;
			break;
		case eMaterialSoundType_GRAVEL:
			this->iBreakSound=eSoundType_DIG_GRAVEL;
			break;
		case eMaterialSoundType_GRASS:
			this->iBreakSound=eSoundType_DIG_GRASS;
			break;
		case eMaterialSoundType_METAL:
			this->iBreakSound=eSoundType_DIG_STONE;
			break;
		case eMaterialSoundType_GLASS:
			this->iBreakSound=eSoundType_RANDOM_GLASS;
			break;
		case eMaterialSoundType_CLOTH:
			this->iBreakSound=eSoundType_DIG_CLOTH;
			break;
		case eMaterialSoundType_SAND:
			this->iBreakSound=eSoundType_DIG_SAND;
			break;
		case eMaterialSoundType_SNOW:
			this->iBreakSound=eSoundType_DIG_SNOW;
			break;
		case eMaterialSoundType_LADDER:
			this->iBreakSound=eSoundType_DIG_WOOD;
			break;
		default:
			app.DebugPrintf("NO BREAK SOUND!\n");
			this->iBreakSound=-1;
			break;
		}
		//this->breakSound = L"step." + this->name;
	}

	if( iPlaceSound>-1 )
	{
		this->iPlaceSound = iPlaceSound;
	}
	else
	{
		this->iPlaceSound = this->iBreakSound;
	}

	switch(eMaterialSound)
	{
	case eMaterialSoundType_STONE:
		this->iStepSound=eSoundType_STEP_STONE;
		break;
	case eMaterialSoundType_WOOD:
		this->iStepSound=eSoundType_STEP_WOOD;
		break;
	case eMaterialSoundType_GRAVEL:
		this->iStepSound=eSoundType_STEP_GRAVEL;
		break;
	case eMaterialSoundType_GRASS:
		this->iStepSound=eSoundType_STEP_GRASS;
		break;
	case eMaterialSoundType_METAL:
		this->iStepSound=eSoundType_STEP_METAL;
		break;
	case eMaterialSoundType_CLOTH:
		this->iStepSound=eSoundType_STEP_CLOTH;
		break;
	case eMaterialSoundType_SAND:
		this->iStepSound=eSoundType_STEP_SAND;
		break;
	case eMaterialSoundType_SNOW:
		this->iStepSound=eSoundType_STEP_SNOW;
		break;
	case eMaterialSoundType_LADDER:
		this->iStepSound=eSoundType_STEP_LADDER;
		break;
	default:
		app.DebugPrintf("NO STEP SOUND!\n");

		this->iStepSound=-1;
		break;

	}

	//this->stepSound = L"step." + this->name;
	this->volume = volume;
	this->pitch = pitch;
}

float Tile::SoundType::getVolume() const
{
	return volume;
}
float Tile::SoundType::getPitch() const
{
	return pitch;
}
//wstring getBreakSound() const { return breakSound; }
//wstring getStepSound()	const { return stepSound; }
int Tile::SoundType::getBreakSound() const
{
	return iBreakSound;
}
int Tile::SoundType::getStepSound()	const
{
	return iStepSound;
}
int Tile::SoundType::getPlaceSound() const
{
	return iPlaceSound;
}


/*
4J: These are necessary on the PS3.
(and 4 and Vita).
*/
#if (defined __PS3__ || defined __ORBIS__ || defined __PSVITA__)
const int Tile::stone_Id;
const int Tile::grass_Id;
const int Tile::dirt_Id;
//				4
const int Tile::wood_Id;
const int Tile::sapling_Id;
const int Tile::unbreakable_Id;
const int Tile::water_Id;
const int Tile::calmWater_Id;
const int Tile::lava_Id;
const int Tile::calmLava_Id;
const int Tile::sand_Id;
const int Tile::gravel_Id;
const int Tile::goldOre_Id;
const int Tile::ironOre_Id;
const int Tile::coalOre_Id;
const int Tile::treeTrunk_Id;
const int Tile::leaves_Id;
const int Tile::sponge_Id;
const int Tile::glass_Id;
const int Tile::lapisOre_Id;
const int Tile::lapisBlock_Id;
const int Tile::dispenser_Id;
const int Tile::sandStone_Id;
//				25
const int Tile::bed_Id;
const int Tile::goldenRail_Id;
const int Tile::detectorRail_Id;
const int Tile::pistonStickyBase_Id;
const int Tile::web_Id;
const int Tile::tallgrass_Id;
const int Tile::deadBush_Id;
const int Tile::pistonBase_Id;
const int Tile::pistonExtensionPiece_Id;
const int Tile::wool_Id;
const int Tile::pistonMovingPiece_Id;
const int Tile::flower_Id;
const int Tile::rose_Id;
const int Tile::mushroom_brown_Id;
const int Tile::mushroom_red_Id;
const int Tile::goldBlock_Id;
const int Tile::ironBlock_Id;
const int Tile::stoneSlab_Id;
const int Tile::stoneSlabHalf_Id;
const int Tile::redBrick_Id;
const int Tile::tnt_Id;
const int Tile::bookshelf_Id;
const int Tile::mossyCobblestone_Id;
const int Tile::obsidian_Id;
const int Tile::torch_Id;
const int Tile::fire_Id;
const int Tile::mobSpawner_Id;
const int Tile::stairs_wood_Id;
const int Tile::chest_Id;
const int Tile::redStoneDust_Id;
const int Tile::diamondOre_Id;
const int Tile::diamondBlock_Id;
const int Tile::workBench_Id;
const int Tile::wheat_Id;
const int Tile::farmland_Id;
const int Tile::furnace_Id;
const int Tile::furnace_lit_Id;
const int Tile::sign_Id;
const int Tile::door_wood_Id;
const int Tile::ladder_Id;
const int Tile::rail_Id;
const int Tile::stairs_stone_Id;
const int Tile::wallSign_Id;
const int Tile::lever_Id;
const int Tile::pressurePlate_stone_Id;
const int Tile::door_iron_Id;
const int Tile::pressurePlate_wood_Id;
const int Tile::redStoneOre_Id;
const int Tile::redStoneOre_lit_Id;
const int Tile::redstoneTorch_off_Id;
const int Tile::redstoneTorch_on_Id;
const int Tile::button_stone_Id;
const int Tile::topSnow_Id;
const int Tile::ice_Id;
const int Tile::snow_Id;
const int Tile::cactus_Id;
const int Tile::clay_Id;
const int Tile::reeds_Id;
const int Tile::jukebox_Id;
const int Tile::fence_Id;
const int Tile::pumpkin_Id;
const int Tile::netherRack_Id;
const int Tile::soulsand_Id;
const int Tile::glowstone_Id;
const int Tile::portalTile_Id;
const int Tile::litPumpkin_Id;
const int Tile::cake_Id;
const int Tile::diode_off_Id;
const int Tile::diode_on_Id;
const int Tile::stained_glass_Id;
const int Tile::trapdoor_Id;
const int Tile::monsterStoneEgg_Id;
const int Tile::stoneBrick_Id;
const int Tile::hugeMushroom_brown_Id;
const int Tile::hugeMushroom_red_Id;
const int Tile::ironFence_Id;
const int Tile::thinGlass_Id;
const int Tile::melon_Id;
const int Tile::pumpkinStem_Id;
const int Tile::melonStem_Id;
const int Tile::vine_Id;
const int Tile::fenceGate_Id;
const int Tile::stairs_bricks_Id;
const int Tile::stairs_stoneBrick_Id;
const int Tile::mycel_Id;
const int Tile::waterLily_Id;
const int Tile::netherBrick_Id;
const int Tile::netherFence_Id;
const int Tile::stairs_netherBricks_Id;
const int Tile::netherStalk_Id;
const int Tile::enchantTable_Id;
const int Tile::brewingStand_Id;
const int Tile::cauldron_Id;
const int Tile::endPortalTile_Id;
const int Tile::endPortalFrameTile_Id;
const int Tile::endStone_Id;
const int Tile::dragonEgg_Id;
const int Tile::redstoneLight_Id;
const int Tile::redstoneLight_lit_Id;
const int Tile::woodSlab_Id;
const int Tile::woodSlabHalf_Id;
const int Tile::cocoa_Id;
const int Tile::stairs_sandstone_Id;
const int Tile::stairs_sprucewood_Id;
const int Tile::stairs_birchwood_Id;
const int Tile::stairs_junglewood_Id;
const int Tile::emeraldOre_Id;
const int Tile::enderChest_Id;
const int Tile::tripWireSource_Id;
const int Tile::tripWire_Id;
const int Tile::emeraldBlock_Id;
const int Tile::cobbleWall_Id;
const int Tile::flowerPot_Id;
const int Tile::carrots_Id;
const int Tile::potatoes_Id;
const int Tile::anvil_Id;
const int Tile::button_wood_Id;
const int Tile::skull_Id;
const int Tile::netherQuartz_Id;
const int Tile::quartzBlock_Id;
const int Tile::stairs_quartz_Id;
const int Tile::woolCarpet_Id;
#endif
