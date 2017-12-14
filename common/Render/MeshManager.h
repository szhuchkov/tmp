#pragma once


#include <Render/RenderWorld.h>



struct MeshInstance
{
    VertexBuffer*               verts;
    IndexBuffer*                inds;
    uint32_t                    primitive;
    std::vector<RenderSurface>  surfaces;
    std::vector<uint32_t>       materialIndexes;
    int                         instanceID;
    AABB                        bbox;
    std::vector<Vector3>        posVerts;
    std::vector<uint32_t>       posInds;
};


class MeshManager
{
public:
    inline static MeshManager* GetInstance()
    {
        static MeshManager instance;
        return &instance;
    }

    MeshInstance* LoadMesh(const char* name);
    void FreeMesh(MeshInstance* mesh);

private:
    MeshManager();
    ~MeshManager();

private:
    struct MeshNode
    {
        std::string     name;
        int             instances;
        MeshInstance    mesh;
    };

    int m_nextInstanceID = 1;

    std::vector<MeshNode*> m_meshNodes;
};
