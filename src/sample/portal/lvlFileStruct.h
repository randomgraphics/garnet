//--------------------------------------------------------------------------------------------------
// File:            lvlFileStruct.h
//
// Author:            原林
//
// Created Date:    2002/6/28
//
// Purpose:            lvl文件的结构，用于输出文件
//--------------------------------------------------------------------------------------------------
#ifndef _LVLFILESTRUCT_H_
#define _LVLFILESTRUCT_H_

#include "math.h"

#define LVL_FILE_VERSION 100

#define LVL_FILE_DEFAULT_RESERVE 1

#define LVL_FILE_SAFE_DELETE_ARRAY(x) \
    if (x) delete[](x), (x) = 0

//
// lvlTypeArray
// ===========
//
template<typename T>
struct lvlTypeArray {
    uint length;
    T *  data;

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&length, sizeof(T), 1, fp);
        fwrite(data, sizeof(T), length, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&length, sizeof(T), 1, fp);
        data = new T[length];
        fread(data, sizeof(T), length, fp);
        return true;

        GN_UNGUARD;
    }

    lvlTypeArray(): length(0), data(0) {}
    ~lvlTypeArray() { LVL_FILE_SAFE_DELETE_ARRAY(data); }
};
typedef lvlTypeArray<int>  lvlIntArray;
typedef lvlTypeArray<char> lvlCharArray;

//
// lvlStructArray
// ============
//
template<class T>
struct lvlStructArray {
    uint length;
    T *  data;

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&length, sizeof(int), 1, fp);
        T * p = data;
        for (uint i = 0; i < length; ++p, ++i) p->Write(fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&length, sizeof(int), 1, fp);
        T * p = data = new T[length];
        for (uint i = 0; i < length; ++p, ++i) p->Read(fp);
        return true;

        GN_UNGUARD;
    }

    lvlStructArray(): length(0), data(0) {}
    ~lvlStructArray() { LVL_FILE_SAFE_DELETE_ARRAY(data); }
};

//
// lvlName
// =======
//
//
struct lvlName {
    char name[64];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(name, sizeof(char), 64, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(name, sizeof(char), 64, fp);
        return true;

        GN_UNGUARD;
    }
};

//----------------------------------------------------------------------------------
// struct lvlHdr
// lvl文件头
//----------------------------------------------------------------------------------
struct lvlHdr {
    int  version;         //文件版本
    char textureLib[128]; //材质库文件名

    //    int                    num_sector;            //sector数量
    //    int                    num_portal;            //portal数量

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(this, sizeof(lvlHdr), 1, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(this, sizeof(lvlHdr), 1, fp);
        if (LVL_FILE_VERSION != version) {
            GN_ERROR(GN_T("incorrect level version!"));
            return false;
        }
        return true;

        GN_UNGUARD;
    }
};

//----------------------------------------------------------------------------------
// struct lvlPortal
// lvl文件portal结构
//----------------------------------------------------------------------------------
struct lvlPortal {
    char       name[64];       //关卡编辑器中的名字，用来与脚本连接
    uint       frontSectorID;  // portal面对连接的sector
    uint       backSectorID;   // portal背对连接的sector
    Plane3f    plane;          //所在平面
    uint       numVert;        //多边形顶点数量
    Vector3f * vList;          //多边形顶点列表
    char       scriptName[64]; //脚本文件名
    char       reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(name, sizeof(char), 64, fp);
        fwrite(&frontSectorID, sizeof(int), 1, fp);
        fwrite(&backSectorID, sizeof(int), 1, fp);
        fwrite(&plane, sizeof(Plane3f), 1, fp);
        fwrite(&numVert, sizeof(int), 1, fp);
        fwrite(vList, sizeof(Vector3f), numVert, fp);
        fwrite(scriptName, sizeof(char), 64, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(name, sizeof(char), 64, fp);
        fread(&frontSectorID, sizeof(int), 1, fp);
        fread(&backSectorID, sizeof(int), 1, fp);
        fread(&plane, sizeof(Plane3f), 1, fp);
        fread(&numVert, sizeof(int), 1, fp);
        vList = new Vector3f[numVert];
        fread(vList, sizeof(Vector3f), numVert, fp);
        fread(scriptName, sizeof(char), 64, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlPortal(): vList(0) {}
    ~lvlPortal() { LVL_FILE_SAFE_DELETE_ARRAY(vList); }
};

//----------------------------------------------------------------------------------
// struct lvlFace
// 用于存储表面的属性，代表一个brushside
//----------------------------------------------------------------------------------
#define LVLFACE_DEFAULT 0
#define LVLFACE_SKY     1 << 0
#define LVLFACE_LIGHT   1 << 1

#define LVL_TEXID(classid, tid) (classid << 16) + (tid)

struct lvlFace {
    Plane3f plane; // face所在的平面

    unsigned int texId; // face的贴图在材质库中的索引，前16位表示class，后16位表示id

    int lightmapId; //此面对应的lightmap id

    float lightmapSize; //此面的lightmap比例

    unsigned int property; //面属性

    Vector3f faceClr; //面光源的颜色

    char scriptName[64]; //脚本文件名

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(this, sizeof(lvlFace), 1, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(this, sizeof(lvlFace), 1, fp);
        return true;

        GN_UNGUARD;
    }
};

//----------------------------------------------------------------------------------
// struct lvlDrawFace
// 需要渲染的结构表面
//----------------------------------------------------------------------------------
#define LVLDRAWFACE_DEFAULT 0
#define LVLDRAWFACE_WATER   1 << 0
#define LVLDRAWFACE_FOG     1 << 2
#define LVLDRAWFACE_LAVA    1 << 3

struct lvlDrawFace {
    uint faceId; //所属的lvlFace的列表id

    uint num_vert; // polygon的顶点数

    Vector3f * vList; //顶点列表

    vec2_c * uvList; //对应的uv列表

    vec2_c * luvList; //对应的lightmap的uv列表

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&faceId, sizeof(int), 1, fp);
        fwrite(&num_vert, sizeof(int), 1, fp);
        fwrite(vList, sizeof(Vector3f), num_vert, fp);
        fwrite(uvList, sizeof(vec2_c), num_vert, fp);
        fwrite(luvList, sizeof(vec2_c), num_vert, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&faceId, sizeof(int), 1, fp);
        fread(&num_vert, sizeof(int), 1, fp);
        vList   = new Vector3f[num_vert];
        uvList  = new vec2_c[num_vert];
        luvList = new vec2_c[num_vert];
        fread(vList, sizeof(Vector3f), num_vert, fp);
        fread(uvList, sizeof(vec2_c), num_vert, fp);
        fread(luvList, sizeof(vec2_c), num_vert, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlDrawFace(): vList(0), uvList(0), luvList(0) {}
    ~lvlDrawFace() {
        LVL_FILE_SAFE_DELETE_ARRAY(vList);
        LVL_FILE_SAFE_DELETE_ARRAY(uvList);
        LVL_FILE_SAFE_DELETE_ARRAY(luvList);
    }
};

//----------------------------------------------------------------------------------
// struct lvlCollideFace
// 碰撞监测表面
//----------------------------------------------------------------------------------
struct lvlCollideFace {
    int faceId; //所属的lvlFace的列表id

    uint portalId; //如果faceId为-1，表示此表面源于portal，portalId表示portal id

    uint num_vert; // polygon的顶点数

    Vector3f * vList; //顶点列表

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&faceId, sizeof(int), 1, fp);
        fwrite(&portalId, sizeof(int), 1, fp);
        fwrite(&num_vert, sizeof(int), 1, fp);
        fwrite(vList, sizeof(Vector3f), num_vert, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&faceId, sizeof(int), 1, fp);
        fread(&portalId, sizeof(int), 1, fp);
        fread(&num_vert, sizeof(int), 1, fp);
        vList = new Vector3f[num_vert];
        fread(vList, sizeof(Vector3f), num_vert, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlCollideFace(): vList(0) {}
    ~lvlCollideFace() { LVL_FILE_SAFE_DELETE_ARRAY(vList); }
};

//----------------------------------------------------------------------------------
// struct lvlNonStructFace
// 非结构性表面
//----------------------------------------------------------------------------------
struct lvlNonStructFace {
    unsigned int type; // fog,water,lava......

    unsigned int texId; // face的贴图在材质库中的索引，前16位表示class，后16位表示id

    uint num_vert; // polygon的顶点数

    Vector3f * vList; //顶点列表

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&type, sizeof(unsigned int), 1, fp);
        fwrite(&texId, sizeof(unsigned int), 1, fp);
        fwrite(&num_vert, sizeof(int), 1, fp);
        fwrite(vList, sizeof(Vector3f), num_vert, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&type, sizeof(int), 1, fp);
        fread(&texId, sizeof(unsigned int), 1, fp);
        fread(&num_vert, sizeof(int), 1, fp);
        vList = new Vector3f[num_vert];
        fread(vList, sizeof(Vector3f), num_vert, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlNonStructFace(): vList(0) {}
    ~lvlNonStructFace() { LVL_FILE_SAFE_DELETE_ARRAY(vList); }
};

//----------------------------------------------------------------------------------
// struct lvlBspNode
// 非结构性表面
//----------------------------------------------------------------------------------
struct lvlBspNode {
    Plane3f plane; // node的平面

    uint num_face; // lvlCollideFace的数量

    uint * faceId; // lvlCollideFace的id

    int parent; //父节点索引，-1表示根节点

    int child[2]; //前后子节点索引，-1表示空

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&plane, sizeof(Plane3f), 1, fp);
        fwrite(&num_face, sizeof(int), 1, fp);
        fwrite(faceId, sizeof(int), num_face, fp);
        fwrite(&parent, sizeof(int), 1, fp);
        fwrite(&child, sizeof(int), 2, fp);
        //        fwrite( reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp );
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&plane, sizeof(Plane3f), 1, fp);
        fread(&num_face, sizeof(int), 1, fp);
        faceId = new uint[num_face];
        fread(faceId, sizeof(int), num_face, fp);
        fread(&parent, sizeof(int), 1, fp);
        fread(&child, sizeof(int), 2, fp);
        //        fread( reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp );
        return true;

        GN_UNGUARD;
    }

    lvlBspNode(): faceId(0) {}
    ~lvlBspNode() { LVL_FILE_SAFE_DELETE_ARRAY(faceId); }
};

//----------------------------------------------------------------------------------
// struct lvlCurve
// 曲面
//----------------------------------------------------------------------------------
#define LVLCURVE_DEFAULT 0
#define LVLCURVE_LIGHT   1 << 0

struct lvlCurve {
    char name[64]; //关卡编辑器中的名字，用来与脚本连接

    unsigned int texId; // face的贴图在材质库中的索引

    uint numCtrlPtW, numCtrlPtH; //控制点数量

    Vector3f * ctrlPtList; //控制点列表，按行排列

    uint numSampleW, numSampleH; //采样点数量，不是采样率

    Vector3f * sampleClrList; //采样点颜色列表，按行排列

    vec2_c * sampleUVList; //采样点uv列表，按行排列

    unsigned int property; // face light....

    Vector3f color; // face light color

    Vector3f center; // center of the curve

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(name, sizeof(char), 64, fp);
        fwrite(&texId, sizeof(int), 1, fp);

        fwrite(&numCtrlPtW, sizeof(int), 1, fp);
        fwrite(&numCtrlPtH, sizeof(int), 1, fp);
        fwrite(ctrlPtList, sizeof(Vector3f), numCtrlPtW * numCtrlPtH, fp);

        fwrite(&numSampleW, sizeof(int), 1, fp);
        fwrite(&numSampleH, sizeof(int), 1, fp);
        fwrite(sampleClrList, sizeof(Vector3f), numSampleW * numSampleH, fp);
        fwrite(sampleUVList, sizeof(vec2_c), numSampleW * numSampleH, fp);

        fwrite(&property, sizeof(int), 1, fp);
        fwrite(&color, sizeof(Vector3f), 1, fp);
        fwrite(&center, sizeof(Vector3f), 1, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(name, sizeof(char), 64, fp);
        fread(&texId, sizeof(int), 1, fp);

        fread(&numCtrlPtW, sizeof(int), 1, fp);
        fread(&numCtrlPtH, sizeof(int), 1, fp);
        ctrlPtList = new Vector3f[numCtrlPtW * numCtrlPtH];
        fread(ctrlPtList, sizeof(Vector3f), numCtrlPtW * numCtrlPtH, fp);

        fread(&numSampleW, sizeof(int), 1, fp);
        fread(&numSampleH, sizeof(int), 1, fp);
        sampleClrList = new Vector3f[numSampleW * numSampleH];
        sampleUVList  = new vec2_c[numSampleW * numSampleH];
        fread(sampleClrList, sizeof(Vector3f), numSampleW * numSampleH, fp);
        fread(sampleUVList, sizeof(vec2_c), numSampleW * numSampleH, fp);

        fread(&property, sizeof(int), 1, fp);
        fread(&color, sizeof(Vector3f), 1, fp);
        fread(&center, sizeof(Vector3f), 1, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlCurve(): sampleClrList(0), sampleUVList(0) {}
    ~lvlCurve() {
        LVL_FILE_SAFE_DELETE_ARRAY(sampleClrList);
        LVL_FILE_SAFE_DELETE_ARRAY(sampleUVList);
    }
};

//----------------------------------------------------------------------------------
// struct lvlModel
// 模型
//----------------------------------------------------------------------------------
struct lvlModel {
    char name[64]; //关卡编辑器中的名字，用来与脚本连接

    uint modelNameId; //模型文件名列表的id

    Vector3f trans; //平移

    Vector3f rotate; //旋转

    uint num_vert; //模型的顶点数

    Vector3f * vClrList; //顶点颜色列表

    char scriptName[64]; //脚本文件名

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(name, sizeof(char), 64, fp);
        fwrite(&modelNameId, sizeof(int), 1, fp);
        fwrite(&trans, sizeof(Vector3f), 1, fp);
        fwrite(&rotate, sizeof(Vector3f), 1, fp);
        fwrite(&num_vert, sizeof(int), 1, fp);
        fwrite(vClrList, sizeof(Vector3f), num_vert, fp);
        fwrite(&scriptName, sizeof(char), 64, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(name, sizeof(char), 64, fp);
        fread(&modelNameId, sizeof(int), 1, fp);
        fread(&trans, sizeof(Vector3f), 1, fp);
        fread(&rotate, sizeof(Vector3f), 1, fp);
        fread(&num_vert, sizeof(int), 1, fp);
        vClrList = new Vector3f[num_vert];
        fread(vClrList, sizeof(Vector3f), num_vert, fp);
        fread(&scriptName, sizeof(char), 64, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlModel(): vClrList(0) {}
    ~lvlModel() { LVL_FILE_SAFE_DELETE_ARRAY(vClrList); }
};

//----------------------------------------------------------------------------------
// struct lvlPointLight
// 灯光
//----------------------------------------------------------------------------------
struct lvlPointLight {
    Vector3f pos;

    float dist;

    Vector3f color;

    unsigned int property;

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(this, sizeof(lvlPointLight), 1, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(this, sizeof(lvlPointLight), 1, fp);
        return true;

        GN_UNGUARD;
    }
};

//----------------------------------------------------------------------------------
// struct lvlSpotLight
// 灯光
//----------------------------------------------------------------------------------
struct lvlSpotLight {
    Vector3f pos;

    Vector3f dir;

    float dist;

    int fov; // fov in degree;

    Vector3f color;

    unsigned int property;

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(this, sizeof(lvlSpotLight), 1, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(this, sizeof(lvlSpotLight), 1, fp);
        return true;

        GN_UNGUARD;
    }
};

#define ENTITY_USERDATA_SIZE 256

struct lvlEntity {
    //  [9/11/2002]
    // entity type id, defined by game
    uint id;

    // sector id [9/11/2002]
    uint sectorId;

    Vector3f pos;

    // defined by game
    char userData[ENTITY_USERDATA_SIZE];
    int  userDataSize;

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&id, sizeof(int), 1, fp);
        fwrite(&sectorId, sizeof(int), 1, fp);
        fwrite(&pos, sizeof(Vector3f), 1, fp);
        fwrite(&userDataSize, sizeof(int), 1, fp);
        if (userDataSize) fwrite(userData, sizeof(char), userDataSize, fp);

        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&id, sizeof(int), 1, fp);
        fread(&sectorId, sizeof(int), 1, fp);
        fread(&pos, sizeof(Vector3f), 1, fp);
        fread(&userDataSize, sizeof(int), 1, fp);
        if (userDataSize) fread(userData, sizeof(char), userDataSize, fp);

        return true;

        GN_UNGUARD;
    }
};

//----------------------------------------------------------------------------------
// struct lvlSpace
// 灯光
//----------------------------------------------------------------------------------
struct lvlSpace {
    int type; // fog, water, user......

    uint num_plane; //

    Plane3f * planeList;

    char scriptName[64]; //脚本文件名

    char reserve[LVL_FILE_DEFAULT_RESERVE];

    bool Write(FILE * fp) {
        GN_GUARD;

        fwrite(&type, sizeof(int), 1, fp);
        fwrite(&num_plane, sizeof(int), 1, fp);
        fwrite(planeList, sizeof(Plane3f), num_plane, fp);
        fwrite(scriptName, sizeof(char), 64, fp);
        fwrite(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    bool Read(FILE * fp) {
        GN_GUARD;

        fread(&type, sizeof(int), 1, fp);
        fread(&num_plane, sizeof(int), 1, fp);
        planeList = new Plane3f[num_plane];
        fread(planeList, sizeof(Plane3f), num_plane, fp);
        fread(scriptName, sizeof(char), 64, fp);
        fread(reserve, sizeof(char), LVL_FILE_DEFAULT_RESERVE, fp);
        return true;

        GN_UNGUARD;
    }

    lvlSpace(): planeList(0) {}
    ~lvlSpace() { LVL_FILE_SAFE_DELETE_ARRAY(planeList); }
};

//
// struct lvlSector
// ================
//
struct lvlSector {
    lvlStructArray<lvlCollideFace> collideFaceList;
    lvlIntArray                    drawFaceIDList;
    lvlStructArray<lvlBspNode>     bspNodeList;
    lvlIntArray                    nsFaceIDList; // nonstruct face id list
    lvlIntArray                    curveIDList;
    lvlStructArray<lvlModel>       modelList;
    lvlIntArray                    pointLightIDList;
    lvlIntArray                    spotLightIDList;
    lvlIntArray                    entityIDList;
    lvlIntArray                    spaceIDList;
    lvlIntArray                    frontPortalIDList;
    lvlIntArray                    backPortalIDList;

    bool Write(FILE * fp);
    bool Read(FILE * fp);
};

//
// lvlFileStruct
// =============
//
// main structure of level file
//
struct lvlFileStruct {
    lvlHdr                           header;
    lvlStructArray<lvlFace>          faces;
    lvlStructArray<lvlDrawFace>      drawfaces;
    lvlStructArray<lvlNonStructFace> nsfaces;
    lvlStructArray<lvlCurve>         curves;
    lvlStructArray<lvlName>          modnames;
    lvlStructArray<lvlPointLight>    plights;
    lvlStructArray<lvlSpotLight>     slights;
    lvlStructArray<lvlEntity>        entites;
    lvlStructArray<lvlSpace>         spaces;
    lvlStructArray<lvlPortal>        portals;
    lvlStructArray<lvlSector>        sectors;
    uint                             lmap_num;
    uint                             lmap_width, lmap_height;
    std::vector<uint8_t>             lightmaps;

    bool Write(FILE * fp);
    bool Read(FILE * fp);
};

#endif //_LVLFILESTRUCT_H_
