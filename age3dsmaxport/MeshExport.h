#pragma once


#define AGE_MESH_EXPORTER_CLASS_ID   Class_ID(0x41cc6ee9, 0xc531444)


class IGameMaterial;
class IGameMesh;


extern HINSTANCE cl_hInstance;


class AgeMeshExporter : public SceneExport
{
public:
    const unsigned int VERSION = 100;

    AgeMeshExporter()
    {
    }

    ~AgeMeshExporter()
    {
    }

    int ExtCount() override
    {
        return 1;
    }

    const MCHAR* Ext(int index) override
    {
        return L"mesh";
    }

    const MCHAR* LongDesc() override
    {
        return L"AGE mesh 3DS Max exporter";
    }

    const MCHAR* ShortDesc() override
    {
        return L"AGE Mesh";
    }

    const MCHAR* AuthorName() override
    {
        return L"n3d";
    }

    const MCHAR* CopyrightMessage() override
    {
        return L"Copyright (c) n3d, 1894";
    }

    const MCHAR* OtherMessage1() override
    {
        return L"1";
    }

    const MCHAR* OtherMessage2() override
    {
        return L"2";
    }

    unsigned int Version() override
    {
        return VERSION;
    }

    void ShowAbout(HWND hWnd) override
    {
        MessageBox(hWnd, L"AGE Mesh 3DS Max exporter by n3d", L"AGE", MB_OK);
    }

    BOOL SupportsOptions(int ext, DWORD options) override
    {
        return TRUE;
    }

    int	DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0) override
    {
        MessageBox(NULL, L"Everything is OK so far", L"AGE", MB_OK);
        m_ei = ei;
        m_i = i;
        m_options = options;

        if (!InternalDoExport(name))
            return FALSE;

        return TRUE;
    }

private:
    bool InternalDoExport(const MCHAR* name);
    bool BuildMesh();
    bool ExportMesh(const MCHAR* name);
    bool ExportMaterials(const MCHAR* name);

    uint32_t GetMaterialIndex(IGameMaterial* mtr);

    uint32_t ColorToRGBA(const Color& color, int alpha = 0xff);

    template <typename _T>
    bool Write(HANDLE hf, const _T& value);

    template <typename _T>
    bool Write(HANDLE hf, const std::vector<_T>& array);

private:
    ExpInterface* m_ei = nullptr;
    Interface* m_i = nullptr;
    DWORD m_options = 0;

private:
    struct MeshFace
    {
        int pos[3];
        int norm[3];
        int clr[3];
        int tex[3];
        IGameMaterial* mtr;
    };

    struct MeshNode
    {
        std::wstring            name;
        std::vector<Point3>     verts;
        std::vector<Point3>     normals;
        std::vector<Point3>     tangents;
        std::vector<Point3>     binormals;
        std::vector<Color>      colors;
        std::vector<Point3>     mapVerts;
        std::vector<MeshFace>   faces;
    };

    struct MaterialRange
    {
        size_t start;
        size_t end;
        IGameMaterial* mtr;
    };

    struct IndexedMesh
    {
        std::wstring            name;
        std::vector<Point3>     verts;
        std::vector<Point3>     normals;
        std::vector<Point3>     tangents;
        std::vector<Point3>     binormals;
        std::vector<Color>      colors;
        std::vector<Point3>     mapVerts;
        std::vector<uint32_t>   indexes;
        IGameMaterial*          material;
    };

    std::vector<MeshNode*> m_meshList;
    std::vector<IndexedMesh*> m_indexedMeshes;
    std::set<IGameMaterial*> m_materials;
};


class AgeMeshExporterClassDesc : public ClassDesc2
{
public:
    static AgeMeshExporterClassDesc* GetInstance()
    {
        static AgeMeshExporterClassDesc instance;
        return &instance;
    }

    int IsPublic() override
    {
        return TRUE;
    }

    void* Create(BOOL loading = FALSE) override
    {
        return new AgeMeshExporter();
    }

    const MCHAR* ClassName() override
    {
        return L"AgeMeshExporter";
    }

    SClass_ID SuperClassID() override
    {
        return SCENE_EXPORT_CLASS_ID;
    }

    Class_ID ClassID() override
    {
        return AGE_MESH_EXPORTER_CLASS_ID;
    }

    const MCHAR* Category() override
    {
        return L"SCENE EXPORT";
    }

    const MCHAR* InternalName() override
    {
        return L"AGE Mesh exporter";
    }

    HINSTANCE HInstance() override
    {
        return cl_hInstance;
    }
};

