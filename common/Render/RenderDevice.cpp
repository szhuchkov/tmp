#include "pch.h"
#include "RenderDevice.h"
#include "OGLDriver.h"


#if _DEBUG
#define GL_CHECK_ERROR(x)       { x; CheckGlErrors(__FILE__, __LINE__, #x); }
#else
#define GL_CHECK_ERROR(x)       x
#endif


void CheckGlErrors(const char* file, int line, const char* expr)
{
    static int numErrors = 0;
    const int MAX_ERRORS = 100;

    GLenum err;
    if (numErrors < MAX_ERRORS && GL_NO_ERROR != (err = glGetError()))
    {
        do {
            numErrors++;
            LogRaw("%s(%d): glGetError() = 0x%08x for expr '%s'", file, line, err, expr);
            err = glGetError();
        } while (err != GL_NO_ERROR && numErrors < MAX_ERRORS);

        if (numErrors >= MAX_ERRORS)
        {
            LogPrintf("MAX_ERRORS, stop logging");
        }
    }
}


struct VertexAttribDesc
{
	const char*		name;
	unsigned int	type;
	unsigned int	size;
	int				normalized;
	unsigned int	stride;
};


static VertexAttribDesc g_VertexAttribDesc[] =
{
    { "aPos",       GL_FLOAT,           3,  GL_FALSE,   12 },
    { "aNormal",    GL_FLOAT,           3,  GL_FALSE,   12 },
    { "aTexCoord",  GL_FLOAT,           2,  GL_FALSE,   8 },
    { "aTangent",   GL_FLOAT,           3,  GL_FALSE,   12 },
    { "aTBN",       GL_UNSIGNED_BYTE,   4,  GL_TRUE,    8 },
    { "aBoneWeight",GL_FLOAT,           3,  GL_FALSE,   12 },
    { "aBoneIndex", GL_UNSIGNED_BYTE,   4,  GL_FALSE,   4 },
    { "aColor",     GL_UNSIGNED_BYTE,	4,  GL_TRUE,    4 },
};


static const char* g_UniformName[RenderDevice::MAX_UNIFORMS] =
{
    "gUniform0",
    "gUniform1",
    "gUniform2",
    "gUniform3",
    "gUniform4",
    "gUniform5",
    "gUniform6",
    "gUniform7",
    "gUniform8",
    "gUniform9",
    "gUniform10",
    "gUniform11",
    "gUniform12",
    "gUniform13",
    "gUniform14",
    "gUniform15",
    "gUniform16",
    "gUniform17",
    "gUniform18",
    "gUniform19",
};


static const char* g_MatrixName[RenderDevice::MAX_MATRICES] =
{
    "gWorld",
    "gViewProjection",
    "gViewInverse",
    "gShadowMatrix",
    "gLightMatrix",
    "gUserMatrix0",
    "gUserMatrix1",
    "gUserMatrix2",
    "gUserMatrix3",
    "gUserMatrix4",
};


static const char* g_SamplerName[RenderDevice::MAX_SAMPLERS] =
{
    "sTex0",
    "sTex1",
    "sTex2",
    "sTex3",
    "sTex4",
    "sTex5",
    "sTex6",
    "sTex7",
};


struct VertexBuffer
{
	GLuint			handle;
	unsigned int	format;
	unsigned int	stride;
	unsigned int	numVerts;
};


struct IndexBuffer
{
	GLuint			handle;
	unsigned int	format;
	unsigned int	stride;
	unsigned int	numInds;
};


struct Shader
{
	GLuint			program;

	GLint			attribLocation[ARRAY_SIZE(g_VertexAttribDesc)];

    GLint           matrixLocation[RenderDevice::MAX_MATRICES];
    int             matrixVersion[RenderDevice::MAX_MATRICES];

    GLint           uniformLocation[RenderDevice::MAX_UNIFORMS];
    int             uniformVersion[RenderDevice::MAX_UNIFORMS];
};


struct Texture
{
	GLuint			handle;
	unsigned int	format;
	unsigned int	width;
	unsigned int	height;
	unsigned int	depth;
	unsigned int	levels;
	unsigned int	usage;
};


struct RenderTarget
{
	GLuint			framebuffer;
	Texture*		color;
	Texture*		depth;
};


inline const void* GL_BUFFER_POINTER(unsigned int offset)
{
	const char* ptr = NULL;
	return &ptr[offset];
}


static GLenum GLPrimitiveType(unsigned int type)
{
	static const GLenum table[] =
	{
		GL_POINTS,
		GL_LINES,
		GL_LINE_STRIP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP
	};
	return table[type];
}


RenderDevice::RenderDevice()
{
}


RenderDevice::~RenderDevice()
{
}


bool RenderDevice::Init(void* window, int width, int height, bool fullscreen)
{
	if (!OGLDriver::GetInstance()->Init(window, width, height, fullscreen))
		return false;

    m_width = width;
    m_height = height;

    GL_CHECK_ERROR(glEnable(GL_DEPTH));
    GL_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
    GL_CHECK_ERROR(glDepthFunc(GL_LEQUAL));
    GL_CHECK_ERROR(glEnable(GL_CULL_FACE));
    GL_CHECK_ERROR(glFrontFace(GL_CW));

	return true;
}


void RenderDevice::Shutdown()
{
	OGLDriver::GetInstance()->Shutdown();
}


bool RenderDevice::Update()
{
	if (!OGLDriver::GetInstance()->Update())
		return false;

	return true;
}


RenderDevice::ScreenSize RenderDevice::GetScreenSize()
{
    ScreenSize size;
    size.width = m_width;
    size.height = m_height;
    size.aspect = static_cast<float>(m_width) / m_height;
    return size;
}


bool RenderDevice::Resize(int width, int height)
{
    m_width = width;
    m_height = height;
    m_state = DIRTY_TARGET;
	return true;
}


bool RenderDevice::BeginFrame()
{
	return true;
}


void RenderDevice::EndFrame()
{
	OGLDriver::GetInstance()->SwapBuffers();
}


unsigned int RenderDevice::GetVertexStride(unsigned int format)
{
	unsigned int stride = 0;
	for(int i = 0; i < ARRAY_SIZE(g_VertexAttribDesc); i++)
	{
		if (format & (1 << i))
		{
			stride += g_VertexAttribDesc[i].stride;
		}
	}
	return stride;
}


unsigned int RenderDevice::GetIndexSize(unsigned int format)
{
	if (format & INDEX_32)
		return 4;
	if (format & INDEX_16)
		return 2;
	return 0;
}


void RenderDevice::Clear(unsigned int flags, const float* color, float depth, int stencil)
{
	GLbitfield mask = 0;

	// color bit
	if (flags & CLEAR_COLOR)
	{
        GL_CHECK_ERROR(glClearColor(color[0], color[1], color[2], color[3]));
		mask |= GL_COLOR_BUFFER_BIT;
	}

	// depth bit
	if (flags & CLEAR_DEPTH)
	{
        GL_CHECK_ERROR(glClearDepth(depth));
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	// stencil bit
	if (flags & CLEAR_STENCIL)
	{
        GL_CHECK_ERROR(glClearStencil(stencil));
		mask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(mask);
}


void RenderDevice::SetRenderTarget(RenderTarget* target)
{
    if (m_activeTarget != target)
    {
        m_activeTarget = target;
        m_state |= DIRTY_TARGET;
    }
}


void RenderDevice::SetVertexBuffer(VertexBuffer* buffer)
{
    if (m_activeVerts != buffer)
    {
        m_activeVerts = buffer;
        m_state |= DIRTY_VERTS;
    }
}


void RenderDevice::SetIndexBuffer(IndexBuffer* buffer)
{
    if (m_activeInds != buffer)
    {
        m_activeInds = buffer;
        m_state |= DIRTY_INDS;
    }
}


void RenderDevice::SetShader(Shader* shader)
{
    if (m_activeShader != shader)
    {
        m_activeShader = shader;
        m_state |= DIRTY_SHADER;
    }
}


void RenderDevice::SetTexture(unsigned int sampler, Texture* texture)
{
    if (sampler < MAX_SAMPLERS && m_activeSamplers[sampler] != texture)
    {
        m_state |= DIRTY_SAMPLERS;
        m_samplerState |= (1 << sampler);
        m_activeSamplers[sampler] = texture;
    }
}


void RenderDevice::SetUniform(unsigned int index, const Vector4& value)
{
    if (index < MAX_UNIFORMS)
    {
        if (memcmp(&value, &m_uniformValue[index], sizeof(Vector4)))
        {
            m_state |= DIRTY_UNIFORMS;
            m_uniformVersion[index]++;
            m_uniformValue[index] = value;
        }
    }
}


void RenderDevice::SetMatrix(unsigned int index, const Matrix& value)
{
    if (index < MAX_MATRICES)
    {
        if (memcmp(&value, &m_matrixValue[index], sizeof(Matrix)))
        {
            m_state |= DIRTY_MATRICES;
            m_matrixVersion[index]++;
            m_matrixValue[index] = value;
        }
    }
}


void RenderDevice::DrawPrimitive(unsigned int type, unsigned int offset, unsigned int count)
{
    ValidateState();

	GLenum mode = GLPrimitiveType(type);
    GL_CHECK_ERROR(glDrawArrays(mode, offset, count));
}


void RenderDevice::DrawIndexedPrimitive(unsigned int type, unsigned int vertexOffset, unsigned int vertexCount, unsigned int indexOffset, unsigned int count)
{
    ValidateState();

	GLenum mode = GLPrimitiveType(type);

	unsigned int indexSize = m_activeInds->stride;
	GLenum indexType = (indexSize == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT);

#if PLATFORM_ANDROID
    GL_CHECK_ERROR(glDrawElements(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize)));
#else
	//if (vertexOffset > 0)
	//	glDrawElementsBaseVertex(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize), vertexOffset);
	//else
    GL_CHECK_ERROR(glDrawElements(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize)));
#endif
}


void RenderDevice::ValidateState()
{
    // render target
    if (m_state & DIRTY_TARGET)
    {
        if (m_activeTarget)
        {
            GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, m_activeTarget->framebuffer));
            GL_CHECK_ERROR(glViewport(0, 0, m_activeTarget->color->width, m_activeTarget->color->height));
        }
        else
        {
            GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            GL_CHECK_ERROR(glViewport(0, 0, m_width, m_height));
        }
    }

    // vertex buffer
    if (m_state & DIRTY_VERTS)
    {
        GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_activeVerts ? m_activeVerts->handle : 0));
    }

    // index buffer
    if (m_state & DIRTY_INDS)
    {
        GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_activeInds ? m_activeInds->handle : 0));
    }

    // shader
    if (m_state & DIRTY_SHADER)
    {
        GL_CHECK_ERROR(glUseProgram(m_activeShader ? m_activeShader->program : 0));
    }

    // setup attribute stream
    if (m_state & DIRTY_VERTS || m_state & DIRTY_SHADER)
    {
        SetupVertexStream();
    }

    // update uniforms
    if (m_state & DIRTY_SHADER | m_state & DIRTY_UNIFORMS && m_activeShader)
    {
        for (int i = 0; i < MAX_UNIFORMS; i++)
        {
            if (m_activeShader->uniformLocation[i] != -1)
            {
                if (m_activeShader->uniformVersion[i] != m_uniformVersion[i])
                {
                    m_activeShader->uniformVersion[i] = m_uniformVersion[i];
                    GL_CHECK_ERROR(glUniform4fv(m_activeShader->uniformLocation[i], 1, reinterpret_cast<const float*>(&m_uniformValue[i])));
                }
            }
        }
    }

    // update matrices
    if (m_state & DIRTY_SHADER | m_state & DIRTY_MATRICES)
    {
        for (int i = 0; i < MAX_MATRICES; i++)
        {
            if (m_activeShader->matrixLocation[i] != -1)
            {
                if (m_activeShader->matrixVersion[i] != m_matrixVersion[i])
                {
                    m_activeShader->matrixVersion[i] = m_matrixVersion[i];
                    GL_CHECK_ERROR(glUniformMatrix4fv(m_activeShader->matrixLocation[i], 1, GL_FALSE, reinterpret_cast<const float*>(&m_matrixValue[i])));
                }
            }
        }
    }

    // update active samplers
    if (m_samplerState)
    {
        for (int i = 0; i < MAX_SAMPLERS; i++)
        {
            if (m_samplerState & (1 << i))
            {
                GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + i));
                GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, m_activeSamplers[i] ? m_activeSamplers[i]->handle : 0));
            }
        }
    }

    m_state = 0;
    m_samplerState = 0;
}


void RenderDevice::SetupVertexStream()
{
	// bind vertex streams
	unsigned int activeAttribs = 0;
	unsigned int offset = 0;
	for(int i = 0; i < ARRAY_SIZE(g_VertexAttribDesc); i++)
	{
		if (m_activeVerts->format & (1 << i))
		{
			if (m_activeShader->attribLocation[i] != -1)
			{
                GL_CHECK_ERROR(glVertexAttribPointer(
					m_activeShader->attribLocation[i],
					g_VertexAttribDesc[i].size,
					g_VertexAttribDesc[i].type,
					g_VertexAttribDesc[i].normalized,
					m_activeVerts->stride, GL_BUFFER_POINTER(offset)));
				activeAttribs |= (1 << m_activeShader->attribLocation[i]);
			}
			offset += g_VertexAttribDesc[i].stride;
		}
	}

	// enable/disable active streams
	for(int i = 0; i < ARRAY_SIZE(g_VertexAttribDesc); i++)
	{
		unsigned int curr = activeAttribs & (1 << i);
		unsigned int last = m_activeAttribs & (1 << i);

		if (curr && !last)
		{
            GL_CHECK_ERROR(glEnableVertexAttribArray(i));
		}
		else if (last && !curr)
		{
            GL_CHECK_ERROR(glDisableVertexAttribArray(i));
		}
	}

	m_activeAttribs = activeAttribs;
}


VertexBuffer* RenderDevice::CreateVertexBuffer(unsigned int format, unsigned int numVerts, const void* data)
{
	// get stride and size
	unsigned int stride = GetVertexStride(format);
	unsigned int size = stride * numVerts;

	// static or dynamic data
	GLenum usage = format & VERTEX_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	// alloc buffer
	GLuint handle;
    GL_CHECK_ERROR(glGenBuffers(1, &handle));
    GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, handle));
    GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
    GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// create buffer instance
	VertexBuffer* buffer = new VertexBuffer();
	buffer->handle = handle;
	buffer->format = format;
	buffer->stride = stride;
	buffer->numVerts = numVerts;
	return buffer;
}


void RenderDevice::DeleteVertexBuffer(VertexBuffer* buffer)
{
	if (buffer)
	{
        GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CHECK_ERROR(glDeleteBuffers(1, &buffer->handle));
		delete buffer;
	}
}


void RenderDevice::UpdateVertexBuffer(VertexBuffer* buffer, const void* data, unsigned int offset, unsigned int size)
{
	// use zero to fill entire buffer
	if (!size)
		size = buffer->stride * buffer->numVerts;

    GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, buffer->handle));
    GL_CHECK_ERROR(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


IndexBuffer* RenderDevice::CreateIndexBuffer(unsigned int format, unsigned int numInds, const void* data)
{
	// get stride and size
	unsigned int stride = GetIndexSize(format);
	unsigned int size = stride * numInds;

	// static or dynamic data
	GLenum usage = format & INDEX_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	// alloc buffer
	GLuint handle;
    GL_CHECK_ERROR(glGenBuffers(1, &handle));
    GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
    GL_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
    GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// create buffer instance
	IndexBuffer* buffer = new IndexBuffer();
	buffer->handle = handle;
	buffer->format = format;
	buffer->numInds = numInds;
	buffer->stride = stride;
	return buffer;
}


void RenderDevice::DeleteIndexBuffer(IndexBuffer* buffer)
{
	if (buffer)
	{
        GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GL_CHECK_ERROR(glDeleteBuffers(1, &buffer->handle));
		delete buffer;
	}
}


void RenderDevice::UpdateIndexBuffer(IndexBuffer* buffer, const void* data, unsigned int offset, unsigned int size)
{
	// use zero to fill entire buffer
	if (!size)
		size = buffer->stride * buffer->numInds;

    GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->handle));
    GL_CHECK_ERROR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
    GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


unsigned int RenderDevice::CompileShader(const char* code, unsigned int type)
{
	// create shader and attach sources
	GLuint shader = glCreateShader(type);
	GLint length = strlen(code);
    GL_CHECK_ERROR(glShaderSource(shader, 1, &code, &length));
    GL_CHECK_ERROR(glCompileShader(shader));

	// get compilation log
	char buffer[10000] = { 0 };
	GLsizei logSize;
    GL_CHECK_ERROR(glGetShaderInfoLog(shader, sizeof(buffer), &logSize, buffer));
	if (logSize > 0)
	{
		LogPrintf("Shader info log:");
		LogPrintf("%s", buffer);
	}

	// check compile status
	GLint status;
    GL_CHECK_ERROR(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
	if (!status)
	{
        GL_CHECK_ERROR(glDeleteShader(shader));
		return 0;
	}

	return shader;
}


void RenderDevice::InitShader(Shader* shader)
{
    //TODO: use glProgramUniform*
    GL_CHECK_ERROR(glUseProgram(shader->program));

    // vertex streams
    for (int i = 0; i < ARRAY_SIZE(g_VertexAttribDesc); i++)
        shader->attribLocation[i] = glGetAttribLocation(shader->program, g_VertexAttribDesc[i].name);

    // uniforms
    for (int i = 0; i < MAX_UNIFORMS; i++)
    {
        shader->uniformLocation[i] = glGetUniformLocation(shader->program, g_UniformName[i]);
        shader->uniformVersion[i] = -1;
    }

    // matrices
    for (int i = 0; i < MAX_MATRICES; i++)
    {
        shader->matrixLocation[i] = glGetUniformLocation(shader->program, g_MatrixName[i]);
        shader->matrixVersion[i] = -1;
    }

    // samplers
    for (int i = 0; i < MAX_SAMPLERS; i++)
    {
        GLint loc = glGetUniformLocation(shader->program, g_SamplerName[i]);
        if (loc != -1)
        {
            glUniform1i(loc, i);
        }
    }
}


Shader* RenderDevice::CreateShader(const char* vsCode, const char* fsCode)
{
	// compile shaders
	GLuint vs = CompileShader(vsCode, GL_VERTEX_SHADER);
	GLuint fs = CompileShader(fsCode, GL_FRAGMENT_SHADER);
	if (!vs || !fs)
	{
        GL_CHECK_ERROR(glDeleteShader(vs));
        GL_CHECK_ERROR(glDeleteShader(fs));
		LogPrintf("Unable to create shader");
		return NULL;
	}

	// create program and attach shaders
	GLuint program = glCreateProgram();
    GL_CHECK_ERROR(glAttachShader(program, vs));
    GL_CHECK_ERROR(glAttachShader(program, fs));
    GL_CHECK_ERROR(glLinkProgram(program));

	// delete shaders now
    GL_CHECK_ERROR(glDeleteShader(vs));
    GL_CHECK_ERROR(glDeleteShader(fs));

	// get log
	char buffer[10000] = { 0 };
	GLsizei logSize;
    GL_CHECK_ERROR(glGetProgramInfoLog(program, sizeof(buffer), &logSize, buffer));
	if (logSize > 0)
	{
		LogPrintf("Program info log:");
		LogPrintf("%s", buffer);
	}

	// check link status
	GLint status;
    GL_CHECK_ERROR(glGetProgramiv(program, GL_LINK_STATUS, &status));
	if (!status)
	{
        GL_CHECK_ERROR(glDeleteProgram(program));
		return NULL;
	}

	// create shader instance
	Shader* shader = new Shader();
	shader->program = program;

    // initialize
    InitShader(shader);

	return shader;
}


void RenderDevice::DeleteShader(Shader* shader)
{
	if (shader)
	{
        GL_CHECK_ERROR(glDeleteProgram(shader->program));
		delete shader;
	}
}


Texture* RenderDevice::CreateTexture2D(unsigned int width, unsigned int height, unsigned int levels, unsigned int format, unsigned int usage)
{
    GLuint handle = 0;
    GL_CHECK_ERROR(glGenTextures(1, &handle));

    GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, handle));
    GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

    if (usage == TEXTURE_USAGE_DEPTH_BUFFER)
    {
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }
    else
    {
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }

    GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

    Texture* tex = new Texture();
    tex->width = width;
    tex->height = height;
    tex->depth = 0;
    tex->levels = levels;
    tex->format = format;
    tex->usage = usage;
    tex->handle = handle;

    return tex;
}


void RenderDevice::DeleteTexture(Texture* texture)
{
    if (texture)
    {
        GL_CHECK_ERROR(glDeleteTextures(1, &texture->handle));
        delete texture;
    }
}


void RenderDevice::UpdateTexture2D(Texture* texture, unsigned int level, const void* data)
{
    unsigned int w = std::max<unsigned int>(1, texture->width >> level);
    unsigned int h = std::max<unsigned int>(1, texture->height >> level);
    GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture->handle));
    GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
}


RenderTarget* RenderDevice::CreateRenderTarget(unsigned int width, unsigned int height, unsigned int colorFormat, unsigned int depthFormat)
{
    // create color texture
    Texture* colorTex = nullptr;
    if (colorFormat != TEXF_NONE)
    {
        colorTex = CreateTexture2D(width, height, 1, colorFormat, TEXTURE_USAGE_RENDER_TARGET);
        if (!colorTex)
        {
            LogPrintf("Unable to create color texture for render target (%u, %u @ %u, %u)",
                width, height, colorFormat, depthFormat);
            return nullptr;
        }
    }

    // create depth texture
    Texture* depthTex = nullptr;
    if (depthFormat != TEXF_NONE)
    {
        depthTex = CreateTexture2D(width, height, 1, depthFormat, TEXTURE_USAGE_DEPTH_BUFFER);
        if (!depthTex)
        {
            LogPrintf("Unable to create depth texture for render target (%u, %u @ %u, %u)",
                width, height, colorFormat, depthFormat);
            DeleteTexture(colorTex);
            return nullptr;
        }
    }

    // set dirty flags as we have to unbind the current framebuffer
    m_state |= DIRTY_TARGET;

    // create framebuffer
    GLuint framebuffer = 0;
    GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer));
    GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
    if (colorTex)
        GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorTex->handle, 0));
    if (depthTex)
        GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->handle, 0));

    // check framebuffer status
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LogPrintf("Framebuffer status != GL_FRAMEBUFFER_COMPLETE (%u, %u @ %u, %u)",
            width, height, colorFormat, depthFormat);
        DeleteTexture(colorTex);
        DeleteTexture(depthTex);
        return nullptr;
    }

    // create target
    RenderTarget* target = new RenderTarget();
    target->framebuffer = framebuffer;
    target->color = colorTex;
    target->depth = depthTex;
    return target;
}


void RenderDevice::DeleteRenderTarget(RenderTarget* target)
{
    if (target)
    {
        // unbind target
        if (m_activeTarget == target)
        {
            m_activeTarget = nullptr;
            m_state |= DIRTY_TARGET;
        }

        // unbind target samplers
        for (int i = 0; i < MAX_SAMPLERS; i++)
        {
            if (target->color && m_activeSamplers[i] == target->color ||
                target->depth && m_activeSamplers[i] == target->depth)
            {
                m_state |= DIRTY_SAMPLERS;
                m_samplerState |= (1 << i);
                m_activeSamplers[i] = nullptr;
            }
        }

        // delete target textures
        DeleteTexture(target->color);
        DeleteTexture(target->depth);

        // delete target framebuffer
        if (target->framebuffer)
            GL_CHECK_ERROR(glDeleteFramebuffers(1, &target->framebuffer));

        // release memory
        delete target;
    }
}


Texture* RenderDevice::GetRenderTargetColor(RenderTarget* target)
{
    return target ? target->color : nullptr;
}


Texture* RenderDevice::GetRenderTargetDepth(RenderTarget* target)
{
    return target ? target->depth : nullptr;
}