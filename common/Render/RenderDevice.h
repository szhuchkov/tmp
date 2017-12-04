#pragma once


struct VertexBuffer;
struct IndexBuffer;
struct Shader;
struct Texture;
struct RenderTarget;


class RenderDevice
{
public:
    inline static RenderDevice* GetInstance()
    {
        static RenderDevice instance;
        return &instance;
    }

    // limits
    enum { MAX_SAMPLERS = 8 };
    enum { MAX_UNIFORMS = 20 };
    enum { MAX_MATRICES = 10 };

    struct ScreenSize
    {
        int     width;
        int     height;
        float   aspect;
    };

    enum ClearFlags
    {
        CLEAR_COLOR = 1,
        CLEAR_DEPTH = 2,
        CLEAR_STENCIL = 4,
    };

    enum PrimitiveType
    {
        PRIMITIVE_POINTS,
        PRIMITIVE_LINES,
        PRIMITIVE_LINE_LIST,
        PRIMITIVE_TRIANGLES,
        PRIMITIVE_TRIANGLE_LIST,
    };

    enum VertexFormat
    {
        VERTEX_POSITION = (1 << 0),
        VERTEX_NORMAL = (1 << 1),
        VERTEX_TEXCOORD = (1 << 2),
        VERTEX_TANGENT = (1 << 3),
        VERTEX_TBN = (1 << 4),
        VERTEX_BONE_WEIGHT = (1 << 5),
        VERTEX_BONE_INDEX = (1 << 6),
        VERTEX_COLOR = (1 << 7),

        VERTEX_DYNAMIC = (1 << 16),
    };

    enum IndexFormat
    {
        INDEX_16 = 0,
        INDEX_32 = (1 << 20),
        INDEX_DYNAMIC = (1 << 21),
    };

    enum TextureFormat
    {
        TEXF_NONE = 0,
        TEXF_A8,
        TEXF_A8R8G8B8,

        TEXF_A16F,
        TEXF_R16F,
        TEXF_A16R16G16B16F,

        TEXF_A32F,
        TEXF_R32F,
        TEXF_A32R32G32B32F,

        TEXF_DXT1,
        TEXF_DXT3,
        TEXF_DXT5,

        TEXF_D16,
        TEXF_D24,
        TEXF_D24S8,
    };

    enum TextureUsage
    {
        TEXTURE_USAGE_DEFAULT = 0,
        TEXTURE_USAGE_DYNAMIC,
        TEXTURE_USAGE_RENDER_TARGET,
        TEXTURE_USAGE_DEPTH_BUFFER,
    };

    enum MatrixName
    {
        MATRIX_WORLD,
        MATRIX_VIEW_PROJECTION,
        MATRIX_VIEW_INVERSE,
        MATRIX_SHADOW,
        MATRIX_LIGHT,
        MATRIX_USER_MATRIX_0,
    };

    bool Init(void* window, int width, int height, bool fullscreen);
    void Shutdown();
    bool Update();
    bool Resize(int width, int height);

    bool BeginFrame();
    void EndFrame();

    ScreenSize GetScreenSize();

    static unsigned int GetVertexStride(unsigned int format);
    static unsigned int	GetIndexSize(unsigned int format);

    VertexBuffer* CreateVertexBuffer(unsigned int format, unsigned int numVerts, const void* data);
    void DeleteVertexBuffer(VertexBuffer* buffer);
    void UpdateVertexBuffer(VertexBuffer* buffer, const void* data, unsigned int offset, unsigned int size);

    IndexBuffer* CreateIndexBuffer(unsigned int format, unsigned int numInds, const void* data);
    void DeleteIndexBuffer(IndexBuffer* buffer);
    void UpdateIndexBuffer(IndexBuffer* buffer, const void* data, unsigned int offset, unsigned int size);

    Shader* CreateShader(const char* vs, const char* fs);
    void DeleteShader(Shader* shader);

    Texture* CreateTexture2D(unsigned int width, unsigned int height, unsigned int levels, unsigned int format, unsigned int usage);
    void DeleteTexture(Texture* texture);
    void UpdateTexture2D(Texture* texture, unsigned int level, const void* data);

    RenderTarget* CreateRenderTarget(unsigned int width, unsigned int height, unsigned int colorFormat, unsigned int depthFormat);
    void DeleteRenderTarget(RenderTarget* target);
    Texture* GetRenderTargetColor(RenderTarget* target);
    Texture* GetRenderTargetDepth(RenderTarget* target);

    void Clear(unsigned int mask, const float* color, float depth, int stencil);
    void SetVertexBuffer(VertexBuffer* buffer);
    void SetIndexBuffer(IndexBuffer* buffer);
    void SetShader(Shader* shader);
    void SetTexture(unsigned int sampler, Texture* texture);
    void SetRenderTarget(RenderTarget* target);
    void SetUniform(unsigned int index, const Vector4& value);
    void SetMatrix(unsigned int index, const Matrix& value);
    void DrawPrimitive(unsigned int type, unsigned int offset, unsigned int count);
    void DrawIndexedPrimitive(unsigned int type, unsigned int vertexOffset, unsigned int vertexCount, unsigned int indexOffset, unsigned int count);

private:
    RenderDevice();
    ~RenderDevice();

    unsigned int CompileShader(const char* code, unsigned int type);
    void InitShader(Shader* shader);

    void ValidateState();
    void SetupVertexStream();

    // state flags
    enum
    {
        DIRTY_VERTS = (1 << 0),
        DIRTY_INDS = (1 << 1),
        DIRTY_SHADER = (1 << 2),
        DIRTY_SAMPLERS = (1 << 3),
        DIRTY_MATRICES = (1 << 4),
        DIRTY_UNIFORMS = (1 << 5),
        DIRTY_TARGET = (1 << 6),
    };

private:
    int                 m_width = 0;
    int                 m_height = 0;

    // current dirty state
    uint32_t            m_state = 0;
    uint32_t            m_samplerState = 0;

    // batching
    RenderTarget*       m_activeTarget = nullptr;
    VertexBuffer*       m_activeVerts = nullptr;
    IndexBuffer*        m_activeInds = nullptr;
    Shader*             m_activeShader = nullptr;
    unsigned int        m_activeAttribs = 0;
    unsigned int        m_activeStreamOffset = 0;

    // texture cache
    Texture*            m_activeSamplers[MAX_SAMPLERS] = {};

    // constant cache
    Matrix              m_matrixValue[MAX_MATRICES];
    int                 m_matrixVersion[MAX_MATRICES] = {};
    Vector4             m_uniformValue[MAX_UNIFORMS];
    int                 m_uniformVersion[MAX_UNIFORMS] = {};
};
