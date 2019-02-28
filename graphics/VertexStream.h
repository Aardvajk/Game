#ifndef VERTEXSTREAM_H
#define VERTEXSTREAM_H

#include <pcx/non_copyable.h>

class VertexBuffer;

class VertexStream : public pcx::non_copyable
{
public:
    explicit VertexStream(VertexBuffer &buffer);
    VertexStream(VertexStream &&v);
    ~VertexStream();

    operator bool() const { return true; }

    template<typename T> VertexStream &operator<<(const T &value){ write(&value, sizeof(T)); return *this; }

private:
    void write(const void *data, unsigned size);

    VertexBuffer *buffer;
    char *ptr;
};

#endif // VERTEXSTREAM_H
