#include <Windows.h>


#define AGE_EXPORT_CLASS_ID Class_ID(0x280d4a49, 0x4abe3694)


static HINSTANCE g_hInstance = NULL;


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	g_hInstance = hModule;
	//if (!ControlsInit)
	//{
	//	ControlsInit = true;
	//	InitCustomControls(g_hInstance);
	//	InitCommonControls();
	//}
	return TRUE;
}


__declspec(dllexport) int LibNumClasses()
{
	return 1;
}


__declspec(dllexport) const TCHAR* LibDescription()
{
	return L"AGE models exporter";
}


__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}


class AgePluginClassDesc : public ClassDesc
{
public:
	int IsPublic() override
	{
		return 1;
	}

	void* Create(BOOL loading = FALSE) override
	{
		return new AgeExportPlugin();
	}

	const TCHAR* ClassName() override
	{
		return L"AGE EXPORT";
	}

	SClass_ID SuperClassID() override
	{
		return SCENE_EXPORT_CLASS_ID;
	}

	Class_ID ClassID() override
	{
		return AGE_EXPORT_CLASS_ID;
	}

	const TCHAR* Category() override
	{
		return L"";
	}
};