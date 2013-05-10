#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "SNOEnums.h"

struct Vec2; 
struct Vec3; 
struct Vec4; 
struct Mat4; 
class CRActor; 
class CWorld; 
class CActorMovement; 
class CACD; 
struct tObManStorage; 
class ObMan; 
class ObManPtr; 
struct tObData; 
class CObDataContainer; 
class CObLocal; 
class CObMovHPtr; 
class CObMovH; 
class CObMov; 
class ObManFrames; 
class CAttribLinks; 
class CAttribGroup; 
class CAttribGroupsContainer; 
class CAttribFormula; 
class CObAttribCoder; 
struct tAttribLink;

class NavMeshRaw;
class NavZoneRaw;
class SceneRaw;


//-------------------------------------

//sizeof = 0x8
struct Vec2
{
    float x;        // 0x000
    float y;        // 0x004

    Vec2& operator -(const Vec2 &v1)
    {
        x -= v1.x;
        y -= v1.y;

        return *this;
    };

    template<typename T>
    Vec2& operator /(const T &v1)
    {
        x /= v1;
        y /= v1;

        return *this;
    };
};

//sizeof = 0xC
struct Vec3
{
    float x;        // 0x000
    float y;        // 0x004
    float z;        // 0x008

    Vec3& operator -(const Vec3 &v1)
    {
        x -= v1.x;
        y -= v1.y;
        z -= v1.z;

        return *this;
    };

    template<typename T>
    Vec3& operator /(const T &v1)
    {
        x /= v1;
        y /= v1;
        z /= v1;

        return *this;
    };
};

//sizeof = 0x18
struct AABB
{
    Vec3 Min;        // 0x000
    Vec3 Max;        // 0x00C
};

//sizeof = 0x54
class NavZoneRaw 
{
public:
   DWORD id_navzone;        // 0x000
   DWORD id_world;          // 0x004
   DWORD id_SceneSNO;       // 0x008
   Vec2 unknown_00C;        // 0x00C
   DWORD unknown_014;       // 0x014
   Vec2 zoneMin;            // 0x018
   Vec2 zoneMax;            // 0x020
   DWORD unknown_028[8];    // 0x028
   DWORD pNavZoneDefRaw;    // 0x048
   DWORD pSceneRecord;      // 0x04C
   DWORD unknown_050;       // 0x050
};

//sizeof = 0x1C8
class NavMeshRaw 
{
public:
   DWORD id_navmesh;        // 0x000
   DWORD id_world;          // 0x004
   DWORD unknown_008[3];    // 0x008
   DWORD sno_levelarea;     // 0x014
   DWORD unknown_018[3];    // 0x018
   DWORD sno_portalWorld;   // 0x024
   DWORD unknown_028[10];   // 0x028
   float unknown_050;       // 0x050
   DWORD unknown_054[5];    // 0x054
   float unknown_068;       // 0x068
   DWORD unknown_06C[8];    // 0x06C
   DWORD sizeX;             // 0x08C
   DWORD sizeY;             // 0x090
   AABB  unknown_094;       // 0x094
   AABB  unknown_0AC;       // 0x0AC
   DWORD unknown_0C4[5];    // 0x0C4
   DWORD id_SceneSNO;       // 0x0D8
   DWORD unknown_0DC[4];    // 0x0DC
   Vec2 MeshMin;            // 0x0EC
   DWORD unknown_0F4[16];   // 0x0F4
   AABB bounds;             // 0x134
   AABB boundsMarker;       // 0x14C
   Vec2 MeshMax;            // 0x164
   float unknown_16C[3];    // 0x16C
   DWORD unknown_178;       // 0x178
   NavZoneRaw* pZone;       // 0x17C
   DWORD unknown_180[9];    // 0x180
   DWORD unknown_1A4;       // 0x1A4 Read by D3::NavMesh::GetFlags
   DWORD unknown_1A8;       // 0x1A8
   DWORD unknown_1AC[7];    // 0x1AC
};

//sizeof = 0x2A8
class SceneRaw 
{
public:
    int id_scene;           // 0x000
    NavMeshRaw navmesh;     // 0x004
    DWORD unknown_1CC[55];  // 0x1CC
};

//sizeof = 0x42C
class CRActor  
{ 
public: 
    ULONG id_actor;         // 0x000
    ULONG id_acd;           // 0x004
    CHAR Name[128];         // 0x008
    ULONG id_sno;           // 0x088
    DWORD unknown_8C;       // 0x08C 
    Vec3 Rot;               // 0x090 
    float fUnk_Rot;         // 0x09C 
    Vec3 Pos;               // 0x0A0
    float fUnk_130;         // 0x0AC 
    Vec3 Pos1;              // 0x0B0 
    float fUnk_734;         // 0x0BC 
    Vec3 Pos2;              // 0x0C0 
    DWORD unknown_CC;       // 0x0CC 
    float RadiusDefault;    // 0x0D0 
    DWORD unknown_D4;       // 0x0D4 
    ULONG guid_world;       // 0x0D8 
    ULONG guid_scene;       // 0x0DC 
    DWORD unknown_E0[8];    // 0x0E0 
    Vec3 Pos3;              // 0x100 
    DWORD unknown_10C[5];   // 0x10C 
    DWORD unknown_120;      // 0x120
    DWORD unknown_124[7];   // 0x124
    Vec3 Pos4;              // 0x140 
    DWORD unknown_14C[4];   // 0x14C 
    DWORD N13E1EB50;        // 0x15C 
    DWORD unknown_160[45];  // 0x160 
    Vec3 PosObj;            // 0x214 
    DWORD unknown_21C[89];  // 0x220 
    CActorMovement* Mov;    // 0x384
    float Direction;        // 0x388 
    DWORD unknown_388[6];   // 0x38C 
    Vec3 Vel;               // 0x3A4 
    Vec3 Pos6;              // 0x3B0 
    DWORD unknown_3B8[24];  // 0x3BC 
    ULONG Frame;            // 0x41C 
    ULONG Diff;             // 0x420 
    DWORD unknown_420[2];   // 0x424 
};

//sizeof = 0x48
class CWorld  
{ 
public: 
    DWORD unknown_0;        // 0x000 
    DWORD sno_id;           // 0x004
    DWORD id;               // 0x008
    void* pMatrix;          // 0x00C
    Vec3* a;                // 0x010 
    Vec3* b;                // 0x014 
    DWORD unknown_18[12];   // 0x018 
};

//sizeof = 0x43C ?
class CActorMovement  
{ 
public: 
    ULONG* vt;              // 0x000 
    ULONG Active;           // 0x004 
    float Speed;            // 0x008 
    float SpeedCurrent;     // 0x00C 
    DWORD fix;              // 0x010
    DWORD unknown_14[2];    // 0x014 
    float ScaleSize;        // 0x01C 
    ULONG Flags;            // 0x020 
    DWORD unknown_24;       // 0x024
    DWORD fix2;             // 0x028 Set to 0x00220018 for fix
    DWORD unknown_2C[2];    // 0x02C
    ULONG IsMoving;         // 0x034 
    ULONG PathIsWalkable;   // 0x038
    UCHAR unknown_3C[4];    // 0x03C
    Vec3 MovTo;             // 0x040 
    DWORD guid_world;       // 0x04C 
    Vec3 Pos2;              // 0x050 
    float unknown_5C[2];    // 0x05C 
    DWORD unknown_64[5];    // 0x064 
    Vec3 Tp;                // 0x078 
    UCHAR unknown_84[8];    // 0x084 
    ULONG unk;              // 0x08c 
    DWORD unknown_90;       // 0x090 
    DWORD guid_mesh;        // 0x094
    DWORD unknown_98[4];    // 0x098
    Vec3 Pos3;              // 0x0A8 
    DWORD guid_world2;      // 0x0B4 
    DWORD unknown_B8;       // 0x0B8
    float Speed2;           // 0x0BC 
    DWORD unknown_C0[40];   // 0x0C0 
    ULONG id_actor;         // 0x160 
    ULONG FrameMov;         // 0x164 
    ULONG Frame;            // 0x168 
    ULONG FramePrev;        // 0x16C 
    UCHAR unknown_170[4];   // 0x170 
    float Direction;        // 0x174 
    DWORD unknown_178[177]; // 0x178
};

enum MobRareType
{
    normal        = 0,
    elite        = 1,
    yellow        = 2,
    purple        = 3,
    key_warden    = 4
};

//sizeof = 0x2D0
class CACD
{
public:
    DWORD id_acd;               // 0x000
    char name[128];             // 0x004
    DWORD unk_0;                // 0x084 
    DWORD id_unk1;              // 0x088 
    DWORD id_actor;             // 0x08C 
    DWORD id_sno;               // 0x090 
    char unknown_94[28];        // 0x094 
    DWORD gb_type;              // 0x0B0
    DWORD id_gb;                // 0x0B4  
    MobRareType rtype;          // 0x0B8
    char unknown_B8[20];        // 0x0BC
    Vec3 PosWorld;              // 0x0D0 
    char unknown_DC[36];        // 0x0DC 
    float RadiusDefault;        // 0x100 
    char unknown_104[4];        // 0x104 
    DWORD id_world;             // 0x108 
    char unknown_10C[4];        // 0x10C 
    DWORD id_owner;             // 0x110 
    ItemLocation itemLoc;       // 0x114
    DWORD itemX;                // 0x118
    DWORD itemY;                // 0x11C
    DWORD id_attrib;            // 0x120         
    DWORD id_unk3;              // 0x124 
    char unknown_128[244];      // 0x128 
    char NBAD848;               // 0x21C 
    char RadiusType;            // 0x21D 
    char NBB56E9;               // 0x21E 
    char NBBA038;               // 0x21F 
    char unknown_220[24];       // 0x220 
    float RadiusScaled;         // 0x238 
    char unknown_23C[148];      // 0x23C 
};

//sizeof = 0x1D0
template <class T>
class tContainer
{
public:
    CHAR  Name[256];            // 0x000
    ULONG Limit;                // 0x100
    ULONG SizeOf;               // 0x104
    ULONG Count;                // 0x108
    UCHAR unknown_10C[60];      // 0x10C
    T**   List;                 // 0x148
    UCHAR unknown_14C[64];      // 0x14C
    ULONG Bits;                 // 0x18C
    UCHAR unknown_190[64];      // 0x190
};

//sizeof = 0x94
struct CameraSub
{
    DWORD pad_04C[3];           // 0x000
    float unk_008[2];           // 0x008
    float zoom;                 // 0x014
    float unk_018;              // 0x018
    DWORD actor_id;             // 0x01C
    DWORD unk_020[2];           // 0x020
    float unk_028[24];          // 0x028
    Vec3  position;             // 0x088
};

//sizeof = 0x198
struct CameraRaw
{
    DWORD pad_000[18];          // 0x000
    CameraSub* ptr;             // 0x048
    float pad_04C[42];          // 0x04C
    DWORD cam_mode;             // 0x0F4    0 - free, 2 - snapped, 3 - AV
};

//sizeof = 0x74
struct CameraRaw2
{
    DWORD unk_000;              // 0x000
    float rot1;                 // 0x004
    float rot2;                 // 0x008
    float rot3;                 // 0x01C
    float rot4;                 // 0x010
    Vec3  position;             // 0x014
    float pad_020[3];           // 0x020
    float fovY;                 // 0x02C
    float fovX;                 // 0x030
    float unk_034[16];          // 0x034    some valid data
};

//sizeof = 0x210
struct tObManStorage
{
    DWORD unknown_0[11];                    // 0x000 
    float fl_120;                           // 0x02C 
    float fl_1;                             // 0x030 
    DWORD unknown_34[31];                   // 0x034 
    DWORD tick_count;                       // 0x0B0
    DWORD unknown_B4[4];                    // 0x0B4
    CObDataContainer* Data;                 // 0x0C4 
    DWORD unknown_0C8[7];                   // 0x0C8 
    CAttribGroupsContainer* AttribGroups;   // 0x0E4 
    DWORD unknown_0E8[2];                   // 0x0E8 
    tContainer<CACD>** ACD;                 // 0x0F0 
    DWORD unknown_0F4[16];                  // 0x0F4 
    ULONG Mode;                             // 0x134 
    DWORD unknown_138[3];                   // 0x138
    void* Lights;                           // 0x144
    void* Cutscenes;                        // 0x148
    UCHAR unknown_14C[4];                   // 0x14C
    tContainer<CRActor>* Actors;            // 0x15C
    UCHAR unknown_160[4];                   // 0x160
    ULONG ppCloth;                          // 0x170
    ULONG ppExplosion;                      // 0x174
    DWORD unknown_178[5];                   // 0x178
    CameraRaw* CameraPtr;                   // 0x18C
    DWORD unknown_190[3];                   // 0x190
    ObManFrames* Frames;                    // 0x19C
    DWORD unknown_1A0[3];                   // 0x1A0
    tContainer<SceneRaw>* Scenes;           // 0x1AC
    DWORD unknown_17C[2];                   // 0x1B0
    CObMovHPtr* MovHistory;                 // 0x1B8
    ULONG unknown_1BC;                      // 0x1BC
    ULONG ui_mgr;                           // 0x1DC
    CameraRaw2 *CameraPtr2;                 // 0x1E0
    tContainer<CWorld>* Worlds;             // 0x1E4
    DWORD unknown_1E8;                      // 0x1E8
    CObLocal* Local;                        // 0x1EC
    ULONG unknown_1F0[8];                   // 0x1F0
};

//sizeof = 0x9C0
class ObMan  
{ 
public: 
    struct tPad 
    { 
        DWORD unknown_0[14];        // 0x000 
        ULONG FrameCurrent;         // 0x038 
        DWORD unknown_3C[477];      // 0x03C 
    }; 

    tPad Data;                      // 0x000 
    tObManStorage Storage;          // 0x7B0
}; 

//sizeof = 0x4
class ObManPtr  
{ 
public: 
    ObMan* Ptr;             // 0x000 
}; 

//sizeof = 0xC
struct ObSkill
{
    PowerIds id;            // 0x000
    DWORD rune;             // 0x004
    DWORD unk;              // 0x008
};

//sizeof = 0x7FA0
struct tObData  
{ 
    DWORD unknown_0;        // 0x000 
    ULONG id_acd;           // 0x004 
    ULONG id_actor;         // 0x008 
    DWORD unknown_00C[42];  // 0x00C 
    ObSkill skills[6];      // 0x0B4
    DWORD unknown_0FC[579]; // 0x0FC
    PowerIds passives[3];   // 0xA08
    DWORD unknown_A14[6797];// 0xA14
    DWORD seed[2];          // 0x7448
    char  charName[64];     // 0x7450
    char  unk7490[64];      // 0x7490
    DWORD unk74D0[266];     // 0x74D0
    int   area;             // 0x78F8
    int   player_class;     // 0x78FC
    DWORD unk7900[424];     // 0x7900
}; 

//sizeof = 0x7FF8
class CObDataContainer  
{ 
public: 
    DWORD unknown_0[22];    // 0x000 
    tObData Data;           // 0x058 
}; 

//sizeof = 0x60
class CObLocal  
{ 
public: 
    ULONG Index;            // 0x000 
    UCHAR unknown_4[92];    // 0x004 
}; 

class CObMovHPtr  
{ 
public: 
    UCHAR unknown_0[8];     // 0x000 
    CObMovH* Ptr;           // 0x008 
    UCHAR unknown_C[12];    // 0x00C 
}; 

class CObMovH  
{ 
public: 
    CObMov* Ptr;            // 0x000 
    UCHAR unknown_4[60];    // 0x004 
}; 

class CObMov  
{ 
public: 
    UCHAR unknown_0[20];    // 0x000 
    CObMov* Right;          // 0x014 
    CObMov* Left;           // 0x018 
    CObMov* Parent;         // 0x01C 
    UCHAR unknown_20[32];   // 0x020 
}; 

class ObManFrames  
{ 
public: 
    ULONG Max;              // 0x000 
    UCHAR unknown_4[12];    // 0x004 
    ULONG N13DC069C;        // 0x010 
    UCHAR unknown_14[108];  // 0x014 
}; 

class CAttribLinks  
{ 
public: 
    UCHAR unknown_0[132];     // 0x000 
}; 

template <class T, UINT32 I>
struct tMap
{ 
    T* Ptr;         // 0x000 Data ptr 
    ULONG Unk;      // 0x004
    ULONG Count;    // 0x008 
    T Data[I];      // 0x00C
    ULONG Mask;     //
}; 

class CAttribGroup  
{ 
public: 
    ULONG id_attrib;                // 0x000 
    UCHAR unknown_4[12];            // 0x004 
    CAttribFormula* Formula;        // 0x010 
    UCHAR unknown_14[36];           // 0x014 
    tMap<tAttribLink *, 33> Links;  // 0x038 
    UCHAR unknown_CC[144];          // 0x0CC 
}; 

class CAttribGroupsContainer  
{ 
public: 
    struct tWtf 
    { 
        UCHAR unknown_0[24];                        // 0x000 
        ULONG _600DF00D;                            // 0x018 
    }; 

    tWtf a;                                         // 0x000 
    tWtf b;                                         // 0x01C 
    tWtf c;                                         // 0x038 
    tWtf d;                                         // 0x054 
    tContainer<CAttribGroup>* FastAttribGroups;     // 0x070 
    UCHAR unknown_74[32];                           // 0x074 
}; 

class CAttribFormula  
{ 
public: 
    UCHAR unknown_0[8];             // 0x000 
    tMap<tAttribLink *, 257> Map;   // 0x008 
    UCHAR unknown_41C[56];          // 0x41C 
};

//sizeof = 0x28
struct AttributeDesc 
{ 
    DWORD unk0;         // 0x000
    DWORD DefaultVal;   // 0x004 for when trying to get an attribute that doesn't exist in a FastAttributeGroup 
    DWORD unk2;         // 0x008
    DWORD Type;         // 0x010 0 = float, 1 = int 
    void* Formula1;     // 0x014
    void* Formula2;     // 0x018
    char* Name;         // 0x01C
    DWORD unk3;         // 0x020
    void* unk5;         // 0x024
    DWORD id;           // 0x028
}; 

class CObAttribCoder  
{ 
public: 
    UCHAR unknown_0[4];     // 0x000 
    LONG i;                 // 0x004 
    CObAttribCoder* Next;   // 0x008 
    ULONG Bits;             // 0x00C 
}; 

struct tAttribLink  
{ 
    tAttribLink* Next;      // 0x000 
    LONG AttribIndex;       // 0x004 
    BYTE Value[4];          // 0x008 
};

//sizeof = 0x28
struct usePowerData
{
    PowerIds power_1;       // 0x000
    PowerIds power_2;       // 0x004
    DWORD cmd;              // 0x008
    DWORD acd_id;           // 0x00C
    Vec3 pos;               // 0x010
    DWORD world_id;         // 0x01C
    DWORD end;              // 0x020
    DWORD zero;             // 0x024
};

// sizeof = 0x10 
class CSNODef
{
public:
    DWORD unk[3];           // 0x000
    DWORD pSNOAddr;         // 0x00C
};

// sizeof >= 0x14
class CSNOGroup
{
public:
    DWORD unk0[4];                  // 0x000
    tContainer<CSNODef>* pDef;      // 0x010
};

//-------------------------------------
#endif//_STRUCTS_H_