#include "VertexBuffer.h"

#include <GxGraphics/GxGraphicsDevice.h>
#include <GxGraphics/GxBufferStream.h>

#include <cstring>

VertexBuffer::VertexBuffer() = default;

VertexBuffer::VertexBuffer(Gx::GraphicsDevice &device, unsigned bytes, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool) : n(0)
{
    vb.reset(device, { bytes, usage, pool });
}

VertexBuffer::VertexBuffer(Gx::GraphicsDevice &device, const pcx::buffer &buffer, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool)
{
    n = buffer.size();
    vb.reset(device, { n, usage, pool });

    Gx::BufferStream<Gx::VertexBuffer> os(vb, Gx::Graphics::Lock::Flag::Discard);
    os.write(buffer.data(), n);
}

VertexBuffer::VertexBuffer(Gx::GraphicsDevice &device, pcx::data_istream &ds, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool)
{
    n = ds.get<unsigned>();
    vb.reset(device, { n, usage, pool });

    Gx::BufferStream<Gx::VertexBuffer> os(vb, Gx::Graphics::Lock::Flag::Discard);
    ds.read(os.data(), n);
}

void VertexBuffer::reset(Gx::GraphicsDevice &device)
{
    vb.reset(device);
}

void VertexBuffer::release()
{
    vb.release();
}

bool VertexBuffer::isDeviceBound() const
{
    return vb.isDeviceBound();
}

unsigned VertexBuffer::size() const
{
    return n;
}

unsigned VertexBuffer::capacity() const
{
    return vb.size();
}

void VertexBuffer::renderTriangleList(Gx::GraphicsDevice &device, unsigned stride) const
{
    device.renderTriangleList(vb, 0, (n / stride) / 3);
}

void VertexBuffer::renderLineList(Gx::GraphicsDevice &device, unsigned stride) const
{
    device.renderLineList(vb, 0, (n / stride) / 2);
}

void VertexBuffer::renderPointList(Gx::GraphicsDevice &device, unsigned stride) const
{
    device.renderPointList(vb, 0, n / stride);
}
