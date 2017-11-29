#include "pch.h"
#include "ShaderManager.h"
#include "RenderDevice.h"


ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::Init()
{
    if (!PreloadShaders())
    {
        LogPrintf("Shader preload failed");
        return false;
    }
    return true;
}


void ShaderManager::Shutdown()
{
    for (auto item : m_shaders)
        RenderDevice::GetInstance()->DeleteShader(item.shader);
    m_shaders.clear();
}


Shader* ShaderManager::GetShader(const char* vs, const char* ps, const char* gs)
{
    // search for existing shader
    for (auto item : m_shaders)
    {
        if ((!vs || (item.vs == vs)) &&
            (!ps || (item.ps == ps)) &&
            (!gs || (item.gs == gs)))
            return item.shader;
    }

    // warn if not loaded in preload
    if (!m_preload)
    {
        LogPrintf("Warning: shader not found in preload: '%s', '%s', '%s'",
            vs ? vs : "",
            ps ? ps : "",
            gs ? gs : "");
    }

    // load source files
    auto vsCode = vs ? FS_LoadText(vs) : "";
    auto psCode = ps ? FS_LoadText(ps) : "";
    auto gsCode = gs ? FS_LoadText(gs) : "";

    // create shader from sources
    Shader* shader = RenderDevice::GetInstance()->CreateShader(vsCode.c_str(), psCode.c_str());
    if (shader)
    {
        // add new node to the list
        m_shaders.push_back(ShaderNode());
        auto& node = m_shaders.back();

        node.vs = vs ? vs : "";
        node.ps = ps ? ps : "";
        node.gs = gs ? gs : "";
        node.shader = shader;
    }
    else
    {
        LogPrintf("Unable to create shader from '%s', '%s', '%s'",
            vs ? vs : "null",
            ps ? ps : "null",
            gs ? gs : "null");
    }

    return shader;
}


bool ShaderManager::PreloadShaders()
{
    m_preload = true;

    // get preload list from the file
    char *preloadList = FS_LoadFile("ShaderPreload.txt", nullptr);
    if (preloadList)
    {
        LogPrintf("Preload shaders...");

        int numErrors = 0;

        // name pair using space separator
        std::string item;
        const char* context = preloadList;
        while(context != nullptr)
        {
            context = GetLine(context, item);
            auto ptr = item.find_first_of(' ');
            if (ptr != std::string::npos)
            {
                auto vs = item.substr(0, ptr);
                auto ps = item.substr(ptr + 1);
                auto shader = GetShader(vs.c_str(), ps.c_str());
                if (shader)
                {
                    LogPrintf("Success on '%s', '%s'", vs.c_str(), ps.c_str());
                }
                else
                {
                    LogPrintf("Preload filed on shader '%s', '%s'", vs.c_str(), ps.c_str());
                    numErrors++;
                }
            }
            else
            {
                LogPrintf("Warning: invalid shader preload item: '%s'", item.c_str());
            }
        }
        delete[] preloadList;

        if (numErrors)
        {
            LogPrintf("Total %d shaders failed on preload", numErrors);
            return false;
        }
    }
    else
    {
        LogPrintf("Warning: unable to find shader preload list");
    }

    m_preload = false;

    return true;
}
