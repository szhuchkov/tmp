#include "pch.h"
#include "RenderDevice.h"
#include "OGLDriver.h"


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
    "Uniform0",
    "Uniform1",
    "Uniform2",
    "Uniform3",
    "Uniform4",
    "Uniform5",
    "Uniform6",
    "Uniform7",
    "Uniform8",
    "Uniform9",
    "Uniform10",
    "Uniform11",
    "Uniform12",
    "Uniform13",
    "Uniform14",
    "Uniform15",
    "Uniform16",
    "Uniform17",
    "Uniform18",
    "Uniform19",
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

    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

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
		glClearColor(color[0], color[1], color[2], color[3]);
		mask |= GL_COLOR_BUFFER_BIT;
	}

	// depth bit
	if (flags & CLEAR_DEPTH)
	{
		glClearDepth(depth);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	// stencil bit
	if (flags & CLEAR_STENCIL)
	{
		glClearStencil(stencil);
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
	glDrawArrays(mode, offset, count);
}


void RenderDevice::DrawIndexedPrimitive(unsigned int type, unsigned int vertexOffset, unsigned int vertexCount, unsigned int indexOffset, unsigned int count)
{
    ValidateState();

	GLenum mode = GLPrimitiveType(type);

	unsigned int indexSize = m_activeInds->stride;
	GLenum indexType = (indexSize == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT);

#if PLATFORM_ANDROID
	glDrawElements(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize));
#else
	//if (vertexOffset > 0)
	//	glDrawElementsBaseVertex(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize), vertexOffset);
	//else
		glDrawElements(mode, count, indexType, GL_BUFFER_POINTER(indexOffset * indexSize));
#endif
}


void RenderDevice::ValidateState()
{
    // render target
    if (m_state & DIRTY_TARGET)
    {
        if (m_activeTarget)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_activeTarget->framebuffer);
            glViewport(0, 0, m_activeTarget->color->width, m_activeTarget->color->height);
        }
        else
        {
            glBindBuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, m_width, m_height);
        }
    }

    // vertex buffer
    if (m_state & DIRTY_VERTS)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_activeVerts ? m_activeVerts->handle : 0);
    }

    // index buffer
    if (m_state & DIRTY_INDS)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_activeInds ? m_activeInds->handle : 0);
    }

    // shader
    if (m_state & DIRTY_SHADER)
    {
        glUseProgram(m_activeShader ? m_activeShader->program : 0);
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
                    glUniform4fv(m_activeShader->uniformLocation[i], 1, reinterpret_cast<const float*>(&m_uniformValue[i]));
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
                    glUniformMatrix4fv(m_activeShader->matrixLocation[i], 1, GL_FALSE, reinterpret_cast<const float*>(&m_matrixValue[i]));
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
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, m_activeSamplers[i] ? m_activeSamplers[i]->handle : 0);
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
				glVertexAttribPointer(
					m_activeShader->attribLocation[i],
					g_VertexAttribDesc[i].size,
					g_VertexAttribDesc[i].type,
					g_VertexAttribDesc[i].normalized,
					m_activeVerts->stride, GL_BUFFER_POINTER(offset));
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
			glEnableVertexAttribArray(i);
		}
		else if (last && !curr)
		{
			glDisableVertexAttribArray(i);
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
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer->handle);
		delete buffer;
	}
}


void RenderDevice::UpdateVertexBuffer(VertexBuffer* buffer, const void* data, unsigned int offset, unsigned int size)
{
	// use zero to fill entire buffer
	if (!size)
		size = buffer->stride * buffer->numVerts;

	glBindBuffer(GL_ARRAY_BUFFER, buffer->handle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer->handle);
		delete buffer;
	}
}


void RenderDevice::UpdateIndexBuffer(IndexBuffer* buffer, const void* data, unsigned int offset, unsigned int size)
{
	// use zero to fill entire buffer
	if (!size)
		size = buffer->stride * buffer->numInds;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->handle);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


unsigned int RenderDevice::CompileShader(const char* code, unsigned int type)
{
	// create shader and attach sources
	GLuint shader = glCreateShader(type);
	GLint length = strlen(code);
	glShaderSource(shader, 1, &code, &length);
	glCompileShader(shader);

	// get compilation log
	char buffer[10000] = { 0 };
	GLsizei logSize;
	glGetShaderInfoLog(shader, sizeof(buffer), &logSize, buffer);
	if (logSize > 0)
	{
		LogPrintf("Shader info log:");
		LogPrintf("%s", buffer);
	}

	// check compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}


void RenderDevice::InitShader(Shader* shader)
{
    //TODO: use glProgramUniform*
    glUseProgram(shader->program);

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
		glDeleteShader(vs);
		glDeleteShader(fs);
		LogPrintf("Unable to create shader");
		return NULL;
	}

	// create program and attach shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// delete shaders now
	glDeleteShader(vs);
	glDeleteShader(fs);

	// get log
	char buffer[10000] = { 0 };
	GLsizei logSize;
	glGetProgramInfoLog(program, sizeof(buffer), &logSize, buffer);
	if (logSize > 0)
	{
		LogPrintf("Program info log:");
		LogPrintf("%s", buffer);
	}

	// check link status
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glDeleteProgram(program);
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
		glDeleteProgram(shader->program);
		delete shader;
	}
}


Texture* RenderDevice::CreateTexture2D(unsigned int width, unsigned int height, unsigned int levels, unsigned int format, unsigned int usage)
{
    GLuint handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    if (usage == TEXTURE_USAGE_DEPTH_BUFFER)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

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
        glDeleteTextures(1, &texture->handle);
        delete texture;
    }
}


void RenderDevice::UpdateTexture2D(Texture* texture, unsigned int level, const void* data)
{
    unsigned int w = std::max<unsigned int>(1, texture->width >> level);
    unsigned int h = std::max<unsigned int>(1, texture->height >> level);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}
