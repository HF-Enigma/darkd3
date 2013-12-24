#ifndef _GB_CLASSES_H_
#define _GB_CLASSES_H_

#include "stdafx.h"
#include "SNOEnums.h"
#include "Structs.h"


//--------------SNO helpers--------------

//sizeof = 0x10
class MemHeader
{
public:
	DWORD id;				// 0x000
	DWORD pad_004[3];		// 0x004
};

//sizeof = 0x20
class FileHeader
{
	DWORD deadbeef;			// 0x000
	DWORD snoGroup;			// 0x004
	DWORD pad_008[2];		// 0x008
	MemHeader mempart;		// 0x010
};

//sizeof = 0x10
class DataPtr
{
public:
	DWORD file_offset;		// 0x000
	DWORD size;				// 0x004
	DWORD mem_offset;		// 0x008
	DWORD unused;			// 0x00C
};

//sizeof = 0x8
class DataPtr2
{
public:
	DWORD file_offset;		// 0x000
	DWORD size;				// 0x004
};

//sizeof = 0x14
class AxialCylinder
{
public:
	Vec3 Position;			// 0x000
	float Ax1;				// 0x00C
	float Ax2;				// 0x010
};

//sizeof = 0x10
class Sphere
{
	Vec3 Position;			// 0x000
	float Radius;			// 0x00C
};

//sizeof = 0x44
class WeightedLook
{
public:
	char LookLinkp[64];		// 0x000
	DWORD pad_040;			// 0x040
};

//sizeof = 0x44
class ActCollData
{
public:
	DWORD ColFlags[4];		// 0x000
	DWORD pad_010;			// 0x010
	AxialCylinder Cylinder;	// 0x014
	AABB aabb;				// 0x028
	float pad_;				// 0x040
};

//sizeof = 0x14
class MonsterDef
{
public:
	float floats[4];		// 0x000
	DWORD intval;			// 0x010
};

//sizeof = 0x10
class MobSkillDecl
{
public:
	float F0;				// 0x000
	float F1;				// 0x004
	DWORD I0;				// 0x008
	float F2;				// 0x00C
};

//sizeof = 0x8
class SkillDecl
{
public:
	DWORD SNOPower;			// 0x000
	DWORD I0;				// 0x004
};

//sizeof = 0xC
class HPDropInfo
{
public:
	float F0;				// 0x000
	DWORD GBID;				// 0x004
	DWORD I1;				// 0x008
};

//sizeof = 0x8
class NavMeshSquare
{
public:
	float z;					// 0x000
	NavCellFlagsDW flags;		// 0x004
};

//sizeof = 0x6
class NavGridSquare
{
public:
	NavCellFlagsW Flags;		// 0x000
	WORD W1;					// 0x002
	WORD CellLookupIndex;		// 0x004
};

//sizeof = 0x20
class NavCell
{
public:
	Vec3 Min;					// 0x000
	Vec3 Max;					// 0x00C
	NavCellFlagsW Flag;			// 0x018
	WORD NeighbourCount;		// 0x01A
	DWORD NeighborsIndex;		// 0x01C
};

//sizeof = 0x4
class NavCellLookup
{
public:
	NavCellFlagsW Flags;		// 0x000
	WORD WCell;					// 0x002
};

//sizeof = 0x128
class NavMeshDef
{
public:
	DWORD SquaresX;				// 0x000
	DWORD SquaresY;				// 0x004
	DWORD pad_008;				// 0x008
	DWORD SquareCount;			// 0x00C
	float pad_010;				// 0x010
	DataPtr2 Squares;			// 0x014
	DWORD pad_01C;				// 0x01C
	NavMeshSquare* array_ptr;	// 0x020
	DWORD pad_024;				// 0x024
	//char name[256];				// 0x028
};

//sizeof = 0x84
class NavZoneDef
{
public:
	DWORD NavCellCount;			// 0x000
	DWORD pad_004[3];			// 0x004
	DataPtr2 NavCells;			// 0x010
	DWORD NeighbourCount;		// 0x018
	DWORD pad_01C[3];			// 0x01C
	DataPtr2 NavCellNeighbours; // 0x024
	float pad_02C[2];			// 0x02C
	DWORD pad_034;				// 0x034
	Vec2 V0;					// 0x038
	DWORD pad_040[3];			// 0x040
	DataPtr2 GridSquares;		// 0x04C
	DWORD pad_054[4];			// 0x054
	DataPtr2 CellLookups;		// 0x064
	DWORD BorderDataCount;		// 0x06C
	DWORD pad_070[3];			// 0x070
	DataPtr2 BorderData;		// 0x07C
};


//--------------Main SNO classes--------------

//sizeof = 0x354
class SNOActor
{
public:
	MemHeader header;		// 0x000
	ActorType type;			// 0x010
	DWORD ApperanceSNO;		// 0x014
	DWORD PhysMeshSNO;		// 0x018
	AxialCylinder Cylinder; // 0x01C
	Sphere sphere;			// 0x030
	AABB AABBBounds;		// 0x040
	DataPtr TagMap;			// 0x058
	DWORD AnimSetSNO;		// 0x068
	DWORD MonsterSNO;		// 0x06C
	DataPtr MsgEvents;		// 0x070
	DWORD pad_080[2];		// 0x080
	Vec3 V0;				// 0x088
	WeightedLook Looks[8];	// 0x094
	DWORD PhysicsSNO;		// 0x2B4
	DWORD pad_2B8[2];		// 0x2B8
	float pad_2C0[3];		// 0x2C0
	ActCollData ColData;	// 0x2CC
	DWORD InvImages[10];	// 0x310
	DWORD pad_338[2];		// 0x338
	DWORD BitField0;		// 0x340
	DataPtr2 CastingNotes;	// 0x344
	DataPtr2 VoiceOverRole;	// 0x34C
};

//sizeof = 0x4AA
class SNOMonster
{
public:
	MemHeader header;			// 0x000
	DWORD ActorSNO;				// 0x010
	DWORD pad_014;				// 0x014
	MonsterType Type;			// 0x018
	MonsterRace Race;			// 0x01C
	DWORD Size;					// 0x020
	MonsterDef mobdef;			// 0x024
	DWORD Resists;				// 0x038
	DWORD pad_03C[2];			// 0x03C
	DWORD lvlNormal;			// 0x044
	DWORD lvlNightmare;			// 0x048
	DWORD lvlHell;				// 0x04C
	DWORD lvlInferno;			// 0x050
	float pad_[139];			// 0x054
	float floats[6];			// 0x280
	DWORD pad_2EC;				// 0x298
	HPDropInfo DropInfo[4];		// 0x29C
	DWORD SNOSkillKit;			// 0x2CC
	SkillDecl skills[8];		// 0x2D0
	MobSkillDecl mobskills[8];	// 0x310
	DWORD SNOTCFirstKill;		// 0x390
	DWORD SNOTC;				// 0x394
	DWORD SNOTCRare;			// 0x398
	DWORD SNOTCChamp;			// 0x39C
	DWORD SNOTCChampLight;		// 0x3A0
	UCHAR pad_3A4[32];			// 0x3A4
	DWORD SNOInv;				// 0x3C4
	DWORD SNOSecondInv;			// 0x3C8
	DWORD SNOLore;				// 0x3CC
	DWORD AIBehavior[6];		// 0x3D0
	DWORD GbidArray0[8];		// 0x3E8
	DWORD SNOSummonActor[6];	// 0x408
	DWORD GbidArray1[4];		// 0x420
	DWORD GbidArray2[6];		// 0x430
	DWORD pad_448[3];			// 0x448
	DWORD PowerType;			// 0x454
	DWORD pad_458[6];			// 0x458
	DataPtr2 TagMap;			// 0x470
	DWORD pad_478[6];			// 0x478
	DataPtr2 MinionSpawn;		// 0x490
	char Name[128];				// 0x498
};

//sizeof = 0x30C
class SNOScene
{
public:
	MemHeader header;			// 0x000
	AABB AABBBounds;			// 0x010
	AABB AABBMarketSetBounds;	// 0x028
	NavMeshDef navmesh;			// 0x040
	DataPtr2 Exclusions;		// 0x168
	DWORD pad_170[14];			// 0x170
	DataPtr2 Inclusions;		// 0x1A8
	DWORD pad_1B0[14];			// 0x1B0
	DataPtr2 MarkerSets;		// 0x1E8
	DWORD pad_1F0[14];			// 0x1F0
	char lootlink[64];			// 0x228
	DataPtr2 MapTrigEvent;		// 0x268
	DWORD pad_270[4];			// 0x270
	NavZoneDef NavZone;			// 0x280
	DWORD SNOAppearance;		// 0x304
	DWORD SNOPhysMesh;			// 0x308
};

//--------------GameBalance Classes--------------

//sizeof 
class SNOGBHeader
{
public:
	DWORD SnoID;					// 0x000
	DWORD unknown_004;				// 0x004
	DWORD count;					// 0x008
	SNOGBType type;					// 0x00C
	DWORD unknown_0210;				// 0x
	DWORD unknown_0214;				// 0x
	DataPtr ItemType;				// 0x
	DataPtr Item;					// 0x
	DataPtr Experience;				// 0x
	DataPtr HelpCodes;				// 0x
	DataPtr MonsterLevel;			// 0x
	DataPtr Affixes;				// 0x
	DataPtr Heros;					// 0x
	DataPtr MovementStyles;			// 0x
	DataPtr Labels;					// 0x
	DataPtr LootDistribution;		// 0x
	DataPtr RareItemNames;			// 0x
	DataPtr MonsterAffixes;			// 0x
	DataPtr RareMonsterNames;		// 0x
	DataPtr SocketedEffects;		// 0x
	DataPtr ItemEnhancement;		// 0x
	DataPtr ItemDropTable;			// 0x
	DataPtr ItemLevelModifiers;		// 0x
	DataPtr QualityClasses;			// 0x
	DataPtr Hirelings;				// 0x
	DataPtr SetItemBonus;			// 0x
	DataPtr EliteModifiers;			// 0x
	DataPtr ItemTiers;				// 0x
	DataPtr PowerFormula;			// 0x
	DataPtr Recipes;				// 0x
	DataPtr SAE;					// 0x
	//char padding[0x10];			// 0x
};

//sizeof = 0x14
class AttribSpec
{
public:
	DWORD AttributeId;		// 0x000
	DWORD SNOParam;			// 0x004
	DWORD pad0[2];			// 0x008
	DataPtr2 Formula;		// 0x00C
};

//sizeof =  0x708
class SNOGBItem
{
public:
	CHAR  Name[0x100];							// 0x000
	DWORD SNOActorID;							// 0x100
	DWORD ItemType1;							// 0x104
	DWORD Flags;								// 0x108
	DWORD I0;									// 0x10C
	DWORD ItemLevel;							// 0x110	
	DWORD eItem;								// 0x114
	DWORD I2;									// 0x118
	DWORD RandomPropertiesCount;				// 0x11C
	DWORD MaxSockets;							// 0x120
	DWORD MaxStackAmount;						// 0x124
	DWORD BaseGoldValue;						// 0x128
	DWORD I7;									// 0x12C
	DWORD RequiredLevel;						// 0x130
	DWORD DurabilityMin;						// 0x134
	DWORD DurabilityDelta;						// 0x138
	DWORD I8;									// 0x13C
	DWORD SNOBaseItem;							// 0x140
	DWORD SNOSet;								// 0x144
	DWORD SNOComponentTreasureClass;			// 0x148
	DWORD SNOComponentTreasureClassMagic;		// 0x14C
	DWORD SNOComponentTreasureClassRare;		// 0x150
	DWORD SNORareNamePrefixStringList;			// 0x154
	DWORD SNORareNameSuffixStringList;			// 0x158
	DWORD I15[4];								// 0x15C
	UCHAR pad1[88];								// 0x16C
	float WeaponDamageMin;						// 0x
	float WeaponDamageDelta;					// 0x
	UCHAR pad2[84];								// 0x
	float ArmorValue;							// 0x
	float F3;									// 0x
	UCHAR pad3[168];							// 0x
	float AttacksPerSecond;						// 0x
	UCHAR pad4[84];								// 0x
	float F4;									// 0x
	float F5;									// 0x
	UCHAR pad5[104];							// 0x
	DWORD SNOSkill0;							// 0x
	DWORD I11;									// 0x
	DWORD SNOSkill1;							// 0x
	DWORD I12;									// 0x
	DWORD SNOSkill2;							// 0x
	DWORD I13;									// 0x
	DWORD SNOSkill3;							// 0x
	DWORD I14;									// 0x
	UCHAR pad6[44];								// 0x
	AttribSpec Attributes[16];					// 0x
	DWORD ItemQuality;							// 0x
	DWORD RecipeToGrant[10];					// 0x
	DWORD EnhancementToGrant;					// 0x
	DWORD LegendaryAffixFamily[6];				// 0x
	DWORD MaxAffixLevel[6];						// 0x
	DWORD I18[6];								// 0x
	DWORD GemType;								// 0x
	DWORD I16;									// 0x
	DWORD Alpha;								// 0x
	DWORD pad7[72];  							// 0x
};

#endif//_GB_CLASSES_H_