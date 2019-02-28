#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GxGraphics/GxGraphicsResource.h>
#include <GxGraphics/GxVertexBuffer.h>

#include <pcx/buffer.h>
#include <pcx/datastream.h>

class VertexBuffer : public Gx::GraphicsResource
{
public:
    VertexBuffer();
    VertexBuffer(Gx::GraphicsDevice &device, unsigned bytes, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool);
    VertexBuffer(Gx::GraphicsDevice &device, const pcx::buffer &buffer, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool);
    VertexBuffer(Gx::GraphicsDevice &device, pcx::data_istream &ds, Gx::Graphics::Usage::Flags usage, Gx::Graphics::Pool pool);

    virtual void reset(Gx::GraphicsDevice &device) override;
    virtual void release() override;

    virtual bool isDeviceBound() const override;

    unsigned size() const;
    unsigned capacity() const;

    void renderTriangleList(Gx::GraphicsDevice &device, unsigned stride) const;
    void renderLineList(Gx::GraphicsDevice &device, unsigned stride) const;

private:
    friend class VertexStream;

    Gx::VertexBuffer vb;
    unsigned n;
};

#endif // VERTEXBUFFER_H
