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
        if (mesh.GetNumVerts() != 24)
            return false;
        if (mesh.GetNumInds() != 36)
            return false;
        if (mesh.GetVertexSize() != 48)
            return false;
        if (mesh.GetNumSurfaces() != 1)
            return false;
        if (mesh.GetNumBones() != 0)
            return false;

        // validate format
        if (mesh.GetFormat() != (
            MeshData::VERTEX_POSITION |
            MeshData::VERTEX_NORMAL |
            MeshData::VERTEX_TEXCOORD |
            MeshData::VERTEX_TANGENT |
            MeshData::VERTEX_COLOR |
            MeshData::INDEX_32))
            return false;

        // validate bones data
        //auto bones = mesh.GetBones();
        //if (strcmp(bones[0].name, "bone_00"))
        //    return false;

        // validate surface data
        auto surfaces = mesh.GetSurfaces();
        if (surfaces[0].indexOffset != 0 ||
            surfaces[0].indexCount != 36 ||
            surfaces[0].vertexCount != 24 ||
            surfaces[0].vertexOffset != 0 ||
            surfaces[0].materialIndex != 0)
            return false;

        return true;
    }
};
