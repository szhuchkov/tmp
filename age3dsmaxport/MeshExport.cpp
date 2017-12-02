#include "pch.h"
#include <IGame/IGame.h>
#include <IGame/IGameMaterial.h>
#include "MeshExport.h"
#include "../common/Render/Loaders/MeshData.h"


//using namespace tinyxml2;


struct MeshVertexIndexed
{
    int v;
    int n;
    int t;
    int tb;
    int c;

    bool operator == (const MeshVertexIndexed& other) const
    {
        return (
            v == other.v &&
            n == other.n &&
            t == other.t &&
            tb == other.tb &&
            c == other.c);
    }

    bool operator < (const MeshVertexIndexed& other) const
    {
        if (v != other.v) return v < other.v ? true : false;
        if (n != other.n) return n < other.n ? true : false;
        if (t != other.t) return t < other.t ? true : false;
        if (tb != other.tb) return tb < other.tb ? true : false;
        if (c != other.c) return c < other.c ? true : false;
        return false;
    }
};


namespace std
{
    template <>
    struct hash<MeshVertexIndexed>
    {
        size_t operator () (const MeshVertexIndexed& v) const
        {
            auto p = reinterpret_cast<const uint32_t*>(&v);
            size_t sz = sizeof(v) / sizeof(uint32_t);
            size_t h = 0x8349578234;
            for (size_t i = 0; i < sz; i++)
                h = (h << 1) | h;
            h &= 0xff;
            return h;
        }
    };
}


bool AgeMeshExporter::InternalDoExport(const MCHAR* name)
{
    auto igame = GetIGameInterface();

    auto conversionManager = GetConversionManager();
    conversionManager->SetCoordSystem(IGameConversionManager::IGAME_D3D);

    auto exportSelected = m_options & SCENE_EXPORT_SELECTED ? true : false;
    igame->InitialiseIGame(exportSelected);
    igame->SetStaticFrame(0);
    
    auto numNodes = igame->GetTopLevelNodeCount();
    for (int nodeIndex = 0; nodeIndex < numNodes; nodeIndex++)
    {
        auto igameNode = igame->GetTopLevelNode(nodeIndex);
        if (igameNode->IsNodeHidden() || igameNode->IsTarget())
            continue;

        auto igameObj = igameNode->GetIGameObject();
        if (!igameObj->IsEntitySupported())
            continue;

        if (igameObj->GetIGameType() != IGameObject::IGAME_MESH)
            continue;

        auto igameMesh = reinterpret_cast<IGameMesh*>(igameObj);
        igameMesh->SetCreateOptimizedNormalList();
        if (!igameMesh->InitializeData())
            continue;

        if (!igameMesh->InitializeBinormalData())
            continue;

        int mapChannel = 1;
        auto activeChannels = igameMesh->GetActiveMapChannelNum();
        if (activeChannels.Count() > 0)
            mapChannel = activeChannels[0];

        int numVerts = igameMesh->GetNumberOfVerts();
        int numNormals = igameMesh->GetNumberOfNormals();
        int numTangents = igameMesh->GetNumberOfTangents(mapChannel);
        int numBinormals = igameMesh->GetNumberOfBinormals(mapChannel);
        int numMapVerts = igameMesh->GetNumberOfMapVerts(mapChannel);
        int numColors = igameMesh->GetNumberOfColorVerts();
        int numFaces = igameMesh->GetNumberOfFaces();

        if (!numVerts || !numFaces)
            continue;

        auto node = new MeshNode();
        m_meshList.push_back(node);

        node->verts.resize(numVerts);
        for (int i = 0; i < numVerts; i++)
            node->verts[i] = igameMesh->GetVertex(i);

        node->normals.resize(numNormals);
        for (int i = 0; i < numNormals; i++)
            node->normals[i] = igameMesh->GetNormal(i);

        node->tangents.resize(numTangents);
        for (int i = 0; i < numTangents; i++)
            node->tangents[i] = igameMesh->GetTangent(i, mapChannel);

        node->binormals.resize(numBinormals);
        for (int i = 0; i < numBinormals; i++)
            node->binormals[i] = igameMesh->GetBinormal(i, mapChannel);

        node->mapVerts.resize(numMapVerts);
        for (int i = 0; i < numMapVerts; i++)
            node->mapVerts[i] = igameMesh->GetMapVertex(mapChannel, i);

        node->faces.resize(numFaces);
        for (int i = 0; i < numFaces; i++)
        {
            auto src = igameMesh->GetFace(i);
            auto& dst = node->faces[i];

            DWORD mapIdx[3] = { 0, 0, 0 };
            igameMesh->GetMapFaceIndex(mapChannel, i, mapIdx);

            for (int j = 0; j < 3; j++)
            {
                dst.pos[j] = src->vert[j];
                dst.norm[j] = src->norm[j];
                dst.clr[j] = src->color[j];
                dst.tex[j] = mapIdx[j];
                dst.mtr = igameMesh->GetMaterialFromFace(src);
                m_materials.insert(dst.mtr);
            }
        }
    }

    bool res = true;

    res = BuildMesh();
    if (res)
    {
        if (!ExportMesh(name))
            res = false;

        if (!ExportMaterials(name))
            res = false;
    }

    // cleanup data
    for (auto item : m_meshList)
        delete item;
    m_meshList.clear();
    m_materials.clear();

    igame->ReleaseIGame();

    return res;
}


bool AgeMeshExporter::BuildMesh()
{
    for (const auto& node : m_meshList)
    {
        // sort by material
        std::sort(node->faces.begin(), node->faces.end(), [](const MeshFace& a, const MeshFace& b){
            return a.mtr < b.mtr;
        });

        // generate material ranges
        std::vector<MaterialRange> materialRanges;
        IGameMaterial* lastMaterial = nullptr;
        size_t rangeStart = 0;
        for (size_t i = 0; i < node->faces.size(); i++)
        {
            const auto& f = node->faces[i];
            if (!lastMaterial || f.mtr != lastMaterial)
            {
                if (lastMaterial)
                {
                    materialRanges.resize(materialRanges.size() + 1);
                    materialRanges.back().start = rangeStart;
                    materialRanges.back().end = i;
                    materialRanges.back().mtr = lastMaterial;
                }
                lastMaterial = f.mtr;
            }
        }

        // last range
        materialRanges.resize(materialRanges.size() + 1);
        materialRanges.back().start = rangeStart;
        materialRanges.back().end = node->faces.size();
        materialRanges.back().mtr = lastMaterial;

        // generate indexed mesh for each material range
        for (auto range : materialRanges)
        {
            size_t numFaces = range.end - range.start;

            std::unordered_map<MeshVertexIndexed, uint32_t> vertexMap;
            std::vector<MeshVertexIndexed> vertexBuffer;
            std::vector<uint32_t> indexBuffer;
            for (size_t i = range.start; i < range.end; i++)
            {
                const auto& f = node->faces[i];
                for (int j = 0; j < 3; j++)
                {
                    MeshVertexIndexed v = { f.pos[j], f.norm[j], f.tex[j] };
                    auto it = vertexMap.find(v);
                    if (it != vertexMap.end())
                    {
                        indexBuffer.push_back(it->second);
                    }
                    else
                    {
                        auto index = static_cast<uint32_t>(vertexBuffer.size());
                        indexBuffer.push_back(index);
                        vertexBuffer.push_back(v);
                        vertexMap[v] = index;
                    }
                }
            }

            IndexedMesh* mesh = new IndexedMesh();
            mesh->material = range.mtr;

            size_t numVerts = vertexBuffer.size();
            mesh->verts.resize(numVerts);
            mesh->normals.resize(numVerts);
            mesh->tangents.resize(numVerts);
            mesh->binormals.resize(numVerts);
            mesh->mapVerts.resize(numVerts);
            mesh->colors.resize(numVerts);
            for (size_t i = 0; i < numVerts; i++)
            {
                auto& f = vertexBuffer[i];
                mesh->verts[i] = node->verts[f.v];
                mesh->normals[i] = node->normals.empty() ? Point3(0, 0, 1) : node->normals[f.n];
                mesh->tangents[i] = node->tangents.empty() ? Point3(0, 1, 0) : node->tangents[f.tb];
                mesh->binormals[i] = node->binormals.empty() ? Point3(1, 0, 0) : node->binormals[f.tb];
                mesh->mapVerts[i] = node->mapVerts.empty() ? Point3(0, 0, 0) : node->mapVerts[f.t];
                mesh->colors[i] = node->colors.empty() ? Color(1, 1, 1) : mesh->colors[f.c];
                mesh->colors[i] = Color(1, 1, 1);
            }

            size_t numInds = indexBuffer.size();
            mesh->indexes.resize(numInds);
            for (size_t i = 0; i < numInds; i++)
            {
                mesh->indexes[i] = indexBuffer[i];
            }

            m_indexedMeshes.push_back(mesh);
        }
    }
    return true;
}


bool AgeMeshExporter::ExportMesh(const MCHAR* name)
{
    MCHAR dstName[1024];
    wcscpy(dstName, name);

    MCHAR* ext = wcsrchr(dstName, '.');
    wcscpy(ext, L".mesh");

    HANDLE hf = CreateFile(dstName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
    if (hf == INVALID_HANDLE_VALUE)
        return false;

    uint32_t numVerts = 0;
    uint32_t numInds = 0;
    uint32_t numSurfaces = 0;
    uint32_t numBones = 0;

    std::vector<Point3> pos;
    std::vector<Point3> nrm;
    std::vector<Point3> tng;
    std::vector<Point2> tex;
    std::vector<uint32_t> clr;
    std::vector<uint32_t> idx;

    std::vector<MeshData::Surface> surfaces(m_indexedMeshes.size());
    for (size_t i = 0; i < m_indexedMeshes.size(); i++)
    {
        auto& s = surfaces[i];
        auto m = m_indexedMeshes[i];

        s.vertexOffset = numVerts;
        s.indexOffset = numInds;
        s.vertexCount = static_cast<uint32_t>(m->verts.size());
        s.indexCount = static_cast<uint32_t>(m->indexes.size());
        s.materialIndex = GetMaterialIndex(m->material);

        for (uint32_t j = 0; j < s.vertexCount; j++)
        {
            pos.push_back(m->verts[j]);
            nrm.push_back(m->normals[j]);
            tex.push_back(Point2(m->mapVerts[j].x, m->mapVerts[j].y));
            tng.push_back(m->tangents[j]);
            clr.push_back(ColorToRGBA(m->colors[j]));
        }

        for (uint32_t j = 0; j < s.indexCount; j++)
        {
            idx.push_back(s.vertexOffset + m->indexes[j]);
        }

        numVerts += s.vertexCount;
        numInds += s.indexCount;
        numSurfaces++;
    }

    MeshData::Header header;
    header.version = MeshData::VERSION;
    header.format = MeshData::VERTEX_POSITION | MeshData::VERTEX_NORMAL | MeshData::VERTEX_TANGENT | MeshData::VERTEX_TEXCOORD | MeshData::VERTEX_COLOR | MeshData::INDEX_32;
    header.numVerts = numVerts;
    header.numInds = numInds;
    header.numSurfaces = numSurfaces;
    header.numBones = numBones;

    bool res = true;

    res &= Write(hf, header);

    for (uint32_t i = 0; i < pos.size(); i++)
    {
        res &= Write(hf, pos[i]);
        res &= Write(hf, nrm[i]);
        res &= Write(hf, tex[i]);
        res &= Write(hf, tng[i]);
        res &= Write(hf, clr[i]);
    }

    res &= Write(hf, idx);
    res &= Write(hf, surfaces);

    CloseHandle(hf);

    if (!res)
        return false;

    return true;
}


std::string wchar_to_UTF8(const wchar_t * in)
{
    std::string out;
    unsigned int codepoint = 0;
    for (in; *in != 0; ++in)
    {
        if (*in >= 0xd800 && *in <= 0xdbff)
            codepoint = ((*in - 0xd800) << 10) + 0x10000;
        else
        {
            if (*in >= 0xdc00 && *in <= 0xdfff)
                codepoint |= *in - 0xdc00;
            else
                codepoint = *in;

            if (codepoint <= 0x7f)
                out.append(1, static_cast<char>(codepoint));
            else if (codepoint <= 0x7ff)
            {
                out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else if (codepoint <= 0xffff)
            {
                out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else
            {
                out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            codepoint = 0;
        }
    }
    return out;
}


std::string ToTextureName(const MCHAR* name)
{
    // convert to UTF-8
    std::string fullName = wchar_to_UTF8(name);

    // find last backslash
    auto end = fullName.find_last_of('\\');
    if (end != std::string::npos)
        fullName = fullName.substr(end + 1);

    // find again last normal slash
    end = fullName.find_last_of('/');
    if (end != std::string::npos)
        fullName = fullName.substr(end + 1);

    return fullName;
}


bool AgeMeshExporter::ExportMaterials(const MCHAR* name)
{
    tinyxml2::XMLDocument doc;
    auto libElement = doc.NewElement("MaterialLibrary");
    doc.LinkEndChild(libElement);

    // for each material
    for (auto mtr : m_materials)
    {
        // append new material element
        auto mtrElement = doc.NewElement("Material");
        libElement->LinkEndChild(mtrElement);

        if (!mtr)
            continue;

        auto maxMtr = mtr->GetMaxMaterial();

        std::string diffuseName = "";
        std::string normalsName = "";

        // get diffuse texmap
        auto diffuseMap = maxMtr->GetSubTexmap(ID_DI);
        if (diffuseMap && diffuseMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
        {
            // to bitmap
            auto bmDiffuse = reinterpret_cast<BitmapTex*>(diffuseMap);
            diffuseName = ToTextureName(bmDiffuse->GetMapName());
        }

        // get normals
        auto normalMap = maxMtr->GetSubTexmap(ID_BU);
        if (normalMap)
        {
            // first child texture should be normals bitmap
            auto texNormals = normalMap->GetSubTexmap(0);
            if (texNormals && texNormals->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
            {
                // to bitmap
                auto bmNormals = reinterpret_cast<BitmapTex*>(texNormals);
                normalsName = ToTextureName(bmNormals->GetMapName());
            }
        }

        // append diffuse
        auto diffuseElement = doc.NewElement("Texture0");
        diffuseElement->SetText(diffuseName.c_str());
        mtrElement->LinkEndChild(diffuseElement);

        // append normals
        if (!normalsName.empty())
        {
            auto normalsElement = doc.NewElement("Texture1");
            normalsElement->SetText(normalsName.c_str());
            mtrElement->LinkEndChild(normalsElement);
        }
    }

    // generate materials file name
    MCHAR dstName[1024];
    wcscpy(dstName, name);
    MCHAR* ext = wcsrchr(dstName, '.');
    wcscpy(ext, L".mtr");

    // write to file
    HANDLE hf = CreateFile(dstName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
    if (hf == INVALID_HANDLE_VALUE)
        return false;

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);

    auto strDoc = printer.CStr();
    auto szDoc = printer.CStrSize() + 1;

    DWORD bytes;
    WriteFile(hf, strDoc, szDoc, &bytes, NULL);
    bool res = bytes == szDoc;

    CloseHandle(hf);

    if (!res)
        return false;

    return true;
}


uint32_t AgeMeshExporter::GetMaterialIndex(IGameMaterial* mtr)
{
    auto idx = std::distance(m_materials.begin(), m_materials.find(mtr));
    return static_cast<uint32_t>(idx);
}


template <typename _T>
bool AgeMeshExporter::Write(HANDLE hf, const _T& value)
{
    DWORD sz = sizeof(_T);
    DWORD bytes = 0;
    if (!WriteFile(hf, &value, sz, &bytes, NULL))
        return false;
    if (bytes != sz)
        return false;
    return true;
}


template <typename _T>
bool AgeMeshExporter::Write(HANDLE hf, const std::vector<_T>& array)
{
    DWORD sz = sizeof(_T) * static_cast<uint32_t>(array.size());
    DWORD bytes = 0;
    if (!array.empty())
    {
        if (!WriteFile(hf, &array[0], sz, &bytes, NULL))
            return false;
        if (bytes != sz)
            return false;
    }
    return true;
}


uint32_t AgeMeshExporter::ColorToRGBA(const Color& color, int alpha)
{
    uint32_t res = (alpha & 0xff) << 24;
    res |= (static_cast<uint32_t>(255 * color.r) & 0xff) << 16;
    res |= (static_cast<uint32_t>(255 * color.g) & 0xff) << 8;
    res |= (static_cast<uint32_t>(255 * color.b) & 0xff) << 0;
    return res;
}