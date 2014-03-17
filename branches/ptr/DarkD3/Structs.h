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
   DWORD unknown_06C[11];   // 0x06C
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

//sizeof > 0x1A8
class CRActor  
{ 
public: 
    ULONG id_actor;         // 0x000
    CHAR Name[128];         // 0x004
    ULONG id_sno;           // 0x084
    DWORD unknown_8C[3];    // 0x088 
    Vec3 Rot;               // 0x094
    float fUnk_Rot;         // 0x0A0
    Vec3 Pos;               // 0x0A4
    float fUnk_130;         // 0x0B0
    Vec3 Pos1;              // 0x0B4
    float fUnk_734;         // 0x0C0
    Vec3 Pos2;              // 0x0C4
    DWORD unknown_CC;       // 0x0D0
    float RadiusDefault;    // 0x0D4
    DWORD unknown_D4;       // 0x0D8
    ULONG guid_world;       // 0x0DC
    ULONG guid_scene;       // 0x0E0
    DWORD unknown_E8[8];    // 0x0E4
    Vec3 Pos3;              // 0x104
    DWORD unknown_10C[5];   // 0x110
    DWORD unknown_128;      // 0x124
    DWORD unknown_12C[7];   // 0x128
    Vec3 Pos4;              // 0x144
    DWORD unknown_14C[4];   // 0x150
    DWORD N13E1EB50;        // 0x160
    DWORD unknown_168[12];  // 0x164
    Vec3 PosObj;            // 0x194
    DWORD unknown_1A0;      // 0x1A0
    CActorMovement* Mov;    // 0x1A4
    float Direction;        // 0x1A8

    /*DWORD unknown_388[6];   // 0x1AC
    Vec3 Vel;               //          0x3A4 
    Vec3 Pos6;              //          0x3B0 
    DWORD unknown_3B8[24];  //          0x3BC 
    ULONG Frame;            //          0x41C 
    ULONG Diff;             //          0x420 
    DWORD unknown_420[2];   //          0x424 
    */
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
    DWORD fix2;             // 0x028 Set to 0x120010 for fix
    DWORD unknown_2C[2];    // 0x02C
    ULONG IsMoving;         // 0x034 
    ULONG PathIsWalkable;   // 0x038
    UCHAR unknown_3C[4];    // 0x03C
    Vec3 Tp;                // 0x040 
    DWORD guid_world;       // 0x04C 
    Vec3 Pos2;              // 
    DWORD unknown_5C[9];    // 
    Vec3 MoveTo;            // 
    DWORD guid_world2;      // 
    DWORD unknown_84[3];    //
    DWORD guid_mesh;        //
    DWORD unknown_98[4];    //
    Vec3 Pos3;              //
    DWORD guid_world3;      //
    DWORD unknown_B8;       //
    float Speed2;           //
    DWORD unknown_C0[62];   //
    ULONG id_actor;         //
    ULONG FrameMov;         //
    ULONG Frame;            //
    ULONG FramePrev;        //
    UCHAR unknown_170[4];   //
    float Direction;        //
    DWORD unknown_178[177]; //
};

enum MobRareType
{
    normal        = 0,
    elite         = 1,
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

//sizeof >= 0x12C
template <class T>
class tContainer
{
public:
    CHAR  Name[256];            // 0x000
    ULONG Limit;                // 0x100
    ULONG SizeOf;               // 0x104
    ULONG Count;                // 0x108
    ULONG unknown_10C[5];       // 0x10C
    T**   List;                 // 0x120
    ULONG unknown_124[16];      // 0x124
    ULONG Bits;                 // 0x164
};

//sizeof >= 0x12C
template <class T>
class tContainer2
{
public:
    CHAR  Name[256];            // 0x000
    ULONG Limit;                // 0x100
    ULONG SizeOf;               // 0x104
    ULONG Count;                // 0x108
    ULONG unknown_10C[4];       // 0x10C
    T**   List;                 // 0x11C
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

//sizeof = 0x230
struct tObManStorage
{
    DWORD unknown_0[6];                     // 0x000 
    float fl_120;                           // 0x018
    float fl_1;                             // 0x01C
    DWORD unknown_34[44];                   // 0x020
    DWORD tick_count;                       // 0x0D0
    DWORD unknown_B4[4];                    // 0x0D4
    CObDataContainer* Data;                 // 0x0E4
    DWORD unknown_0C8[7];                   // 0x0E8
    CAttribGroupsContainer* AttribGroups;   // 0x104
    DWORD unknown_0E8[2];                   // 0x108
    tContainer<CACD>** ACD;                 // 0x110
    DWORD unknown_0F4[18];                  // 0x114
    ULONG Mode;                             // 0x15C
    DWORD unknown_138[3];                   // 0x160
    void* Lights;                           // 0x16C
    void* Cutscenes;                        // 0x170
    DWORD unknown_14C;                      // 0x174
    tContainer<CRActor>* Actors;            // 0x178
    DWORD unknown_154;                      // 0x17C
    ULONG ppCloth;                          // 0x180
    ULONG ppExplosion;                      // 0x184
    DWORD unknown_160[5];                   // 0x188
    CameraRaw* CameraPtr;                   // 0x19C
    DWORD unknown_178[3];                   // 0x1A0
    ObManFrames* Frames;                    // 0x1AC
    DWORD unknown_188[3];                   // 0x1B0
    tContainer<SceneRaw>* Scenes;           // 0x1BC
    DWORD unknown_198[9];                   // 0x1C0
    CObMovHPtr* MovHistory;                 // 0x1E4
    ULONG unknown_1C0;                      // 0x1E8
    struct UIManager* ui_mgr;               // 0x1EC
    CameraRaw2 *CameraPtr2;                 // 0x1F0
    tContainer<CWorld>* Worlds;             // 0x1F4
    DWORD unknown_1D0;                      // 0x1F8
    CObLocal* Local;                        // 0x1FC
    ULONG unknown_1F0[12];                  // 0x200
};

//sizeof = 0x9F8
class ObMan  
{ 
public: 
    DWORD Data[490];                // 0x000 
    tObManStorage Storage;          // 0x7A8
}; 

//sizeof = 0x4
class ObManPtr  
{ 
public: 
    ObMan* Ptr;             // 0x000 
}; 

//sizeof = 0x10
struct ObSkill
{
    PowerIds id;            // 0x000
    DWORD rune;             // 0x004
    DWORD unk[2];           // 0x008
};

//sizeof = 0x8
struct ObSkill2
{
    PowerIds id;            // 0x000
    DWORD rune;             // 0x004
};


//sizeof = 0xD0D0
struct tObData  
{ 
    DWORD unknown_0;            // 0x000 
    ULONG id_acd;               // 0x004 
    ULONG id_actor;             // 0x008 
    DWORD unknown_00C[40];      // 0x00C 
    ObSkill skills[6];          // 0x0AC
    DWORD unknown_0FC[1084];    // 0x10C
    ObSkill2 skills2[6];        // 0x11FC
    PowerIds passives[4];       // 0x122C
    DWORD unknown_A14[11041];   // 0x1238
    DWORD seed[2];              // 0xBEC0
    char  charName[96];         // 0xBEC8
    DWORD unk74D0[279];         // 0xBF28
    int   area;                 // 0xC384
    int   player_class;         // 0xC388
}; 

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
    DWORD flags;                    // 0x004 
    DWORD unk8;                     // 0x008
    CAttribFormula* Formula;        // 0x00C 
    DWORD unknown_10[4];            // 0x010 
    CAttribFormula* Formula2;       // 0x020
}; 

class CAttribGroupsContainer  
{ 
public: 
    // sizeof = 0x1C
    struct tWtf 
    { 
        ULONG unknown_0[6];                         // 0x000 
        ULONG _600DF00D;                            // 0x018 
    }; 

    tWtf a;                                         // 0x000 
    tWtf b;                                         // 0x01C 
    tWtf c;                                         // 0x038 
    tContainer<CAttribGroup>* FastAttribGroups;     // 0x054 
    UCHAR unknown_74[32];                           // 0x058
}; 

class CAttribFormula  
{ 
public: 
    DWORD limit;            // 0x000
    DWORD count;            // 0x004
    DWORD unk_008[2];       // 0x008
    DWORD array_base;       // 0x010
};

//sizeof = 0x28
struct AttributeDesc 
{ 
    DWORD id;           // 0x000
    DWORD DefaultVal;   // 0x004 for when trying to get an attribute that doesn't exist in a FastAttributeGroup 
    DWORD unk2;         // 0x008
    DWORD unk3;         // 0x00C
    DWORD Type;         // 0x010 0 = float, 1 = int 
    void* Formula[2];   // 0x014
    char* Name;         // 0x01C
    DWORD unk4;         // 0x020
    unsigned char unk5; // 0x024
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

// sizeof = 0x14
class CSNODef
{
public:
    DWORD unk[4];           // 0x000
    DWORD pSNOAddr;         // 0x010
};

// sizeof >= 0x14
class CSNOGroup
{
public:
    DWORD unk0[4];                  // 0x000
    tContainer2<CSNODef>* pDef[2];  // 0x010
};

// sizeof = 0xD0
class CCharInfo
{
public:
    DWORD unk_000[7];           // 0x000
    char *pBNetTag;             // 0x01C
    DWORD unk_020[4];           // 0x020
    int charID;                 // 0x030
    DWORD unk_034[2];           // 0x034
    char *pName;                // 0x03C
    DWORD unk_040[3];           // 0x040
    int class_idx;              // 0x04C
    DWORD unk_050;              // 0x050
    int level;                  // 0x054
    DWORD unk_058;              // 0x058
    int paragon;                // 0x05C
    DWORD unk_060[23];          // 0x060
    int played;                 // 0x0BC
    __int64 created_on;         // 0x0C0
    DWORD unk_0C4[2];           // 0x0C8
};

// sizeof = 0x158
class CCharContainer
{
public:
    DWORD unk[34];              // 0x000
    CCharInfo chr;              // 0x088
};

class CCharContainer2
{
public:
    DWORD unk[6];               // 0x000
    CCharContainer info;        // 0x018
};


// sizeof = 0x120
class CCharMgr
{
public:
    void** vtable;              // 0x000
    DWORD unk_004[2];           // 0x004
    int selected_idx;           // 0x00C
    DWORD unk_010[28];          // 0x010
    CCharContainer* pCharList;  // 0x080
    DWORD unk_084;              // 0x084
    int count;                  // 0x088
    DWORD unk2[37];             // 0x08C
};


// sizeof == 0x30
class CCharMap
{
public:
    DWORD unk_000;              // 0x000
    CCharContainer2* pFirst;    // 0x004
    CCharContainer2* pFree;     // 0x008
    int limit;                  // 0x00C
    int size;                   // 0x010
    int count;                  // 0x014
    int unk_018;                // 0x018
    int free;                   // 0x01C
    int available;              // 0x020
    DWORD unk_020[3];           // 0x024
};

// sizeof = 0x220
class CCharMgr2
{
public:
    void** vtable;              // 0x000
    DWORD unk_000[42];          // 0x000
    CCharMap* pCharMap;         // 0x0AC
    DWORD unk_0B0[92];          // 0x0B0
};

// sizeof = 0x878
class CBNetService
{
public:
    void** vtable;              // 0x000
    DWORD unk_004[38];          // 0x004
    CCharMgr2* pChrMgr;         // 0x09C
    DWORD unk_0A0[502];         // 0x0A0
};

// sizeof = 0x20
class CMnagers
{
public:
    void* unk_000[4];           // 0x000
    CBNetService* ptr;          // 0x010
    void* unk_014[3];           // 0x014
};

//-------------------------------------
#endif//_STRUCTS_H_