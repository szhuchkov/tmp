#include "pch.h"
#include "TexFormatJPG.h"

extern "C" {
#include <jpeglib\jpeglib.h>
}


class TexFormatJPG::Context
{
public:
    Context(TexFormatJPG* tex) :
        m_tex(tex)
    {
        // setup err and create decompress
        m_dec.err = jpeg_std_error(&m_err);
        jpeg_create_decompress(&m_dec);

        // setup source
        m_src.context = this;
        m_src.fill_input_buffer = fill_input_buffer;
        m_src.init_source = init_source;
        m_src.resync_to_restart = jpeg_resync_to_restart;
        m_src.skip_input_data = skip_input_data;
        m_src.term_source = term_source;
        m_dec.src = &m_src;
    }

    ~Context()
    {
        jpeg_destroy_decompress(&m_dec);
    }

    bool Load(const char* raw, size_t size)
    {
        m_dataPtr = reinterpret_cast<const unsigned char*>(raw);
        m_dataSize = size;

        // read header
        jpeg_read_header(&m_dec, true);

        // setup output texture
        m_tex->width = m_dec.image_width;
        m_tex->height = m_dec.image_height;
        m_tex->data = new char[4 * m_tex->width * m_tex->height];

        // start decompress
        jpeg_start_decompress(&m_dec);

        // single line data and pointer to the scanlines
        unsigned char* rowData = new unsigned char[3 * m_tex->width];
        unsigned char* scanline[] = { rowData };

        // read each row from jpeg
        for (unsigned int rowIndex = 0; rowIndex < m_tex->width; rowIndex++)
        {
            jpeg_read_scanlines(&m_dec, scanline, 1);

            // copy raw data as RGBA32
            unsigned char* src = rowData;
            unsigned char* dst = reinterpret_cast<unsigned char*>(m_tex->data + 4 * m_tex->width * rowIndex);
            for (unsigned int i = 0; i < m_tex->width; i++)
            {
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = char(0xff);
            }
        }

        // finish decompression
        jpeg_finish_decompress(&m_dec);

        return true;
    }

private:
    bool FillBuffer()
    {
        return true;
    }

    void SkipData(long bytes)
    {
        long skip = std::min<long>(bytes, m_src.bytes_in_buffer);
        m_src.bytes_in_buffer -= skip;
        m_src.next_input_byte += skip;
    }

    void InitSource()
    {
        m_src.bytes_in_buffer = m_dataSize;
        m_src.next_input_byte = m_dataPtr;
    }

    void TermSource()
    {
    }

    static boolean fill_input_buffer(jpeg_decompress_struct* dec)
    {
        auto src = reinterpret_cast<SourceManager*>(dec->src);
        return src->context->FillBuffer() ? TRUE : FALSE;
    }

    static void init_source(jpeg_decompress_struct* dec)
    {
        auto src = reinterpret_cast<SourceManager*>(dec->src);
        src->context->InitSource();
    }

    static void skip_input_data(jpeg_decompress_struct* dec, long bytes)
    {
        auto src = reinterpret_cast<SourceManager*>(dec->src);
        src->context->SkipData(bytes);
    }

    static void term_source(jpeg_decompress_struct* dec)
    {
        auto src = reinterpret_cast<SourceManager*>(dec->src);
        src->context->TermSource();
    }

private:
    struct SourceManager : public jpeg_source_mgr
    {
        Context* context;
    };

    SourceManager m_src = {};
    TexFormatJPG* m_tex = nullptr;

    jpeg_decompress_struct m_dec = {};
    jpeg_error_mgr m_err = {};

    const unsigned char* m_dataPtr = nullptr;
    size_t m_dataSize = 0;
};


TexFormatJPG::TexFormatJPG()
{
    m_context = new Context(this);
}


TexFormatJPG::~TexFormatJPG()
{
    delete m_context;
}


bool TexFormatJPG::Load(const char* name)
{
    size_t size = 0;
    char* data = FS_LoadFile(name, &size);
    if (!data)
        return false;

    bool res = m_context->Load(data, size);

    delete[] data;

    return res;
}