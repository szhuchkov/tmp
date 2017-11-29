#pragma once


#include "UnitTest.h"
#include "Render/Loaders/MeshData.h"


class MeshDataTest : public UnitTest
{
public:
    // name of the test
    const char* GetName() override
    {
        return "MeshData test";
    }

    // run test
    bool Run() override
    {
        MeshData mesh;
        if (!mesh.Load("test.mesh"))
            return false;

        // common info
        if (mesh.GetNumVerts() != 100)
            return false;
        if (mesh.GetNumInds() != 100)
            return false;
        if (mesh.GetVertexSize() != 20)
            return false;
        if (mesh.GetNumSurfaces() != 2)
            return false;
        if (mesh.GetNumBones() != 2)
            return false;

        // validate format
        if (mesh.GetFormat() != (MeshData::VERTEX_POSITION | MeshData::VERTEX_NORMAL | MeshData::VERTEX_TEXCOORD))
            return false;

        // validate bones data
        auto bones = mesh.GetBones();
        if (strcmp(bones[0].name, "bone_00"))
            return false;

        // validate surface data
        auto surfaces = mesh.GetSurfaces();
        if (surfaces[1].indexOffset != 50)
            return false;
        if (surfaces[1].materialIndex != 1)
            return false;

        return true;
    }
};
