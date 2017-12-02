#pragma once


#define AGE_SCENE_EXPORTER_CLASS_ID   Class_ID(0x5c0f7a9a, 0x5e773054)


extern HINSTANCE cl_hInstance;


class AgeSceneExporter : public SceneExport
{
public:
    const unsigned int VERSION = 100;

    AgeSceneExporter()
    {
    }

    ~AgeSceneExporter()
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

private:
    ExpInterface* m_ei = nullptr;
    Interface* m_i = nullptr;
    DWORD m_options = 0;
};


class AgeSceneExporterClassDesc : public ClassDesc2
{
public:
    static AgeSceneExporterClassDesc* GetInstance()
    {
        static AgeSceneExporterClassDesc instance;
        return &instance;
    }

    int IsPublic() override
    {
        return TRUE;
    }

    void* Create(BOOL loading = FALSE) override
    {
        return new AgeSceneExporter();
    }

    const MCHAR* ClassName() override
    {
        return L"AgeSceneExporter";
    }

    SClass_ID SuperClassID() override
    {
        return SCENE_EXPORT_CLASS_ID;
    }

    Class_ID ClassID() override
    {
        return AGE_SCENE_EXPORTER_CLASS_ID;
    }

    const MCHAR* Category() override
    {
        return L"SCENE EXPORT";
    }

    const MCHAR* InternalName() override
    {
        return L"AGE Scene exporter";
    }

    HINSTANCE HInstance() override
    {
        return cl_hInstance;
    }
};

