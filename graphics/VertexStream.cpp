#include "VertexStream.h"

#include "graphics/VertexBuffer.h"

#include <cstring>

VertexStream::VertexStream(VertexBuffer &buffer) : buffer(&buffer), ptr(reinterpret_cast<char*>(buffer.vb.lock(Gx::Graphics::Lock::Flag::Discard)))
{
    buffer.n = 0;
}

VertexStream::VertexStream(VertexStream &&v) : buffer(v.buffer), ptr(v.ptr)
{
    v.buffer = nullptr;
}

VertexStream::~VertexStream()
{
    if(buffer)
    {
        buffer->vb.unlock();
    }
}

void VertexStream::write(const void *data, unsigned size)
{
    std::memcpy(ptr + buffer->n, data, size);
    buffer->n += size;
}
