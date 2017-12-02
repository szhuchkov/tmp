#include "pch.h"
#include "MeshExport.h"
#include "SceneExport.h"


ClassDesc2* g_ClassDesc[] =
{
    AgeMeshExporterClassDesc::GetInstance(),
    AgeSceneExporterClassDesc::GetInstance()
};


HINSTANCE cl_hInstance = NULL;


BOOL WINAPI DllMain(HINSTANCE hInstance, ULONG dwReason, LPVOID lpvReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        cl_hInstance = hInstance;
        DisableThreadLibraryCalls(hInstance);
        if (FAILED(CoInitialize(NULL)))
        {
            MessageBox(NULL, L"CoInitialize() failed", L"AGE Exporter", MB_OK);
            return FALSE;
        }
        break;
    }
    return TRUE;
}


__declspec(dllexport) const TCHAR* LibDescription()
{
    return L"Age 3DS Max exporter";
}


__declspec(dllexport) int LibNumberClasses()
{
    return 1;// sizeof(g_ClassDesc) / sizeof(g_ClassDesc[0]);
}


__declspec(dllexport) ClassDesc* LibClassDesc(int classIndex)
{
    return g_ClassDesc[classIndex];
}


__declspec(dllexport) ULONG LibVersion()
{
    return VERSION_3DSMAX;
}


__declspec(dllexport) ULONG CanAutoDefer()
{
    return TRUE;
}