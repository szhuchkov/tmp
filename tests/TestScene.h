#pragma once


#include "RenderDevice.h"


class TestScene
{
private:
	struct DrawVertex
	{
		float		x, y, z;
		uint32_t	color;
	};

	VertexBuffer*	m_verts = nullptr;
	IndexBuffer*	m_inds = nullptr;
	Shader*			m_shader = nullptr;

	static constexpr char* g_TestVS = R"(
#version 100

attribute vec3 aPos;
attribute vec4 aColor;

varying vec4 vColor;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	vColor = aColor;
}
)";

	static constexpr char* g_TestFS = R"(
#version 100

varying lowp vec4 vColor;

void main()
{
	gl_FragColor = vColor;
}
)";

private:
	TestScene()
	{
	}

	~TestScene()
	{
	}

public:
	inline static TestScene* GetInstance()
	{
		static TestScene instance;
		return &instance;
	}

	bool Init()
	{
		static DrawVertex verts[] =
		{
			{ -0.5f, -0.5f, 0.0f, 0xffffffff },
			{ -0.5f,  0.5f, 0.0f, 0xff0000ff },
			{  0.5f,  0.5f, 0.0f, 0xff00ff00 },
			{  0.5f, -0.5f, 0.0f, 0xffff0000 },
		};

		static uint16_t inds[] =
		{
			0, 1, 2, 0, 2, 3
		};

		m_verts = RenderDevice::GetInstance()->CreateVertexBuffer(RenderDevice::VERTEX_XYZ | RenderDevice::VERTEX_COLOR, ARRAY_SIZE(verts), verts);
		if (!m_verts)
		{
			LogPrintf("Unable to create vertex buffer");
			return false;
		}

		m_inds = RenderDevice::GetInstance()->CreateIndexBuffer(RenderDevice::INDEX_16, ARRAY_SIZE(inds), inds);
		if (!m_inds)
		{
			LogPrintf("Unable to create index buffer");
			return false;
		}

		m_shader = RenderDevice::GetInstance()->CreateShader(g_TestVS, g_TestFS);
		if (!m_shader)
		{
			LogPrintf("Unable to create shader");
			return false;
		}

		return true;
	}

	void Shutdown()
	{
		RenderDevice::GetInstance()->DeleteVertexBuffer(m_verts);
		m_verts = nullptr;

		RenderDevice::GetInstance()->DeleteIndexBuffer(m_inds);
		m_inds = nullptr;

		RenderDevice::GetInstance()->DeleteShader(m_shader);
		m_shader = nullptr;
	}

	void Render()
	{
		RenderDevice::GetInstance()->SetVertexBuffer(m_verts);
		RenderDevice::GetInstance()->SetIndexBuffer(m_inds);
		RenderDevice::GetInstance()->SetShader(m_shader);
		RenderDevice::GetInstance()->DrawIndexedPrimitive(RenderDevice::PRIMITIVE_TRIANGLES, 0, 4, 0, 6);
	}
};
